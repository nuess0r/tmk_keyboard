/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* 
 * Keymap for Phantom controller
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "host.h"
#include "debug.h"
#include "keymap.h"

// Convert physical keyboard layout to matrix array
// This is a macro to define keymap easily in keyboard layout form.
#define KEYMAP( \
    K5A,      K5B, K5C, K5D, K5E,   K5F, K5G, K5H, K5I,   K5J, K5K, K5L, K5M,   K5N, K5O, K5P, \
    \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K, K4L, K4M, K4N,   K4O, K4P, K4Q,   K4R, K4S, K4T, K4U, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L, K3M, K3N,   K3O, K3P, K3Q,   K3R, K3S, K3T, K3U, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L, K2M,                         K2N, K2O, K2P, \
    K1A, K1B, K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L,                  K1M,        K1N, K1O, K1P, K1Q, \
    K0A, K0B, K0C, K0D, K0E,                                                K0F, K0G, K0H,   K0I,      K0J \
) { \
/* 00 */ { KC_NO,    KC_NO,    KC_##K0A, KC_NO,    KC_NO,    KC_NO,    KC_##K0E, KC_NO    }, \
/* 01 */ { KC_NO,    KC_##K1A, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##K1L, KC_NO    }, \
/* 02 */ { KC_##K5A, KC_##K3A, KC_##K4A, KC_##K4B, KC_##K3B, KC_##K2B, KC_##K1B, KC_NO    }, \
/* 03 */ { KC_NO,    KC_##K2A, KC_##K5B, KC_##K4C, KC_##K3C, KC_##K2C, KC_##K1C, KC_NO    }, \
/* 04 */ { KC_##K5E, KC_##K5D, KC_##K5C, KC_##K4D, KC_##K3D, KC_##K2D, KC_##K1D, KC_NO    }, \
/* 05 */ { KC_##K2F, KC_##K3F, KC_##K4F, KC_##K4E, KC_##K3E, KC_##K2E, KC_##K1E, KC_##K1F }, \
/* 06 */ { KC_##K5F, KC_##K4N, KC_##K5J, KC_##K5K, KC_NO,    KC_##K3N, KC_##K2M, KC_##K0C }, \
/* 07 */ { KC_##K2G, KC_##K3G, KC_##K4G, KC_##K4H, KC_##K3H, KC_##K2H, KC_##K1H, KC_##K1G }, \
/* 08 */ { KC_##K5G, KC_##K3M, KC_##K4M, KC_##K4I, KC_##K3I, KC_##K2I, KC_##K1I, KC_NO    }, \
/* 09 */ { KC_NO,    KC_##K5H, KC_##K5I, KC_##K4J, KC_##K3J, KC_##K2J, KC_##K1J, KC_NO    }, \
/* 0A */ { KC_##K2L, KC_##K3L, KC_##K4L, KC_##K4K, KC_##K3K, KC_##K2K, KC_NO,    KC_##K1K }, \
/* 0B */ { KC_NO,    KC_##K2N, KC_##K3O, KC_##K5L, KC_##K3R, KC_##K1N, KC_##K4R, KC_##K0G }, \
/* 0C */ { KC_##K0I, KC_##K2O, KC_##K4O, KC_##K5M, KC_##K3S, KC_##K1O, KC_##K4S, KC_##K0H }, \
/* 0D */ { KC_##K0J, KC_##K2P, KC_##K4Q, KC_##K3Q, KC_##K3T, KC_##K1P, KC_##K4T, KC_##K4U }, \
/* 0E */ { KC_##K1M, KC_NO,    KC_##K4P, KC_##K3P, KC_##K3U, KC_##K1Q, KC_##K5P, KC_##K0F }, \
/* 0F */ { KC_##K0B, KC_NO,    KC_NO,    KC_##K5N, KC_##K5O, KC_NO,    KC_NO,    KC_##K0D }  \
}
/*         0         1         2         3         4         5         6         7       */

static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP(
        GRV, F1, F2, F3, F4,  F5, F6, F7, F8,  F9, MRWD, MPLY, MFFD,                MUTE,VOLD,VOLU,
        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS, EQL, BSPC,     INS,  HOME, PGUP,    BRK,PSLS,PAST,PMNS,
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC, RBRC,BSLS,     DEL, END, PGDN,      P7,  P8,  P9, PPLS,
        LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,     ENT,                            P4,  P5,  P6,
        LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,          RSFT,          UP,              P1,  P2,  P3, PENT,
        LCTL,LGUI,                  SPC,                           RGUI, RALT,     LEFT,DOWN,RGHT,       P0,       PDOT
    )
};
static const uint16_t PROGMEM fn_actions[] = {
};

#define KEYMAPS_SIZE    (sizeof(keymaps) / sizeof(keymaps[0]))
#define FN_ACTIONS_SIZE (sizeof(fn_actions) / sizeof(fn_actions[0]))

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
    if (layer < KEYMAPS_SIZE) {
        return pgm_read_byte(&keymaps[(layer)][(key.row)][(key.col)]);
    } else {
        // fall back to layer 0
        return pgm_read_byte(&keymaps[0][(key.row)][(key.col)]);
    }
}

/* translates Fn keycode to action */
action_t keymap_fn_to_action(uint8_t keycode)
{
    action_t action;
    if (FN_INDEX(keycode) < FN_ACTIONS_SIZE) {
        action.code = pgm_read_word(&fn_actions[FN_INDEX(keycode)]);
    } else {
        action.code = ACTION_NO;
    }
    return action;
}
