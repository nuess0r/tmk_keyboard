#include <stdbool.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "matrix.h"
#include "action.h"
#include "backlight.h"
#include "suspend_avr.h"
#include "suspend.h"
#include "timer.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#endif

#ifdef SUSPEND_MODE_STANDBY
static void standby(void)
{
#ifdef SLEEP_MODE_STANDBY
    set_sleep_mode(SLEEP_MODE_STANDBY);
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
#endif
}
#endif

static void idle(void)
{
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
}


void suspend_idle(uint8_t time)
{
    idle();
}

void suspend_power_down(void)
{
#ifdef NO_SUSPEND_POWER_DOWN
    ;
#elif defined(SUSPEND_MODE_NOPOWERSAVE)
    ;
#elif defined(SUSPEND_MODE_STANDBY)
    standby();
#elif defined(SUSPEND_MODE_IDLE)
    idle();
#else
    power_down(WDTO_15MS);
#endif
}

bool suspend_wakeup_condition(void)
{
    matrix_power_up();
    matrix_scan();
    matrix_power_down();
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        if (matrix_get_row(r)) return true;
    }
    return false;
}

// run immediately after wakeup
void suspend_wakeup_init(void)
{
    // clear keyboard state
    matrix_clear();
    clear_keyboard();
#ifdef BACKLIGHT_ENABLE
    backlight_init();
#endif
}

#ifndef NO_SUSPEND_POWER_DOWN
/* watchdog timeout */
ISR(WDT_vect)
{
    // compensate timer for sleep
    switch (wdt_timeout) {
        case WDTO_15MS:
            timer_count += 15 + 2;  // WDTO_15MS + 2(from observation)
            break;
        default:
            ;
    }
}
#endif
