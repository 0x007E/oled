/**
 * @file twi.c
 * @brief Source file with implementation of hardware TWI/I2C functions and macros.
 * 
 * This file contains the definitions of function implementations and macros 
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
 * @see https://github.com/0x007e/avr "AVR ATmega GitHub Repository"
 */

#include "twi.h"

/**
 * @brief Initialize the TWI (I2C) hardware interface in master mode.
 *
 * @return Returns the current TWI status byte after initialization.
 *
 * @details
 * This function configures the `TWI` hardware registers to initialize the bus in master mode.
 * 
 * In master mode, it sets the bitrate and prescaler according to predefined macros.
 * 
 * @note If TWI interrupt processing is enabled (`TWI_TWIE`), interrupts are activated.
 * 
 * The function returns the initial TWI status for further status checking after initialization.
 */
unsigned char twi_init(void)
{
    TWBR = (unsigned char)(TWI_BITRATE);    // Setup TWI Bitrate
    TWSR = (unsigned char)(TWI_PRESCALER);   // Setup TWI Prescaler
			
    // TWI interrupt setup
    #ifdef TWI_TWIE
        TWCR  |= (1<<TWIE);
    #endif

    // Return message initialization
    return twi_status();
}

/**
 * @brief Disable the TWI (I2C) interface.
 *
 * @details
 * This function disables the hardware `TWI` module. This effectively releases control of the TWI lines and sets the pins to reset state. Use this function to safely deactivate the TWI bus when it is no longer needed.
 */
void twi_disable(void)
{
    TWCR &= ~((1<<TWEA) | (1<<TWEN) | (1<<TWIE));
    
    #ifdef TWI_TWIE
        TWCR  &= ~(1<<TWIE);
    #endif
}

/**
 * @brief Retrieves the current TWI status byte.
 *
 * @details
 * Reads and returns the `TWI` status byte from the `TWSR` register, including the write collision flag.
 * - Bits `7` to `3` (masked with `0xF8`) contain the TWI status code which indicates the current state of the TWI hardware (see `TWI_STATUS_*` defines).
 * - Bit `2` indicates a write collision error.
 *
 * @return
 * A byte where:
 * - Bits `[7:3]` represent the TWI status code.
 * - Bit `[2]` indicates a write collision (1 if collision occurred, 0 otherwise).
 */
inline unsigned char twi_status(void)
{
    return (0xF8 & TWSR) | (0x04 & (TWCR>>1));
}

