/**
 * @file twi_soft.c
 * @brief Source file with implementation of software TWI/I2C functions and macros.
 * 
 * This file contains the definitions of function implementations and macros 
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
 * @see https://github.com/0x007e/avr "AVR ATmega GitHub Repository"
 */

#include "twi_soft.h"

unsigned char twi_soft_bus_status;

/**
 * @brief Initialize the software TWI (I2C) bus.
 *
 * @return Returns `TWI_SOFT_STATUS_BUS_INIT_COMPLETE` if the bus was successfully initialized, or `TWI_SOFT_STATUS_BUS_ERROR` if a bus initialization error occurred.
 *
 * @details
 * This function initializes the software `TWI` bus by configuring the `SDA` and `SCL` lines to their idle high state and checking if the bus lines are free (not held low). It introduces delays to ensure proper timing according to the configured TWI clock pulse duration. If either the `SCL` or `SDA` line is stuck low, the function returns a bus error status. Otherwise, it confirms that the bus has been initialized successfully.
 */
unsigned char twi_soft_init(void)
{
    TWI_SOFT_PORT &= ~((1<<TWI_SOFT_SCL) | (1<<TWI_SOFT_SDA));

    SCL_HIGH();
    SDA_HIGH();
    
    _delay_us(TWI_SOFT_CLOCK_PULSE_US);
    _delay_us(TWI_SOFT_CLOCK_PULSE_US);

    if( !(TWI_SOFT_PIN & (1<<TWI_SOFT_SCL)) || 
        !(TWI_SOFT_PIN & (1<<TWI_SOFT_SDA)))
    {
        return TWI_SOFT_STATUS_BUS_ERROR;
    }
    return TWI_SOFT_STATUS_BUS_INIT_COMPLETE;
}

/**
 * @brief Disable the software TWI (I2C) interface.
 *
 * @details
 * This function disables the software `TWI` module by clearing the `SDA` and `SCL` bits in the port and data direction registers. This effectively releases control of the TWI lines and sets the pins to a low output state. Use this function to safely deactivate the TWI bus when it is no longer needed.
 */
void twi_soft_disable(void)
{
    TWI_SOFT_PORT &= ~((1<<TWI_SOFT_SCL) | (1<<TWI_SOFT_SDA));
    TWI_SOFT_DDR &= ~((1<<TWI_SOFT_SCL) | (1<<TWI_SOFT_SDA));
}

/**
 * @brief Get the current status of the software TWI bus.
 *
 * @return The status byte indicating the current state of the TWI bus.
 *
 * @details
 * This inline function returns the value of the internal status variable `twi_soft_bus_status` which reflects the current operational state or error conditions of the software TWI interface.
 */
inline unsigned char twi_soft_status(void)
{
    return twi_soft_bus_status;
}

/**
 * @brief Generate a TWI start or repeated start condition.
 *
 * @return Returns `TWI_None` to indicate the start condition was initiated successfully.
 *
 * @details
 * This function produces a start condition on the `TWI` (I2C) bus which is used to initiate communication with a slave device. It sets the data line (`SDA`) low while the clock line (`SCL`) is high, creating the required falling edge signaling the start condition.
 *
 * @note Unlike a normal start function that requires the bus to be free, this implementation does not check the bus state and thus can also generate a repeated start condition if invoked during an ongoing transmission without issuing a stop condition first.
 *
 * @attention Interrupts can be blocked during the critical timing to ensure precise signal timing if the `TWI_SOFT_BLOCK_IE` macro is defined.
 */
TWI_Error twi_soft_start(void)
{
    twi_soft_bus_status |= TWI_SOFT_STATUS_TRANSMIT;

    #ifdef TWI_SOFT_BLOCK_IE
        cli();
    #endif

    SCL_HIGH();
    _delay_us(TWI_SOFT_CLOCK_PULSE_US);

    SDA_LOW();
    _delay_us(TWI_SOFT_CLOCK_PULSE_US);
    SCL_LOW();

    #ifdef TWI_SOFT_BLOCK_IE
        sei();
    #endif

    return TWI_None;
}


/**
 * @brief Stop the TWI (I2C) transmission.
 *
 * @details
 * This function generates the `TWI` stop condition by first pulling both SCL and SDA lines low, then releasing SCL high, followed by releasing SDA high, with appropriate delays to ensure bus timing compliance. After sending the stop condition, the transmit status flag is cleared to indicate the end of the transmission.
 */
void twi_soft_stop(void)
{
    SCL_LOW();
    SDA_LOW();
    _delay_us(TWI_SOFT_CLOCK_PULSE_US);
    SCL_HIGH();
    _delay_us(TWI_SOFT_CLOCK_PULSE_US);
    SDA_HIGH();
    _delay_us(TWI_SOFT_CLOCK_PULSE_US);

    twi_soft_bus_status &= ~TWI_SOFT_STATUS_TRANSMIT;
}

