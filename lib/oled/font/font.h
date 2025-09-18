/**
 * @file font.h
 * @brief Header file for font handling and character bitmap retrieval.
 * 
 * This file defines macros for font dimensions, ASCII character range, and configuration options such as storing fonts in EEPROM or ignoring lower case characters. It provides the prototype for `font_getchararray`, a function that returns the bitmap data of a given ASCII character. The font data supports rendering text on graphical displays like OLED by converting characters into pixel patterns. Configuration macros allow customization of font size and storage.
 * 
 * @author g.raf
 * @date 2025-09-03
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the GPLv3 License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger project and subject to the license specified in the repository. For updates and the complete revision history, see the GitHub repository.
 * 
 * @see https://github.com/0x007e/oled "AVR ATmega GitHub Repository"
 */

#ifndef FONT_H_
#define FONT_H_
	
	#ifndef FONT_WIDTH
		/**
		 * @def FONT_WIDTH
		 * @brief Defines the width in pixels of each character in the font.
		 * 
		 * @details
		 * This macro sets the horizontal size of a single character glyph in pixels. It is used to calculate spacing and positioning when rendering text on graphical displays.
		 */
		#define FONT_WIDTH 5
	#endif
	
	#ifndef FONT_HEIGHT
		/**
		 * @def FONT_HEIGHT
		 * @brief Defines the height in pixels of each character in the font.
		 * 
		 * @details
		 * This macro sets the vertical size of a single character glyph in pixels. It is used to define the font height for rendering text on graphical displays.
		 */
		#define FONT_HEIGHT 7
	#endif
	
	#ifndef FONT_ASCII_START_CHAR
		/**
		 * @def FONT_ASCII_START_CHAR
		 * @brief Specifies the starting ASCII character for the font.
		 * 
		 * @details
		 * This macro defines the first ASCII character that the font supports or displays. It is used to set the beginning character code when mapping font glyphs.
		 */
		#define FONT_ASCII_START_CHAR ' '
	#endif
	
	#ifndef FONT_ASCII_END_CHAR
		/**
		 * @def FONT_ASCII_END_CHAR
		 * @brief Specifies the ending ASCII character for the font.
		 * 
		 * @details
		 * This macro defines the last ASCII character that the font supports or displays. It is used to set the ending character code when mapping font glyphs.
		 */
		#define FONT_ASCII_END_CHAR '~'
	#endif
	
	#ifndef FONT_IGNORE_LOWER_CASE
		/**
		 * @def FONT_IGNORE_LOWER_CASE
		 * @brief Converts all lowercase letters to uppercase to save memory in PROGMEM or EEPROM.
		 * 
		 * @details
		 * When this macro is defined, only uppercase letters are displayed by converting any lowercase letters to their uppercase equivalents. This helps reduce memory usage in program memory (PROGMEM) or EEPROM by avoiding the storage of separate lowercase glyphs.
		 * 
		 * @note Defining this macro enables the ignoring of lowercase characters entirely.
		 */
		// #define FONT_IGNORE_LOWER_CASE

		#ifdef _DOXYGEN_    // Used for documentation, can be ignored
            #define FONT_IGNORE_LOWER_CASE
        #endif
	#endif
	
	// Saves the fonts in eeprom
	// All ASCII characters need 470 Bytes of EEPROM
	// ASCII characters (without lower case) need 340 Bytes of EEPROM
	#ifndef FONT_STORE_IN_EEPROM
		/**
		 * @def FONT_STORE_IN_EEPROM
		 * @brief Stores the font data in EEPROM memory.
		 * 
		 * @details
		 * When this macro is defined, font data is saved in the EEPROM memory instead of PROGMEM,
		 * which helps reduce PROGMEM usage. The amount of EEPROM required depends on the character set:
		 * - All ASCII characters require approximately 470 bytes of EEPROM.
		 * - ASCII characters without lowercase letters require approximately 340 bytes of EEPROM.
		 * 
		 * @note Using EEPROM for font storage extends PROGRAM availability but be mindful of EEPROM write limits.
		 */
		// #define FONT_STORE_IN_EEPROM
        
		#ifdef _DOXYGEN_    // Used for documentation, can be ignored
            #define FONT_STORE_IN_EEPROM
        #endif
	#endif
	
	#include <ctype.h>
    #include <avr/io.h>
	
	#ifdef FONT_STORE_IN_EEPROM
		#include <avr/eeprom.h>
	#else
		#include <avr/pgmspace.h>
	#endif
	
    unsigned char* font_getchararray(const char character);

#endif /* FONT_H_ */