/**
 * @file frame.h
 * @brief Header file for frame handling on OLED display.
 * 
 * This file provides macro definitions and function prototypes for managing graphical frames on an OLED display. It defines default frame dimensions based on the OLED's column and row size, allows enabling a specific background, and supports different numeric display operations including decimal, signed, and unsigned number rendering. The interface depends on included font, OLED driver, number datatype, radix, and drawing element modules. Configuration macros control CPU frequency, frame size, background support, and numeric formatting options. The provided functions initialize the frame system, draw text and numbers at specified positions, draw progress bars, and clear the display frame.
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

#ifndef FRAME_H_
#define FRAME_H_

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
	
	#ifndef FRAME_WIDTH
		/**
		 * @def FRAME_WIDTH
		 * @brief Defines the width of the display frame in pixels.
		 * 
		 * @details
		 * This macro sets the horizontal size (width) of the display frame used for rendering graphics or text. If not already defined, it defaults to the OLED display's column size (`OLED_COLUMN_SIZE`). This allows flexibility for different display sizes by adjusting the macro at compile time.
		 * 
		 * @note `FRAME_WIDTH` is typically configured to match the physical dimensions of the OLED display.
		 */
		#define FRAME_WIDTH OLED_COLUMN_SIZE
	#endif
	
	#ifndef FRAME_HEIGHT
		/**
		 * @def FRAME_HEIGHT
		 * @brief Defines the height of the display frame in pixels.
		 * 
		 * @details
		 * This macro sets the vertical size (height) of the display frame used for rendering graphics or text. If not already defined, it defaults to the OLED display's row size (`OLED_ROW_SIZE`). This allows adjustment of frame height to match different OLED display dimensions at compile time.
		 * 
		 * @note `FRAME_HEIGHT` should be set to correspond to the physical number of pixel rows of the OLED.
		 */
		#define FRAME_HEIGHT OLED_ROW_SIZE
	#endif
	
	#ifndef FRAME_SPECIFIC_BACKGROUND
		/**
		 * @def FRAME_SPECIFIC_BACKGROUND
		 * @brief Enables the use of a custom background stored in a framebuffer array in PROGMEM.
		 * 
		 * @details
		 * When this macro is defined, a specific background image stored in PROGMEM (`frame_background` in `frame.c`) is drawn to the OLED display. The background serves as a persistent image or pattern that is constantly used as a base layer,  and during data output or screen updates, the display content is continuously reconciled against this background to maintain visual consistency.
		 * 
		 * The custom background can be created or edited using the tool available at [https://0x007e.github.io/oled/](https://0x007e.github.io/oled/), allowing flexible design of frame backgrounds.
		 *
		 * @note Defining this macro activates background drawing and reconciliation with display content and reserves PROGMEM space.
		 */
		#define FRAME_SPECIFIC_BACKGROUND
	#endif

	#ifndef FRAME_NUMBER_ENABLE_DECIMAL
		/**
		 * @def FRAME_NUMBER_ENABLE_DECIMAL
		 * @brief Enables support for decimal (floating-point) numbers in frame rendering.
		 * 
		 * @details
		 * Defining this macro enables the display and formatting of decimal numbers (numbers with fractional parts) within the frame output functions.
		 * 
		 * @note Enabling decimal number support increases program memory usage due to added formatting and processing code. This tradeoff should be considered when working with memory-constrained embedded systems. If memory usage is critical and decimal precision is not required, refrain from defining this macro.
		 */
		//#define FRAME_NUMBER_ENABLE_DECIMAL

        #ifdef _DOXYGEN_    // Used for documentation, can be ignored
            #define FRAME_NUMBER_ENABLE_DECIMAL
        #endif

		#ifndef FRAME_NUMBER_PRECISION
			/**
			 * @def FRAME_NUMBER_PRECISION
			 * @brief Sets the number of decimal places to display for floating-point numbers.
			 * 
			 * @details
			 * This macro defines the precision used when rendering decimal (floating-point) numbers on the display. It specifies how many digits should appear after the decimal point. The default value is 2 decimal places if not explicitly defined.
			 * 
			 * @note Increasing the precision allows for more detailed numbers, but may increase program memory usage and processing time.
			 */
			#define FRAME_NUMBER_PRECISION 2
		#endif
	#endif
	
	#include <stdlib.h>
	#include <string.h>
    #include <avr/io.h>
    #include <avr/pgmspace.h>
	#include <util/delay.h>

	#include "../oled.h"
    #include "../font/font.h"
    #include "../../common/numbers/datatypes.h"
    #include "../../common/numbers/radix.h"
	#include "../../common/drawing/elements.h"

    /**
     * @enum FRAME_Number_Operation_t
     * @brief Specifies the numeric operation modes for number rendering.
     *
     * @details
     * This enumeration defines how numbers should be interpreted and processed when rendering formatted output on the display. It distinguishes between decimal operations (typically for floating-point numbers), signed operations, and unsigned operations.
     */
    enum FRAME_Number_Operation_t
    {
        FRAME_Number_Operation_Decimal = 0,     /**< Decimal operation mode for floating-point numbers */
        FRAME_Number_Operation_Signed,          /**< Signed operation mode */
        FRAME_Number_Operation_Unsigned         /**< Unsigned operation mode */
    };

    /**
     * @typedef FRAME_Number_Operation
     * @brief Alias for enum FRAME_Number_Operation_t representing numeric operation modes.
     */
    typedef enum FRAME_Number_Operation_t FRAME_Number_Operation;

    void frame_init(void);
	void frame_draw_text(const char *text, DRAWING_Position position);
	void frame_draw_number(const void* number, unsigned char length, NUMBER_Type type, NUMBER_Radix radix, DRAWING_Position position);
	void frame_draw_number_uint(const unsigned int number, unsigned char length, NUMBER_Radix radix, DRAWING_Position position);
	void frame_draw_number_int(const int number, unsigned char length, NUMBER_Radix radix, DRAWING_Position position);
	void frame_draw_bar(DRAWING_Position position, DRAWING_Size size, unsigned char percent);
	void frame_clear(void);

#endif /* FRAME_H_ */