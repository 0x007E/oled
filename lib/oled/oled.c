/**
 * @file oled.c
 * @brief Source file with implementation of OLED display driver functions.
 * 
 * This file contains the initialization and control functions for the OLED display using `TWI` (I2C) communication, supporting both software and hardware TWI interfaces. It includes functions for sending commands and data to the display, positioning, scrolling, clearing, and updating display memory pages and segments.
 * 
 * @note The OLED initialization routine commands are stored either in EEPROM or program memory,
 * controlled by the `OLED_INIT_ROUTINE_IN_EEPROM` macro.
 *
 * Communication with the OLED is abstracted through macros that select software or hardware
 * TWI implementations based on compile-time configuration.
 * 
 * @author g.raf
 * @date 2025-09-03
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the MIT License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger project and subject to the license specified in the repository.
 * For updates and the complete revision history, see the GitHub repository.
 * 
 * @see https://github.com/0x007e/oled "AVR ATmega GitHub Repository"
 */

#include "oled.h"

#ifdef OLED_INIT_ROUTINE_IN_EEPROM
const unsigned char oled_init_commands[] EEMEM =
#else
const unsigned char oled_init_commands[] PROGMEM =
#endif
{
	(OLED_CMD_SET_DISPLAY | OLED_CMD_SET_DISPLAY_OFF),
	OLED_CMD_SET_MULTIPLEX_RATIO,
	OLED_CMD_SET_MULTIPLEX_RATIO_64_MUX,
	OLED_CMD_SET_DISPLAY_OFFSET,
	OLED_CMD_SET_DISPLAY_OFFSET_VALUE,
	OLED_CMD_SET_DISPLAY_START_LINE,
	OLED_CMD_SET_SEGMENT_REMAP,
	OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION,
	OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION,
	OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_SETUP,
	OLED_CMD_SET_CONTRAST,
	OLED_CMD_SET_CONTRAST_VALUE,
	OLED_CMD_ENTIRE_DISPLAY_ON,
	OLED_CMD_SET_DISPLAY_MODE,
	OLED_CMD_SET_DISPLAY_CLOCK,
	(OLED_CMD_SET_DISPLAY_CLOCK_FREQUENCY | OLED_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO),
	OLED_CMD_SET_MEMORY_ADDRESSING_MODE,
	OLED_CMD_SET_MEMORY_ADDRESSING_MODE_PAGE,
	OLED_CMD_SET_CHARGE_PUMP,
	OLED_CMD_SET_CHARGE_PUMP_INTERNAL,
	(OLED_CMD_SET_DISPLAY | OLED_CMD_SET_DISPLAY_ON)
};

static void oled_command(unsigned char command)
{
    #ifdef OLED_USE_SOFT_TWI
        twi_soft_set(OLED_CONTROL_COMMAND);
        twi_soft_set(command);
    #else
        twi_set(OLED_CONTROL_COMMAND);
        twi_set(command);
    #endif
}

/**
 * @brief Initializes the OLED display.
 * 
 * @details
 * This function initializes the `TWI` (I2C) communication interface to the OLED display, supports both software and hardware TWI based on compile-time configuration. It then sends a predefined sequence of initialization commands stored either in EEPROM or program memory to configure the OLED hardware registers and settings. After sending the initialization commands, the communication is ended with a stop condition, and the display is cleared to prepare it for use.
 * 
 * @note This function must be called once during system startup before any display operations.
 */
void oled_init(void)
{
    #ifdef OLED_USE_SOFT_TWI
        twi_soft_init();
    #else
        twi_init();
    #endif
	
	OLED_START(TWI_Write);
	
	// Send OLED initializing sequence
	for (unsigned char i=0; i < sizeof(oled_init_commands)/sizeof(oled_init_commands[0]); i++)
	{
		#ifdef OLED_INIT_ROUTINE_IN_EEPROM
		    oled_command(eeprom_read_byte(&oled_init_commands[i]));
		#else
		    oled_command(pgm_read_byte(&oled_init_commands[i]));
		#endif
	}
	OLED_STOP();
	
	oled_clear();
}

/**
 * @brief Disables the TWI interface used for OLED communication.
 * 
 * @details
 * This inline function disables the `TWI` (I2C) peripheral or software interface used to communicate with the OLED display, depending on whether software or hardware TWI is enabled. It is used to safely release TWI resources when OLED communication is no longer required, helping to reduce power consumption or prevent bus contention.
 */
inline void oled_disable(void)
{
    #ifdef OLED_USE_SOFT_TWI
	    twi_soft_disable();
    #else
        twi_disable();
    #endif
}

unsigned char column_current_position, page_current_position;

