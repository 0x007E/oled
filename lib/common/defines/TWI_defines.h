
/**
 * @file TWI_defines.h
 * @brief Macro definitions for TWI/I2C communication flags and operations.
 * 
 * This header file contains essential macros for handling TWI (Two-Wire Interface) communication.
 * 
 * @note These macros serve to improve code clarity and maintain protocol compliance by providing meaningful names for standard TWI/I2C signal and operation indicators.
 * 
 * @author g.raf
 * @date 2025-09-02
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the GPLv3 License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger project and subject to the license specified in the repository. For updates and the complete revision history, see the GitHub repository.
 * 
 * @see https://github.com/0x007e/avr-common  "AVR Common GitHub Repository"
 */

#ifndef TWI_DEFINES_H_
#define TWI_DEFINES_H_

    #ifndef TWI_ACKNOWLEDGE
        /**
         * @def TWI_ACKNOWLEDGE
         * @brief Flag representing a TWI acknowledgment (ACK).
         *
         * @details
         * This macro defines the value used to indicate that an acknowledgment has been received on the TWI (I2C) bus. It is typically used to signal successful data reception or transmission acknowledgement.
         */
        #define TWI_ACKNOWLEDGE 0x01
    #endif

    #ifndef TWI_NACKNOWLEDGE
        /**
         * @def TWI_NACKNOWLEDGE
         * @brief Flag representing a TWI not-acknowledgment (NACK).
         *
         * @details
         * This macro defines the value used to indicate that a not-acknowledgment (NACK) has been received on the TWI (I2C) bus. It signals that the last transmitted byte was not acknowledged by the receiver.
         */
        #define TWI_NACKNOWLEDGE 0x00
    #endif

    #ifndef TWI_WRITE
        /**
         * @def TWI_NACKNOWLEDGE
         * @brief Flag representing a TWI write operation.
         *
         * @details
         * This macro defines the value used to indicate a write command on the TWI (I2C) bus, where the master sends data to a slave device.
         */
        #define TWI_WRITE 0x00
    #endif

    #ifndef TWI_READ
        /**
         * @def TWI_READ
         * @brief Flag representing a TWI read operation.
         *
         * @details
         * This macro defines the value used to indicate a read command on the TWI (I2C) bus, where the master requests data from the slave device.
         */
        #define TWI_READ 0x01
    #endif

#endif /* TWI_DEFINES_H_ */