/**
 * @brief Sends the slave address with the read/write operation on the TWI bus.
 *
 * @param address 7-bit address of the slave device.
 * @param operation Specifies the operation type: `TWI_Write` or `TWI_Read`.
 *
 * @return Returns `TWI_None` if addressing was successful, TWI_Arbitration if arbitration was lost during transmission, or `TWI_General` if a general error occurred.
 *
 * @details
 * This function prepares and sends the 8-bit address byte on the `TWI` bus, combining the 7-bit slave address shifted left by 1 with the least significant bit indicating the operation (`read` or `write`). The actual transmission and status handling is performed by `twi_soft_set()`.
 */
TWI_Error twi_soft_address(unsigned char address, TWI_Operation operation)
{
    unsigned char temp = (address<<1) | (0x01 & operation);   // Write data to data register
    return twi_soft_set(temp);
}

/**
 * @brief Transmit a byte of data on the TWI bus.
 *
 * @param data The data byte to be transmitted.
 *
 * @return Returns `TWI_None` if the data was successfully transmitted and acknowledged, `TWI_Arbitration` if arbitration was lost during transmission, or `TWI_General` if a general error occurred.
 *
 * @details
 * This function sends the given 8-bit data byte bit-by-bit over the software `TWI` bus, handling the clock and data line states manually. It checks for arbitration loss when transmitting 1 bits and waits for clock stretching by the slave. After transmission, it reads the acknowledgment bit from the slave to determine successful reception.
 */
TWI_Error twi_soft_set(unsigned char data)
{
    for (unsigned char i=0; i < CHAR_BIT; i++)
    {
        unsigned char temp = 0;

        SCL_LOW();
        if((data<<i) & 0x80)
        {
            SDA_HIGH();
            temp = 1;
        }
        else
        {
            SDA_LOW();
        }
        _delay_us(TWI_SOFT_CLOCK_PULSE_US);

        // Check for arbitration
        if( (temp == 1) &&
            (!(TWI_SOFT_PIN & (1<<TWI_SOFT_SDA))))
        {
            return TWI_Arbitration;
        }
        SCL_HIGH();

        // Clock stretching active?
        while(!(TWI_SOFT_PIN & (1<<TWI_SOFT_SCL)))
        {
            asm volatile("NOP");
        }
        _delay_us(TWI_SOFT_CLOCK_PULSE_US);
    }
    SCL_LOW();
    SDA_HIGH();
    _delay_us(TWI_SOFT_CLOCK_PULSE_US);

    TWI_Error temp = TWI_Ack;

    if(!(TWI_SOFT_PIN & (1<<TWI_SOFT_SDA)))
    {
        temp =  TWI_None;
    }

    SCL_HIGH();
    _delay_us(TWI_SOFT_CLOCK_PULSE_US);
    return temp;
}

/**
 * @brief Reads a byte of data from the TWI bus and sends an ACK or NACK.
 *
 * @param data Pointer to a variable where the received data byte will be stored.
 * @param acknowledge Specifies whether to send an ACK (TWI_ACK) or NACK (TWI_NACK) after reception.
 *
 * @return Returns `TWI_None` if the data byte was received successfully, or `TWI_General` for other errors.
 *
 * @details
 * This function reads one data byte from the software `TWI` bus by manually controlling the clock and data lines. After reception, it sends either an acknowledgment (`ACK`) or not-acknowledgment (`NACK`) bit to the transmitter depending on the 'acknowledge' parameter, following the `TWI` protocol.
 */
TWI_Error twi_soft_get(unsigned char *data, TWI_Acknowledge acknowledge)
{
    unsigned char received = 0;

    for (unsigned char i = 0; i < CHAR_BIT; i++)
    {
        SCL_LOW();
        _delay_us(TWI_SOFT_CLOCK_PULSE_US);

        SCL_HIGH();

        // Clock stretching active?
        while (!(TWI_SOFT_PIN & (1<<TWI_SOFT_SCL)))
        {
            asm volatile("NOP");
        }
        _delay_us(TWI_SOFT_CLOCK_PULSE_US);

        // Read bit from SDA
        received <<= 1;
        if (TWI_SOFT_PIN & (1<<TWI_SOFT_SDA))
        {
            received |= 1;
        }
    }

    // Prepare to send ACK or NACK bit
    SCL_LOW();

    if (acknowledge == TWI_ACK)
    {
        SDA_LOW();
    }
    else
    {
        SDA_HIGH();
    }

    _delay_us(TWI_SOFT_CLOCK_PULSE_US);

    SCL_HIGH();

     // Clock stretching active?
    while (!(TWI_SOFT_PIN & (1<<TWI_SOFT_SCL)))
    {
        asm volatile("NOP");
    }
    _delay_us(TWI_SOFT_CLOCK_PULSE_US);

    SCL_LOW();
    SDA_HIGH();

    *data = received;

    return TWI_None;
}
