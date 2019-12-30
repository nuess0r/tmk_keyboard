/* Copyright 2012 Jun Wako <wakojun@gmail.com>
 *
 * This is heavily based on phantom/board.{c|h}.
 * https://github.com/BathroomEpiphanies/AVR-Keyboard
 *
 * Copyright (c) 2012 Fredrik Atmer, Bathroom Epiphanies Inc
 * http://bathroomepiphanies.com
 *
 * As for liscensing consult with the original files or its author.
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"


#ifndef DEBOUNCE
#   define DEBOUNCE	10
#endif
static uint8_t debouncing = DEBOUNCE;

// bit array of key state(1:on, 0:off)
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

#ifdef MATRIX_HAS_GHOST
bool matrix_has_ghost_in_row(uint8_t row);
#endif
static uint16_t read_rows(void);
static void init_rows(void);
static void unselect_cols(void);
static void select_col(uint8_t col);

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // initialize row and col
    unselect_cols();
    init_rows();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++)  {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {  // 0-16
        select_col(col);
        _delay_us(30);       // without this wait it won't read stable value.
        uint16_t rows = read_rows();
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {  // 0-5
            bool prev_bit = matrix_debouncing[row] & ((matrix_row_t)1<<col);
            bool curr_bit = rows & (1<<row);
            if (prev_bit != curr_bit) {
                matrix_debouncing[row] ^= ((matrix_row_t)1<<col);
                if (debouncing) {
                    dprint("bounce!: "); dprintf("%02X", debouncing); dprintln();
                }
                debouncing = DEBOUNCE;
            }
        }
        unselect_cols();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 01234567\n");
    for (uint8_t row = 0; row < matrix_rows(); row++) {
        phex(row); print(": ");
        pbin_reverse(matrix_get_row(row));
#ifdef MATRIX_HAS_GHOST
        if (matrix_has_ghost_in_row(row)) {
            print(" <ghost");
        }
#endif
        print("\n");
    }
}

#ifdef MATRIX_HAS_GHOST
inline
bool matrix_has_ghost_in_row(uint8_t row)
{
    // no ghost exists in case less than 2 keys on
    if (((matrix[row] - 1) & matrix[row]) == 0)
        return false;

    // ghost exists in case same state as other row
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        if (i != row && (matrix[i] & matrix[row]))
            return true;
    }
    return false;
}
#endif

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop32(matrix[i]);
    }
    return count;
}

/* Row pin configuration
 * row: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
 * pin: A0 A1 A2 A3 A4 A5 A6 A7 C7 C6 C5 C4 C3 C2 C1 C0
 */
static void init_rows(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRA  &= ~0b11111111;
    PORTA |=  0b11111111;
    DDRC  &= ~0b11111111;
    PORTC |=  0b11111111;
}

static uint16_t read_rows(void)
{
    return (PINA&(1<<0) ? 0 : (1<<0)) |
           (PINA&(1<<1) ? 0 : (1<<1)) |
           (PINA&(1<<2) ? 0 : (1<<2)) |
           (PINA&(1<<3) ? 0 : (1<<3)) |
           (PINA&(1<<4) ? 0 : (1<<4)) |
           (PINA&(1<<5) ? 0 : (1<<5)) |
           (PINA&(1<<6) ? 0 : (1<<6)) |
           (PINA&(1<<7) ? 0 : (1<<7)) |
           (PINC&(1<<7) ? 0 : (1<<8)) |
           (PINC&(1<<6) ? 0 : (1<<9)) |
           (PINC&(1<<5) ? 0 : (1<<10)) |
           (PINC&(1<<4) ? 0 : (1<<11)) |
           (PINC&(1<<3) ? 0 : (1<<12)) |
           (PINC&(1<<2) ? 0 : (1<<13)) |
           (PINC&(1<<1) ? 0 : (1<<14)) |
           (PINC&(1<<0) ? 0 : (1<<15));
}

/* Column pin configuration
 * col:  0  1  2  3  4  5  6  7
 * pin: F0 F1 F2 F3 F4 F5 F6 E7
 */
static void unselect_cols(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRB  |= 0b11111111; // PB: 7 6 5 4 3 2 1 0
    PORTB |= 0b11111111;
}

static void select_col(uint8_t col)
{
    DDRB  |=  (1 << col);
    PORTB &= ~(1 << col);
}
