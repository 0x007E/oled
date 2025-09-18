/**
 * @file twi_soft.h
 * @brief Header file with declarations and macros for software TWI/I2C.
 * 
 * This file provides function prototypes, type definitions, and constants
 * for software-based TWI (I2C) communication on AVR microcontrollers.
 * 
 * @author g.raf
 * @date 2025-09-01
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the MIT License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger project and subject to the license specified in the repository. For updates and the complete revision history, see the GitHub repository.
 * 
 * @see https://github.com/0x007e/avr  "AVR ATmega GitHub Repository"
 */

#ifndef TWI_SOFT_H_
#define TWI_SOFT_H_

    #ifndef F_CPU
        /**
         * @def F_CPU
         * @brief System clock frequency definition.
         *
         * @details
         * This macro defines the operating frequency of the microcontroller's clock in Hertz. It is used by delay functions and timing calculations. The value should match the actual hardware clock frequency to ensure correct timing behavior in the software.
         */
        #define F_CPU 12000000UL
    #endif

    #ifndef F_SOFT_TWI
        /**
         * @def F_SOFT_TWI
         * @brief TWI clock frequency definition.
         *
         * @details
         * This macro defines the clock frequency used by the software-emulated `TWI` (`I2C`) implementation in Hertz. It sets the speed at which the software generates clock pulses for data transfer. Modifying this value affects the timing delays and the overall bus speed.
         */
        #define F_SOFT_TWI 100000UL
    #endif  

    #ifndef TWI_SOFT_CLOCK_PULSE_US
        /**
         * @def TWI_SOFT_CLOCK_PULSE_US
         * @brief Duration of a half clock pulse in microseconds.
         *
         * @details
         * This macro calculates the length of one half clock pulse period in microseconds based on the configured `TWI` clock frequency (`F_SOFT_TWI`). his value is used to generate timing delays in the software TWI implementation to produce the desired clock signal frequency.
         */
        #define TWI_SOFT_CLOCK_PULSE_US (float)(1000000.0/(F_SOFT_TWI<<1))
    #endif

    #ifndef TWI_SOFT_PULLUP_ENABLE
        /**
         * @def TWI_SOFT_PULLUP_ENABLE
         * @brief Enable internal pull-up resistors for TWI lines.
         *
         * @details
         * Defining this macro enables the use of internal pull-up resistors on the `SDA` and `SCL` lines of the software `TWI` implementation. When enabled, the software configures the GPIO pins with internal pull-ups, eliminating the need for external pull-up resistors if the hardware supports it. If this macro is not defined, the software assumes external pull-up resistors are present and handles the lines differently.
         */
        // #define TWI_SOFT_PULLUP_ENABLE

        #ifdef _DOXYGEN_    // Used for documentation, can be ignored
            #define TWI_SOFT_PULLUP_ENABLE
        #endif
    #endif

    #ifndef TWI_SOFT_DDR
        /**
         * @def TWI_SOFT_DDR
         * @brief Data Direction Register for the TWI port.
         *
         * @details
         * This macro defines which Data Direction Register (`DDR`) is used for the software `TWI` (`I2C`) implementation. It controls the direction (input/output) of the GPIO pins used for the `TWI` clock (`SCL`) and data (`SDA`) lines. By default, it is set to `DDRB`, but can be redefined to match the actual hardware pin configuration used in the project.
         */
        #define TWI_SOFT_DDR DDRB 
    #endif

    #ifndef TWI_SOFT_PORT
        /**
         * @def TWI_SOFT_PORT
         * @brief Port register for the TWI lines.
         *
         * @details
         * This macro defines the PORT register used for the software `TWI` (`I2C`) implementation on the AVR microcontroller. It is responsible for writing output values to the GPIO pins connected to the `TWI` clock (`SCL`) and data (`SDA`) lines. By default, it is set to `PORTB`, but can be redefined to match the actual hardware wiring in the project.
         */
        #define TWI_SOFT_PORT PORTB
    #endif

    #ifndef TWI_SOFT_PIN
        /**
         * @def TWI_SOFT_PIN
         * @brief Port register for the TWI lines.
         *
         * @details
         * This macro defines the `PORT` register used for the software `TWI` (`I2C`) implementation on the AVR microcontroller. It is responsible for writing output values to the GPIO pins connected to the TWI clock (`SCL`) and data (`SDA`) lines. By default, it is set to `PORTB`, but can be redefined to match the actual hardware wiring in the project.
         */
        #define TWI_SOFT_PIN PINB
    #endif

    #ifndef TWI_SOFT_SCL
        /**
         * @def TWI_SOFT_SCL
         * @brief Pin number for TWI Clock line (SCL).
         *
         * @details
         * This macro defines the specific pin number on the port that is used for the software `TWI` clock line (`SCL`) in the AVR microcontroller. By default, it is set to `PINB0`, but can be redefined to fit the hardware
         */
        #define TWI_SOFT_SCL PINB0
    #endif

    #ifndef TWI_SOFT_SDA
        /**
         * @def TWI_SOFT_SDA
         * @brief Pin number for TWI Data line (SDA).
         *
         * @details
         * This macro defines the specific pin number on the port that is used for the software `TWI` data line (`SDA`) in the AVR microcontroller. By default, it is set to `PINB1`, but can be redefined to fit the hardware
         */
        #define TWI_SOFT_SDA PINB1
    #endif

    /**
     * @defgroup TWI_SOFT_STATUS Status flags for software TWI transmission.
     * @brief Different status flags that can be set during software TWI transmission.
     *
     * @details
     * Flags used to indicate the current state of TWI communication:
     * - `bus init complete`
     * - `transmit running`
     * - `receive running`
     * - `bus error`
     * 
     * @{
     */
    #ifndef TWI_SOFT_STATUS
        #define TWI_SOFT_STATUS_BUS_INIT_COMPLETE 0x00 /**< Bus initialization complete */
        #define TWI_SOFT_STATUS_TRANSMIT          0x01 /**< Transmission running */
        #define TWI_SOFT_STATUS_RECEIVE           0x02 /**< Reception running */
        #define TWI_SOFT_STATUS_BUS_ERROR         0xFF /**< Bus error occurred */
    #endif
    /** @} */

    #ifndef TWI_SOFT_BLOCK_IE
        /**
         * @def TWI_SOFT_BLOCK_IE
         * @brief Enable blocking of all system interrupts during TWI operations.
         *
         * @details
         * When defined, this macro causes the software TWI implementation to disable all system interrupts while a `TWI` transmission is in progress. This  ensures uninterrupted communication on the TWI bus but may affect  responsiveness to other interrupt-driven events. Use with caution to balance real-time requirements of the system.
         *
         * \warning
         * All interrupts of the system will be blocked until the twi transmission is complete!
         */
        // #define TWI_SOFT_BLOCK_IE

        #ifdef _DOXYGEN_    // Used for documentation, can be ignored
            #define TWI_SOFT_BLOCK_IE
        #endif
    #endif

    #include <limits.h>
    #include <avr/io.h>
    #include <util/delay.h>

    #ifdef TWI_SOFT_BLOCK_IE
        #include <avr/interrupt.h>
    #endif

    #include "../../../common/defines/TWI_defines.h"
    #include "../../../common/enums/TWI_enums.h"

    /**
     * @defgroup TWI_Control_Macros Software TWI (I2C) Control Macros
     * @brief Macros for controlling SCL and SDA lines in software TWI implementation.
     * 
     * This group contains macros to set `SCL` and `SDA` lines to logical `HIGH` or `LOW`.
     * The implementation differs based on whether `TWI_SOFT_PULLUP_ENABLE` is defined:
     * - With pull-up enabled: `PORT` and `DDR` registers are adjusted.
     * - Without pull-up: only `DDR` registers are modified.
     * 
     * @{
     */
    #ifdef TWI_SOFT_PULLUP_ENABLE
        #define SCL_LOW()  { TWI_SOFT_PORT &= ~(1<<TWI_SOFT_SCL); TWI_SOFT_DDR |= (1<<TWI_SOFT_SCL); }
        #define SDA_LOW()  { TWI_SOFT_PORT &= ~(1<<TWI_SOFT_SDA); TWI_SOFT_DDR |= (1<<TWI_SOFT_SDA); }
        #define SCL_HIGH() { TWI_SOFT_DDR &= ~(1<<TWI_SOFT_SCL);  TWI_SOFT_PORT |= (1<<TWI_SOFT_SCL); }
        #define SDA_HIGH() { TWI_SOFT_DDR &= ~(1<<TWI_SOFT_SDA);  TWI_SOFT_PORT |= (1<<TWI_SOFT_SDA); }
    #else
        #define SCL_LOW()  { TWI_SOFT_PORT &= ~(1<<TWI_SOFT_SCL); TWI_SOFT_DDR |= (1<<TWI_SOFT_SCL); }
        #define SDA_LOW()  { TWI_SOFT_PORT &= ~(1<<TWI_SOFT_SCL); TWI_SOFT_DDR |= (1<<TWI_SOFT_SDA); }
        #define SDA_HIGH() { TWI_SOFT_DDR &= ~(1<<TWI_SOFT_SDA);  TWI_SOFT_PORT &= ~(1<<TWI_SOFT_SCL); }
        #define SCL_HIGH() { TWI_SOFT_DDR &= ~(1<<TWI_SOFT_SCL);  TWI_SOFT_PORT &= ~(1<<TWI_SOFT_SCL); }
    #endif
    /** @} */

    unsigned char twi_soft_init(void);
             void twi_soft_disable(void);
    unsigned char twi_soft_status(void);

    TWI_Error twi_soft_start(void);
            void twi_soft_stop(void);
    TWI_Error twi_soft_address(unsigned char address, TWI_Operation operation);
    TWI_Error twi_soft_set(unsigned char data);
    TWI_Error twi_soft_get(unsigned char *data, TWI_Acknowledge acknowledge);

#endif /* TWI_SOFT_H_ */