/**
 * @brief Sets the current write position on the OLED display.
 * 
 * @param column The column index (horizontal position) to set, must be less than `OLED_COLUMN_SIZE`.
 * @param page The page index to set, must be less than `OLED_ROW_SIZE/OLED_PAGE_SIZE`.
 * 
 * @details
 * This function updates the internal tracking variables `column_current_position` and `page_current_position` to the specified values and sends the corresponding commands to the OLED controller to set the memory addressing position for subsequent pixel data writes. It sends commands to set the page start address and the lower and higher nibbles of the column start address. Communication is done over the TWI interface in write mode.
 * 
 * @note If the requested position exceeds display bounds, the function does nothing.
 */
void oled_position(unsigned char column, unsigned char page)
{
	if(column < OLED_COLUMN_SIZE && page < OLED_PAGE_SIZE)
	{
		column_current_position = column;
		page_current_position = page;
		
		OLED_START(TWI_Write);
		
		oled_command(OLED_CMD_SET_PAGE_START_ADDRESS | (0x07 & page));
		oled_command(OLED_CMD_SET_LOWER_START_COLUMN_ADDRESS | (0x0F & column));
		oled_command(OLED_CMD_SET_HIGHER_START_COLUMN_ADDRESS | (0x0F & (column>>4)));
		
		OLED_STOP();
	}
}

/**
 * @brief Scrolls the OLED display vertically by a specified offset.
 * 
 * @param scroll The number of pixel rows to offset the display vertically. Must be less than `OLED_ROW_SIZE` to be valid.
 * 
 * @details
 * This function adjusts the vertical display offset by sending the appropriate command and data to the OLED controller via TWI interface. It effectively shifts the visible area of the display vertically by the specified number of pixel rows, creating a scrolling effect.
 * 
 * @note If the scroll value is equal to or exceeds the maximum row size, the function does nothing.
 */
void oled_scroll_vertical(unsigned char scroll)
{
	if(scroll >= OLED_ROW_SIZE)
	{
		return;
	}
	
	OLED_START(TWI_Write);
	oled_command(OLED_CMD_SET_DISPLAY_OFFSET);
	oled_command(scroll);
	OLED_STOP();
}

/**
 * @brief Displays a full frame (all pages) of pixel data on the OLED.
 * 
 * @param frame Pointer to an array containing pixel data for the entire display, organized as consecutive pages.
 * 
 * @details
 * This function updates the entire OLED screen by sequentially writing data page by page. It first resets the current cursor position to the home position (0,0), then iterates over all pages, calling `oled_page` to write each page’s data.
 * 
 * @note The `frame` parameter should contain `OLED_PAGE_SIZE` pages of pixel data, where each page holds pixel data for the full width of the display.
 */
void oled_frame(unsigned char *frame)
{
	OLED_HOME();
	
	for (unsigned char page=0; page < OLED_PAGE_SIZE; page++)
	{
		oled_page((frame + page * OLED_PAGE_SIZE), page);
	}
}

/**
 * @brief Writes a full page of pixel data to the OLED display.
 * 
 * @param data Pointer to an array containing pixel data for one display page.
 * @param page The page index (0-based) to which the data should be written. Must be less than `OLED_ROW_SIZE/OLED_PAGE_SIZE`.
 * 
 * @details
 * This function writes pixel data to a complete page (horizontal row of 8 pixel rows) on the OLED display. It performs boundary checking on the page parameter; if the page is invalid, the function returns without action.
 * 
 * @note Internally, it calls `oled_page_segment` to update the entire width of the display on the specified page, passing the full column range from 0 to `OLED_COLUMN_SIZE - 1`.
 */
void oled_page(unsigned char *data, unsigned char page)
{
	if(page >= OLED_PAGE_SIZE)
	{
		return;
	}
	oled_page_segment(data, 0, (OLED_COLUMN_SIZE - 1), page);
}

/**
 * @brief Writes a segment of pixel data to a specific page on the OLED display.
 * 
 * @param data Pointer to an array containing pixel data for the segment to write.
 * @param column_start The starting column index of the segment (inclusive).
 * @param column_stop The ending column index of the segment (inclusive).
 * @param page The page index (0-based) where the data should be written. Must be less than `OLED_ROW_SIZE/OLED_PAGE_SIZE`.
 * 
 * @details
 * This function writes a horizontal segment of pixel data on a specified page of the OLED display. It first validates the input parameters to ensure the page and column ranges are within the display bounds, and that the column start is less than the column stop.
 * 
 * The display cursor is set to the starting column and page before data transmission. Data bytes are then sent sequentially to the OLED over the TWI interface, using either software or hardware TWI accordingly. Communication begins with a start condition and control byte indicating data transmission, and ends with a stop condition.
 */
void oled_page_segment(unsigned char *data, unsigned char column_start, unsigned char column_stop, unsigned char page)
{
	if(page >= OLED_PAGE_SIZE || column_stop >= OLED_COLUMN_SIZE || column_start >= column_stop)
	{
		return;
	}
	
	oled_position(column_start, page);
	
	OLED_START(TWI_Write);
	OLED_DATA();
	
	for (unsigned char i=column_start; i <= column_stop; i++)
	{
        #ifdef OLED_USE_SOFT_TWI
            twi_soft_set(*(data++));
        #else
		    twi_set(*(data++));
        #endif
	}
	
	OLED_STOP();
}

