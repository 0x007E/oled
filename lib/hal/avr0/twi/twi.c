/**
 * @file twi.c
 * @brief Source file with implementation of hardware TWI/I2C functions and macros.
 * 
 * This file contains the definitions of function implementations and macros 
 * for hardware-based TWI (I2C) communication on AVR-0/1/2-Series microcontrollers.
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
 * @see https://github.com/0x007e/avr0 "AVR ATmega GitHub Repository"
 */

#include "twi.h"

/**
 * @brief Initialize the TWI (I2C) hardware interface in master mode.
 *
 * @return Returns the current TWI status byte after initialization.
 *
 * @details
 * This function configures the `TWI` hardware registers to initialize the bus exclusively in master mode.
 * 
 * It sets the bitrate and activates the TWI controller using the predefined macros.
 * 
 * @note If interrupt processing is enabled (`TWI_TWIE`), relevant interrupts are configured.
 * 
 * The function returns the initial TWI status for further status checking after initialization.
 */
unsigned char twi_init(void)
{
	PORTMUX.TWISPIROUTEA = TWI_PORTMUX;
	
	TWI0.MBAUD =  TWI_BITRATE;
	TWI0.MCTRLA = TWI_ENABLE_bm
		#ifdef TWI_TWIE
			| TWI_RIEN_bm
			| TWI_WIEN_bm
		#endif
			| TWI_TIMEOUT;
	
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;

    // Return message initialization
    return twi_status();
}

/**
 * @brief Disable the TWI (I2C) interface.
 *
 * @details
 * This inline function disables the hardware `TWI` module. This effectively releases control of the TWI lines and sets the pins to reset state. Use this function to safely deactivate the TWI bus when it is no longer needed.
 */
inline void twi_disable(void)
{
    TWI0.MCTRLA = 0x00;
    PORTMUX.TWISPIROUTEA = PORTMUX_TWI0_DEFAULT_gc;
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
    return TWI0.MSTATUS;
}

#ifndef TWI_TWIE
	static TWI_Error twi_bus_state(void)
	{
		unsigned char status = twi_status();
		
		if(status & (TWI_WIF_bm | TWI_RIF_bm))
		{
			if(!(status & TWI_RXACK_bm))
			{
				return TWI_None;
			}
			return TWI_Ack;
		}
		else if(status & TWI_ARBLOST_bm)
		{
			return TWI_Arbitration;
		}
		else if(status & TWI_BUSERR_bm)
		{
			return TWI_General;
		}
		return TWI_Run;
	}

    /**
	 * @brief Initiates a TWI (I2C) (repeated) start condition on the bus.
	 *
	 * @deprecated This function is obsolete on newer AVR microcontrollers and is retained only for backward compatibility.
	 * 
	 * > It does not perform any operation.
	 *
	 * @return Always returns `TWI_None` as the function is effectively a no-op.
	 *
	 * @details
	 * On modern AVR hardware, start conditions are managed internally by the TWI module, so this function is no longer necessary and does not initiate any bus activity.
	 */
    inline TWI_Error twi_start(void)
    {
        return TWI_None;       // (Repeated)Start failure
    }

    /**
	 * @brief Stop the TWI (I2C) transmission by generating a stop condition.
	 *
	 * @details
	 * This function initiates the `TWI` stop condition on the bus, signaling the end of the current communication frame. It sets the appropriate bits in the `MCTRLB` register to transmit the stop signal.
	 *
	 * @note The function waits until the stop condition has been executed by checking the `BUSSTATE` bits in the `MSTATUS` register. After this, the bus returns to the idle state, ready for the next communication cycle.
	 */
    void twi_stop(void)
    {
	    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		
	    while ((TWI0.MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_IDLE_gc);
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
     */
    inline TWI_Error twi_address(unsigned char address, TWI_Operation operation)
    {
        TWI0.MADDR = (address<<1) | operation;

        TWI_Error temp;

        do
        {
	        temp = twi_bus_state();
        } while (twi_bus_state() == TWI_Run);

        return temp;
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
	 * This function writes the provided data byte into the `TWI` data register.
	 * It then waits for the transmission to complete by repeatedly checking the TWI status.
	 *
	 * The function returns the final TWI status code indicating the result of the transmission.
	 */
    TWI_Error twi_set(unsigned char data)
    {
        TWI0.MDATA = data;
        
        TWI_Error temp;

        do
        {
	        temp = twi_bus_state();
        } while (twi_bus_state() == TWI_Run);

        return temp;
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
	 * - `TWI_General`: A general error occurred during transmission.
	 *
	 * @details
	 * This function configures the `TWI` hardware to receive a byte from the bus and decides whether to send an `ACK` or `NACK` based on the `acknowledge` parameter.
	 * 
	 * @note The function waits until the reception is complete by polling the TWI status function for completion. The received data is then read from the `MDATA` register and stored at the provided pointer.
	 * 
	 * Depending on the acknowledge mode, the function sets the `MCTRLB` register to either send an ACK and prepare for more data reception or send a NACK and stop the communication.
	 * 
	 * Finally, the function returns the status code from the reception process.
	 */
    TWI_Error twi_get(unsigned char *data, TWI_Acknowledge acknowledge)
    {
        TWI_Error temp;

        do
        {
	        temp = twi_bus_state();
        } while (twi_bus_state() == TWI_Run);

        if(temp == TWI_None)
        {
	        *data = TWI0.MDATA;
	        
	        if (acknowledge == TWI_NACK)
	        {
		        TWI0.MCTRLB = TWI_ACKACT_bm;
	        }
	        else
	        {
		        TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;
	        }
        }
        return temp;
    }
#endif