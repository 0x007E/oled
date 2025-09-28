/**
 * @file TWI_enums.h
 * @brief Enumerations for TWI/I2C interface modes, operations, acknowledgment, and error codes.
 * 
 * This header file defines the enums used to represent various states and configurations related to software TWI (Two-Wire Interface, I2C) communication. It includes enumerations for device operation modes (master/slave), communication operation types (read/write), acknowledgment status (ACK/NACK), and error codes for handling communication results.
 * 
 * @note These enums improve code readability by providing meaningful symbolic names for TWI-related values, facilitating protocol-compliant and maintainable software design.
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

#ifndef TWI_ENUM_H_
#define TWI_ENUM_H_

    #include "../defines/TWI_defines.h"
    
    /**
     * @typedef TWI_Mode
     * @brief Alias for enum TWI_Mode_t representing TWI device mode.
     */
    typedef enum TWI_Mode_t TWI_Mode;

    /**
     * @enum TWI_Operation_t
     * @brief Specifies the operation type for the next TWI bus transmission.
     *
     * @details
     * This enumeration indicates whether the next software TWI (I2C) communication operation is a write (master to slave) or a read (slave to master).
     */
    enum TWI_Operation_t
    {
        TWI_Write=TWI_WRITE,    /**< Write operation (master sends data) */
        TWI_Read=TWI_READ       /**< Read operation (master receives data) */
    };
    /**
     * @typedef TWI_Operation
     * @brief Alias for enum TWI_Operation_t representing TWI operation modes.
     */
    typedef enum TWI_Operation_t TWI_Operation;

    /**
     * @enum TWI_Acknowledge_t
     * @brief Indicates acknowledgment status on the TWI bus.
     *
     * @details
     * This enumeration represents whether an acknowledgment (ACK) or a not-acknowledgment (NACK) was received following a data transfer on the software TWI (I2C) bus.
     */
    enum TWI_Acknowledge_t
    {
        TWI_NACK=TWI_NACKNOWLEDGE,  /**< No acknowledgment received (NACK) */
        TWI_ACK=TWI_ACKNOWLEDGE     /**< Acknowledgment received (ACK) */
    };
    /**
     * @typedef TWI_Acknowledge
     * @brief Alias for enum TWI_Acknowledge_t representing TWI acknowledge mode.
     */
    typedef enum TWI_Acknowledge_t TWI_Acknowledge;

    /**
     * @enum TWI_Error_t
     * @brief Error codes for software TWI operations.
     *
     * @details
     * This enumeration defines the possible error statuses that can occur
     * during software TWI (I2C) communication.
     */
    enum TWI_Error_t
    {
        TWI_None=0,         /**< No error occurrs */
		TWI_Run,			/**< Operation running */
        TWI_Start,          /**< Start condition error */
        TWI_Arbitration,    /**< Arbitration error */
        TWI_Ack,            /**< Acknowledge error */
        TWI_General         /**< General error */
    };
    /**
     * @typedef TWI_Error
     * @brief Alias for enum TWI_Error_t representing TWI error status.
     */
    typedef enum TWI_Error_t TWI_Error;

#endif /* TWI_ENUM_H_ */