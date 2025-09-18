/**
 * @file twi.h
 * @brief Header file with declarations and macros for hardware TWI/I2C.
 * 
 * This file provides function prototypes, type definitions, and constants
 * for hardware-based TWI (I2C) communication on AVR microcontrollers.
 * 
 * @author g.raf
 * @date 2025-09-01
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the GPLv3 License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger project and subject to the license specified in the repository. For updates and the complete revision history, see the GitHub repository.
 * 
 * @see https://github.com/0x007e/avr0  "AVR ATmega GitHub Repository"
 */

#ifndef TWI_H_
#define TWI_H_

    #ifndef F_PER
        /**
         * @def F_PER
         * @brief Peripheral clock frequency definition.
         *
         * @details
         * This macro defines the operating frequency of the microcontroller's peripheral clock in Hertz. It is used for timing calculations. The value should match the actual peripheral hardware clock frequency to ensure correct timing behavior.
         */
        #define F_PER 10000000UL
    #endif

    #ifndef F_TWI
        /**
         * @def F_TWI
         * @brief TWI clock frequency definition.
         *
         * @details
         * This macro defines the clock frequency used by the `TWI` (`I2C`) bus in Hertz. It sets the speed at which the hardware generates clock pulses for data transfer. Modifying this value affects timing and the overall bus speed.
         */
        #define F_TWI 100000UL
    #endif 

    #ifndef T_RISE
		#define T_RISE 1/(float)10000000.0
	#endif

    #ifndef TWI_BITRATE
        /**
            * @def TWI_BITRATE
            * @brief Calculation of the TWI bus clock bitrate value.
            *
            * @details
            * This macro calculates the value for the `TWI` bitrate register used to set the clock frequency on the TWI bus. The calculation is based on the CPU clock frequency (`F_CPU`), the desired TWI clock frequency (`F_TWI`), and the prescaler value (`TWI_PRESCALER`).
            *
            * @attention This value defines the timing for clock pulses for the hardware `TWI` bus to achieve the target bus speed.
            */
        #define TWI_BITRATE (unsigned char)(((float)F_PER)/(float)((F_TWI<<1)) - 5UL - ((unsigned int)(F_PER * (T_RISE))>>1))
    #endif

    #ifndef TWI_ADDRESS
        /**
         * @def TWI_ADDRESS
         * @brief Slave address of the TWI controller.
         *
         * @details
         * This macro defines the 7-bit address of the `TWI` (I2C) controller when operating as a slave device on the bus. The address is used by other devices (masters) to communicate with this slave.
         * 
         * @attention it must be unique on the bus to avoid address conflicts.
         */
        #define TWI_ADDRESS 0x42
    #endif

    #ifndef TWI_BROADCAST
        /**
         * @def TWI_BROADCAST
         * @brief Enable or disable listening to general call addresses on the TWI bus.
         *
         * @details
         * This macro configures whether the `TWI` controller, when acting as a slave, responds to general call addresses.
         * 
         * Setting this to `1` enables the controller to listen and respond to general call messages (address `0x00`). Setting to `0` disables general call response, so the controller only reacts to its specific slave address.
         */
        #define TWI_BROADCAST 0x00
    #endif

	#ifndef TWI_TIMEOUT
		/**
		 * @def TWI_TIMEOUT
		 * @brief Configure the bus time-out detection for the TWI (I2C) controller.
		 *
		 * @details
		 * This macro determines how long the TWI controller waits for bus activity before declaring a time-out condition.
		 *
		 * The value can be set to predefined time-out codes:
		 * - `TWI_TIMEOUT_DISABLED_gc`: Bus time-out disabled (default). Time-out detection is turned off.
		 * - `TWI_TIMEOUT_50US_gc`: Bus time-out after 50 �s (SMBus, baud assumed at 100 kHz).
		 * - `TWI_TIMEOUT_100US_gc`: Bus time-out after 100 �s (baud assumed at 100 kHz).
		 * - `TWI_TIMEOUT_200US_gc`: Bus time-out after 200 �s (baud assumed at 100 kHz).
		 *
		 * Setting the inactive bus time-out (TIMEOUT) will enable the inactive bus time-out supervisor. If the bus is inactive for longer than the TIMEOUT setting, the bus state logic will enter the Idle state.
		 */
        #define TWI_TIMEOUT TWI_TIMEOUT_DISABLED_gc
    #endif
	
	#ifndef TWI_PORTMUX
		/**
		 * @def TWI_PORTMUX
		 * @brief Select the pin routing configuration for the TWI (I2C) module.
		 *
		 * @details
		 * This macro configures which physical pins are assigned to the TWI controller for SCL and SDA signals.
		 *
		 * The value can be set to predefined routing codes:
		 * - `PORTMUX_TWI0_DEFAULT_gc`: Default mapping; SCL/SDA referenced in datasheet (e.g. ATmega4808 -> PA[3:2]).
		 * - `PORTMUX_TWI0_ALT1_gc`: Alternative 1; SCL/SDA referenced in datasheet (e.g. ATmega4808 -> PA[3:2]).
		 * - `PORTMUX_TWI0_ALT2_gc`: Alternative 2; SCL/SDA referenced in datasheet (e.g. ATmega4808 -> PC[3:2]).
		 *
		 * Selecting a TWI port mapping is essential when adapting hardware layouts or using multiple TWI modules. In dual TWI mode, each mapping determines the location of slave pins separately from master pins.
		 */
        #define TWI_PORTMUX PORTMUX_TWI0_ALT2_gc
    #endif

    #ifndef TWI_TWIE
        /**
         * @def TWI_TWIE
         * @brief Enable or disable TWI interrupt processing mode.
         *
         * @details
         * This macro defines whether the `TWI` (I2C) communication is handled using interrupts or polling. If defined, TWI processing is intended to be interrupt-driven.
         * 
         * @attention However, interrupts are currently not implemented in this library, so if this macro is defined, polling will be disabled.
         * 
         * @note Users enabling this should be aware that interrupt functionality is not included in this library.
         */
        // #define TWI_TWIE
    #endif

    #include <avr/io.h>

    #ifndef TWI_TWIE
        #include <avr/interrupt.h>
    #endif

    #include "../../../common/defines/TWI_defines.h"
    #include "../../../common/enums/TWI_enums.h"

    unsigned char twi_init(void);
            void twi_disable(void);
    unsigned char twi_status(void);

    #ifndef TWI_TWIE
        TWI_Error twi_start(void);
            void twi_stop(void);
        TWI_Error twi_address(unsigned char address, TWI_Operation operation);
        TWI_Error twi_set(unsigned char data);
        TWI_Error twi_get(unsigned char *data, TWI_Acknowledge acknowledge);
    #endif

#endif /* TWI_H_ */