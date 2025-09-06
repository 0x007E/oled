/**
 * @file tty.h
 * @brief Header file for TTY interface - text output on OLED display.
 * 
 * This file provides function prototypes, macro definitions, and constants for text output on a graphical OLED display. It supports cursor control, text rendering, line clearing, and optional printf functionality. The TTY interface abstracts text handling by using font data and display parameters defined elsewhere and adapts to platform-specific settings. Configuration macros control buffer dimensions, autoscrolling behavior, and printf support. The interface depends on the OLED driver and font data provided in separate modules.
 * 
 * @author g.raf
 * @date 2025-09-03
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the MIT License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger project and subject to the license specified in the repository. For updates and the complete revision history, see the GitHub repository.
 * 
 * @see https://github.com/0x007e/oled "AVR ATmega GitHub Repository"
 */

#ifndef TTY_H_
#define TTY_H_

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

	#ifndef TTY_WIDTH
		/**
		 * @def TTY_WIDTH
		 * @brief Defines the number of text columns available in the TTY interface.
		 * 
		 * @details
		 * This macro calculates the width of the text display area in characters by dividing the total number of OLED display columns (`OLED_COLUMN_SIZE`) by the width of a single font character (`FONT_WIDTH`).
		 * 
		 * @note It sets the maximum number of characters that can fit horizontally on the screen.
		 * 
		 */
		#define TTY_WIDTH (OLED_COLUMN_SIZE/FONT_WIDTH)
	#endif
	
	#ifndef TTY_HEIGHT
		/**
		 * @def TTY_HEIGHT
		 * @brief Defines the number of text lines available in the TTY interface.
		 * 
		 * @details
		 * This macro calculates the height of the text display area in lines by dividing the total number of OLED pixel rows (`OLED_ROW_SIZE`) by the height of one page of the display.
		 * 
		 * @note It determines how many text lines can be displayed vertically on the screen.
		 */
		#define TTY_HEIGHT (OLED_ROW_SIZE/OLED_PAGE_SIZE)
	#endif
	
	#ifndef TTY_AUTOSCROLL
		/**
		 * @def TTY_AUTOSCROLL
		 * @brief Enables automatic scrolling of text when the cursor reaches the bottom of the display.
		 * 
		 * @details
		 * When this macro is defined, the TTY interface will automatically scroll the displayed text upward as new lines are added beyond the last visible line. This ensures that newly written text remains visible without manual intervention to clear or reposition the display.
		 * 
		 * @note If undefined, the TTY interface may either stop rendering new lines or overwrite existing content without scrolling. This macro acts as a compile-time flag to control autoscrolling behavior.
		 */
		#define TTY_AUTOSCROLL
	#endif
	
	#ifndef TTY_ENABLE_PRINTF
		/**
		 * @def TTY_ENABLE_PRINTF
		 * @brief Enables support for printf-style formatted output in the TTY interface.
		 * 
		 * @details
		 * Defining this macro includes the implementation of functions that allow using `printf`-style formatting to send formatted text strings to the OLED display via the TTY interface. This facilitates flexible and convenient text output with variable substitution and formatting.
		 * 
		 * @note If undefined, the printf functionality is excluded, reducing code size and complexity. This macro serves as a compile-time option to include or exclude printf support.
		 */
		#define TTY_ENABLE_PRINTF
	#endif

    #ifndef TTY_DYNAMIC_TEXT
		/**
		 * @def TTY_DYNAMIC_TEXT
		 * @brief Enables dynamic spacing adjustment between characters in the TTY text output.
		 * 
		 * @details
		 * When this macro is defined, the TTY interface dynamically regulates the horizontal spacing between individual characters during text rendering to minimize large gaps. This helps achieve a more visually balanced and compact text appearance on the OLED display, improving readability and aesthetics.
		 * 
		 * @note If undefined, fixed-width spacing will be used for all characters, potentially resulting in uneven gaps. This macro acts as a compile-time flag to enable or disable dynamic character spacing.
		 */
        // #define TTY_DYNAMIC_TEXT

		#ifdef _DOXYGEN_
            #define TTY_DYNAMIC_TEXT
        #endif
    #endif
	
	#include <stdio.h>
    #include <avr/io.h>
	#include <util/delay.h>
	
	#include "../oled.h"
	#include "../font/font.h"

	/**
	 * @def TTY_HOME
	 * @brief Moves the TTY cursor to the home position (top-left corner) of the text area.
	 * 
	 * @details
	 * This macro calls the function `tty_cursor` with coordinates (0, 0), which sets the text cursor to the first column and first line of the TTY interface. It is commonly used to reset the cursor position before writing new text or clearing the display.
	 */
	#define TTY_HOME() (tty_cursor(0, 0))

    void tty_init(void);
	void tty_cursor(unsigned char column, unsigned char line);
	void tty_clear_line(unsigned char line);
	
    char tty_putchar(char character);
    void tty_string(const char *string);
	
	#ifdef TTY_ENABLE_PRINTF
		int tty_printf(char data, FILE *stream);
	#endif
	
#endif /* TTY_H_ */