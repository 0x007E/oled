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
 * @see https://github.com/0x007e/avr  "AVR ATmega GitHub Repository"
 */

#ifndef TWI_H_
#define TWI_H_

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

    #ifndef TWI_PRESCALER
        /**
         * @def TWI_PRESCALER
         * @brief TWI clock prescaler setting.
         *
         * @details
         * This macro defines the prescaler value for the `TWI` hardware clock. It sets the division factor applied to the TWI clock frequency (`F_TWI`) by configuring the prescaler bits (TWPS) in the TWSR register.
         * 
         * @note Valid prescaler values are `0`, `1`, `2`, and `3`, representing division factors of `1`, `4`, `16`, and `64` respectively.
         * 
         * @attention Adjusting this value changes the effective `SCL` clock speed on the `TWI` bus by scaling down the base clock frequency.
         */
        #define TWI_PRESCALER 0
    #endif

    #ifndef TWI_BITRATE
        #ifdef TWI_PRESCALER
            /**
             * @def TWI_BITRATE
             * @brief Calculation of the TWI bus clock bitrate value.
             *
             * @details
             * This macro calculates the value for the `TWI` bitrate register used to set the clock frequency on the TWI bus. The calculation is based on the CPU clock frequency (`F_CPU`), the desired TWI clock frequency (`F_TWI`), and the prescaler value (`TWI_PRESCALER`).
             *
             * @attention This value defines the timing for clock pulses for the hardware `TWI` bus to achieve the target bus speed.
             */
            #define TWI_BITRATE ((F_CPU/F_TWI) - 16UL) / (2 * (1<<TWI_PRESCALER) * (1<<TWI_PRESCALER))
        #endif
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

    /** 
     * @defgroup TWI_Status_Codes TWI Status Codes
     * @brief Status codes returned by the TWI hardware interface.
     *
     * These macros define the possible `TWI` status codes as returned in the `TWSR` register, representing various states and events such as start conditions, address acknowledgments, data transfer acknowledgments, arbitration loss, and error conditions.
     * @{
     */
    #ifndef TWI_STATUS_START
        /**
         * @def TWI_STATUS_START
         * @brief Status code indicating START condition has been transmitted.
         */
        #define TWI_STATUS_START 0x08
    #endif

    #ifndef TWI_STATUS_REPEATED_START
        /**
         * @def TWI_STATUS_REPEATED_START
         * @brief Status code indicating a repeated START condition has been transmitted.
         */
        #define TWI_STATUS_REPEATED_START 0x10
    #endif

    #ifndef TWI_STATUS_ADDRESS_WRITE_NACK
        /**
         * @def TWI_STATUS_ADDRESS_WRITE_NACK
         * @brief Status code indicating SLA+W has been transmitted and NOT acknowledged.
         */
        #define TWI_STATUS_ADDRESS_WRITE_NACK 0x18
    #endif

    #ifndef TWI_STATUS_ADDRESS_WRITE_ACK
        /**
         * @def TWI_STATUS_ADDRESS_WRITE_ACK
         * @brief Status code indicating SLA+W has been transmitted and acknowledged.
         */
        #define TWI_STATUS_ADDRESS_WRITE_ACK 0x20
    #endif

    #ifndef TWI_STATUS_DATA_WRITE_NACK
        /**
         * @def TWI_STATUS_DATA_WRITE_NACK
         * @brief Status code indicating data byte has been transmitted and NOT acknowledged.
         */
        #define TWI_STATUS_DATA_WRITE_NACK 0x28
    #endif

    #ifndef TWI_STATUS_DATA_WRITE_ACK
        /**
         * @def TWI_STATUS_DATA_WRITE_ACK
         * @brief Status code indicating data byte has been transmitted and acknowledged.
         */
        #define TWI_STATUS_DATA_WRITE_ACK 0x30
    #endif

    #ifndef TWI_STATUS_ARBITRATION_LOST
        /**
         * @def TWI_STATUS_ARBITRATION_LOST
         * @brief Status code indicating arbitration has been lost in SLA+W or data bytes.
         */
        #define TWI_STATUS_ARBITRATION_LOST 0x38
    #endif

    #ifndef TWI_STATUS_ADDRESS_READ_ACK
        /**
         * @def TWI_STATUS_ADDRESS_READ_ACK
         * @brief Status code indicating SLA+R has been transmitted and acknowledged.
         */
        #define TWI_STATUS_ADDRESS_READ_ACK 0x40
    #endif

    #ifndef TWI_STATUS_ADDRESS_READ_NACK
        /**
         * @def TWI_STATUS_ADDRESS_READ_NACK
         * @brief Status code indicating SLA+R has been transmitted and NOT acknowledged.
         */
        #define TWI_STATUS_ADDRESS_READ_NACK 0x48
    #endif

    #ifndef TWI_STATUS_DATA_READ_ACK
        /**
         * @def TWI_STATUS_DATA_READ_ACK
         * @brief Status code indicating data byte has been received and acknowledged.
         */
        #define TWI_STATUS_DATA_READ_ACK 0x50
    #endif

    #ifndef TWI_STATUS_DATA_READ_NACK
        /**
         * @def TWI_STATUS_DATA_READ_NACK
         * @brief Status code indicating data byte has been received and NOT acknowledged.
         */
        #define TWI_STATUS_DATA_READ_NACK 0x58
    #endif
    /** @} */

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

    #ifdef TWI_TWIE
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