#ifndef TWI_TWIE
    /**
     * @brief Initiates a TWI (I2C) (repeated) start condition on the bus.
     *
     * @return Returns `TWI_None` if the start condition was successfully transmitted (including repeated start), or `TWI_Start` if the start condition failed.
     *
     * @details
     * This function triggers the `TWI` hardware to send a START condition on the bus, signaling the beginning of a new communication frame. It sets the necessary control bits in the TWCR register to initiate the start and enables the TWI interface.
     * 
     * @note The function then waits for the TWINT flag to indicate completion of the start condition transmission.
     * 
     * After completion, the function checks the TWI status register (TWSR) to verify if the start or repeated start condition was acknowledged by the bus. If successful, it returns `TWI_None`; otherwise, it returns `TWI_Start` indicating failure to send the (repeated) start condition.
     */
    TWI_Error twi_start(void)
    {
        TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); // Start a TWI transmission
                            
        // Check if START is done
        while(!(TWCR & (1<<TWINT)))
            asm volatile("NOP");
    
        // Check if an error occurred
        if( ((TWSR & 0xF8) == TWI_STATUS_START) || 
            ((TWSR & 0xF8) == TWI_STATUS_REPEATED_START))
        {
            return TWI_None;    // (Repeated)Start successfully
        }
        return TWI_Start;       // (Repeated)Start failure
    }

    /**
     * @brief Stop the TWI (I2C) transmission by generating a stop condition.
     *
     * @details
     * This function initiates the `TWI` stop condition on the bus, signaling the end of the current communication frame. It sets the appropriate bits in the `TWCR` register to transmit the stop signal and keeps the `TWI` hardware enabled.
     * @note The function waits until the stop condition has been executed by checking the `TWSTO` bit in the control register. After this, the bus returns to the idle state, ready for the next communication cycle.
     */
    void twi_stop(void)
    {
        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); // Stop a TWI transmission
    
        while(!(TWCR & (1<<TWSTO)))
            asm volatile("NOP");
    }

    /**
     * @brief Send a TWI (I2C) slave address combined with the read/write operation bit.
     *
     * @param address 7-bit slave address to be transmitted.
     * @param operation Specifies the operation type: `0` for write, `1` for read.
     *
     * @return Returns the status code from the `twi_set` function indicating the result of address transmission:
     * - `TWI_None` if the address was acknowledged by the slave.
     * - `TWI_Ack` if an acknowledgement error (NACK) occurred.
     * - `TWI_Arbitration` if arbitration was lost in multi-master mode.
     * - `TWI_General` if a general error occurred.
     *
     * @details
     * This inline function constructs the address byte by shifting the 7-bit slave address left by one and combining it with the least significant bit representing the operation (read/write). It then calls `twi_set` to transmit this address byte over the TWI bus and returns the transmission status.
     * 
     * @note The function relies on `twi_set` for the actual data transmission and error checking.
     */
    inline TWI_Error twi_address(unsigned char address, TWI_Operation operation)
    {
        return twi_set((address<<1) | (0x01 & operation));
    }

    /**
     * @brief Transmit a data byte over the TWI (I2C) bus.
     *
     * @param data The data byte to be transmitted to the bus.
     *
     * @return Returns one of the following status codes:
     * - `TWI_None`: Data was transmitted successfully and acknowledged by the slave.
     * - `TWI_Ack`: Acknowledgement error occurred (NACK received).
     * - `TWI_Arbitration`: Arbitration lost in multi-master mode.
     * - `TWI_General`: A general error occurred during transmission.
     *
     * @details
     * This function writes the provided data byte into the `TWI` data register and triggers the transmission by setting the `TWINT` and `TWEN` bits in the control register.
     * 
     * @note The function waits for the transmission to complete by polling the `TWINT flag`.
     * 
     * After transmission, the function reads the TWI status register (TWSR) to determine the outcome of the transmission and returns the corresponding status code.
     */
    TWI_Error twi_set(unsigned char data)
    {
        TWDR = data;                                // Write data to data register
        TWCR = (1<<TWINT) | (1<<TWEN);              // Transmit databyte
    
        // Check if transmission done
        while(!(TWCR & (1<<TWINT)))
            asm volatile("NOP");
        
        switch ((TWSR & 0xF8))
        {
            case TWI_STATUS_DATA_WRITE_ACK:
                return TWI_None;
            case TWI_STATUS_DATA_WRITE_NACK:
                return TWI_Ack;
            case TWI_STATUS_ARBITRATION_LOST:
                return TWI_Arbitration;
            default:  
                return TWI_General;
        }
    }

    /**
     * @brief Receive a data byte from the TWI (I2C) bus.
     *
     * @param data Pointer to a variable where the received data byte will be stored.
     * @param acknowledge Specifies whether the master should send an ACK or NACK after receiving the byte:
     * - `TWI_ACK` to acknowledge receipt and request more data.
     * - `TWI_NACK` to signal no more data is requested.
     *
     * @return Returns one of the following status codes:
     * - `TWI_None`: Data was successfully received and the expected ACK/NACK was sent.
     * - `TWI_Ack`: Wrong or unexpected ACK/NACK parameter or status received.
     * - `TWI_Arbitration`: Arbitration was lost in multi-master mode.
     *
     * @details
     * This function configures the `TWI` hardware to receive a byte from the bus and decides whether to send an `ACK` or `NACK` based on the `acknowledge` parameter.
     * 
     * @note The function waits until the reception is complete by polling the `TWINT` flag. The received data is then read from the TWDR register and stored at the provided pointer.
     * 
     * The function checks the TWI status register (`TWSR`) for arbitration loss or to verify that the appropriate `ACK`/`NACK` status was received. If the received status matches the expected acknowledge mode, it returns success; otherwise, it indicates an acknowledgment error.
     */
    TWI_Error twi_get(unsigned char *data, TWI_Acknowledge acknowledge)
    {
        // Setup if ACK should be sent
        if(acknowledge == TWI_ACK)
            TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
        else
            TWCR = (1<<TWINT) | (1<<TWEN);
    
        // Check if transmission done
        while (!(TWCR & (1<<TWINT)))
        {
            asm volatile("NOP");
        }
    
        // TWI_Write Data to pointer
        *data = TWDR;
    
        // Check if arbitration lost (only multi-master mode)
        if((TWSR & 0xF8) == TWI_STATUS_ARBITRATION_LOST)
        {
            return TWI_Arbitration;
        }

        // Check if an error occurred
        if((acknowledge == TWI_ACK) && ((TWSR & 0xF8) == TWI_STATUS_DATA_READ_ACK))
        {
            return TWI_None;
        }
        else if((acknowledge == TWI_NACK) && ((TWSR & 0xF8) == TWI_STATUS_DATA_READ_NACK))
        {
            return TWI_None;
        }
        return TWI_Ack;
    }
#endif