/**
 * @brief Writes a single byte of pixel data to a specific column and page on the OLED display.
 * 
 * @param data The byte of pixel data to write; each bit represents a pixel in the vertical column.
 * @param column The column index (horizontal position) where the data should be written.
 * @param page The page index where the data should be written.
 * 
 * @note Both `column` and `page` must be within the display bounds (`OLED_COLUMN_SIZE` and `OLED_ROW_SIZE/OLED_PAGE_SIZE` respectively).
 * 
 * @details
 * This function updates the pixel data of one specific column within a specified page. It first verifies that the target page and column are valid. Then, it sets the OLED cursor to the specified position and sends the pixel data byte via TWI. Communication begins with a start condition and a control byte indicating data transmission, followed by the data byte, and ends with a stop condition.
 */
void oled_column(unsigned char data, unsigned char column, unsigned char page)
{
	if(page >= OLED_PAGE_SIZE || column >= OLED_COLUMN_SIZE)
	{
		return;
	}
	
	oled_position(column, page);
	
	OLED_START(TWI_Write);
	OLED_DATA();
        #ifdef OLED_USE_SOFT_TWI
            twi_soft_set(data);
        #else
            twi_set(data);
        #endif
	OLED_STOP();
}

/**
 * @brief Clears the entire OLED display.
 * 
 * @details
 * This function iterates through all display pages and clears each one by calling `oled_clear_page`. After clearing all pages, it resets the cursor position to the home position (0,0) using `OLED_HOME`.
 * 
 * @note This effectively resets the display content, turning off all pixels.
 */
void oled_clear(void)
{
	for (unsigned char page=0; page < OLED_PAGE_SIZE; page++)
	{
		oled_clear_page(page);
	}
	OLED_HOME();
}

/**
 * @brief Clears a single page of the OLED display.
 * 
 * @param page The page index (0-based) to clear. Must be less than `OLED_ROW_SIZE/OLED_PAGE_SIZE`.
 * 
 * @details
 * This function clears all columns within the specified page by calling `oled_clear_page_segment` with the full column range from 0 to `OLED_COLUMN_SIZE - 1`.
 * 
 * @note It turns off all pixels on that page, effectively wiping the displayed content in that segment.
 */
void oled_clear_page(unsigned char page)
{
	oled_clear_page_segment(0, (OLED_COLUMN_SIZE - 1), page);
}

/**
 * @brief Clears a specific segment of a page on the OLED display.
 * 
 * @param column_start The starting column index of the segment to clear (inclusive).
 * @param column_stop The ending column index of the segment to clear (inclusive).
 * @param page The page index (0-based) on which the segment is located. Must be less than `OLED_ROW_SIZE/OLED_PAGE_SIZE`.
 * 
 * @details
 * This function clears a horizontal segment of a specified page by writing zeroes (turning off pixels) to the display RAM across the specified column range. It first validates the input parameters to ensure they are within the display boundaries and that `column_start` is less than or equal to `column_stop`. After setting the cursor position to the start column of the specified page, it sends a series of zero bytes over the TWI interface to clear each column in the segment. Communication starts with a start condition and a control byte indicating data transmission, and ends with a stop condition.
 */
void oled_clear_page_segment(unsigned char column_start, unsigned char column_stop, unsigned char page)
{
    if(page >= OLED_PAGE_SIZE || column_stop >= OLED_COLUMN_SIZE || column_start >= column_stop)
    {
        return;
    }

	oled_position(column_start, page);
	
	OLED_START(TWI_Write);
	OLED_DATA();
	
	for (unsigned char i=0; i <= column_stop; i++)
	{
        #ifdef OLED_USE_SOFT_TWI
            twi_soft_set(0x00);
        #else
            twi_set(0x00);
        #endif
	}
	
	OLED_STOP();
}

/**
 * @brief Clears a single column within a specified page on the OLED display.
 * 
 * @param column The column index (horizontal position) to clear.
 * @param page The page index (vertical position in groups of 8 pixel rows) where the column resides.
 * 
 * @details
 * This function sets the cursor to the specified column and page, then writes a single zero byte to turn off all pixels in that column for the specified page. Communication with the display is done via TWI, using start and stop conditions, and the data control byte.
 * 
 * @note Clearing a column effectively resets the pixels in that vertical segment to off.
 */
void oled_clear_column(unsigned char column, unsigned char page)
{
	oled_position(column, page);
	
	OLED_START(TWI_Write);
	OLED_DATA();
	
	#ifdef OLED_USE_SOFT_TWI
	    twi_soft_set(0x00);
	#else
	    twi_set(0x00);
	#endif
	
	OLED_STOP();
}