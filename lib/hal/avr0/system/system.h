/**
 * @file system.h
 * @brief Header file with declarations and macros for system clock configuration.
 * 
 * This file provides macro definitions, constants, and function prototypes for configuring the system clock and peripheral clock prescaler on AVR microcontrollers. It defines the available clock sources (`SYSTEM_CLOCK`), the corresponding status bit (`SYSTEM_CLOCK_BIT`), and the optional peripheral prescaler (`SYSTEM_PER_CLOCK_PRESCALER`).
 *
 * @author g.raf
 * @date 2025-09-18
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the MIT License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger embedded systems project and subject to the license specified in the repository. For updates  and the complete revision history, see the GitHub repository.
 * 
 * @see https://github.com/0x007e/avr0 "AVR ATmega GitHub Repository"
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#ifndef SYSTEM_CLOCK
    /**
     * @def SYSTEM_CLOCK
     * @brief Defines the system main clock source for the microcontroller.
     *
     * @details
     * This macro selects the active system clock source from the available options in the AVR ``CLKCTRL`` module.
     *
     * Possible values:
     * - `CLKCTRL_CLKSEL_OSC20M_gc` : Internal 20 MHz oscillator  
     * - `CLKCTRL_CLKSEL_OSCULP32K_gc` : Internal 32 kHz ultra low power oscillator  
     * - `CLKCTRL_CLKSEL_EXTCLK_gc` : External clock source  
     *
     * By default, `SYSTEM_CLOCK` is set to `CLKCTRL_CLKSEL_OSC20M_gc` (20 MHz internal oscillator).
     *
     * @note Changing the system clock source has an impact on all peripherals that rely on clock timing (e.g., timers, delay functions, UART baud rate).
     */
    #define SYSTEM_CLOCK CLKCTRL_CLKSEL_OSC20M_gc

    /**
     * @def SYSTEM_CLOCK_BIT
     * @brief Defines the bit mask corresponding to the current system clock source.
     *
     * @details
     * This macro provides the bit mask that represents the currently selected system clock source. It is typically used when checking or configuring oscillator status bits in the ``CLKCTRL`` registers.
     *
     * Possible values (depending on SYSTEM_CLOCK):
     * - `CLKCTRL_OSC20MS_bm` : Mask for the 20 MHz internal oscillator  
     * - `CLKCTRL_OSC32KS_bm` : Mask for the 32 kHz internal oscillator  
     * - `CLKCTRL_EXTS_bm`    : Mask for the external clock source  
     *
     * By default, `SYSTEM_CLOCK_BIT` is set to match `CLKCTRL_CLKSEL_OSC20M_gc` → `CLKCTRL_OSC20MS_bm`.
     *
     * @note This macro complements `SYSTEM_CLOCK` by indicating the appropriate bit mask for low-level register operations.
     */
    #ifndef SYSTEM_CLOCK_BIT
        #define SYSTEM_CLOCK_BIT CLKCTRL_OSC20MS_bm
    #endif
#endif

#ifndef SYSTEM_PER_CLOCK_PRESCALER
    /**
     * @def SYSTEM_PER_CLOCK_PRESCALER
     * @brief Defines the prescaler setting for the peripheral clock (peripheral frequency).
     *
     * @details
     * This macro determines how the system clock (`F_CPU`) is divided down to generate the peripheral clock used by timers, communication peripherals, and other hardware modules.  
     *
     * Possible values (from ``CLKCTRL`` module):
     * - `CLKCTRL_PDIV_2X_gc`   : F_CPU / 2  
     * - `CLKCTRL_PDIV_4X_gc`   : F_CPU / 4  
     * - `CLKCTRL_PDIV_8X_gc`   : F_CPU / 8  
     * - `CLKCTRL_PDIV_16X_gc`  : F_CPU / 16  
     * - `CLKCTRL_PDIV_32X_gc`  : F_CPU / 32  
     * - `CLKCTRL_PDIV_64X_gc`  : F_CPU / 64  
     * - `CLKCTRL_PDIV_6X_gc`   : F_CPU / 6  
     * - `CLKCTRL_PDIV_10X_gc`  : F_CPU / 10  
     * - `CLKCTRL_PDIV_24X_gc`  : F_CPU / 24  
     * - `CLKCTRL_PDIV_48X_gc`  : F_CPU / 48  
     *
     * If the macro is undefined, the default is **no prescaling** (`F_CPU / 1`).
     *
     * @note Lowering the peripheral clock frequency can reduce power consumption but may affect timing accuracy and maximum baud rates of communication peripherals.
     */
    #define SYSTEM_PER_CLOCK_PRESCALER CLKCTRL_PDIV_2X_gc
#endif

#include <avr/io.h>

void system_init(void);

#endif /* SYSTEM_H_ */