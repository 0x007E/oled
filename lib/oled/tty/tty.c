/**
 * @file tty.c
 * @brief Source file with implementation of TTY text output interface on OLED display.
 * 
 * This file contains the implementation of functions for text rendering on a graphical OLED display. It supports cursor positioning, character and string output, line clearing, and optional printf-style formatted output. The interface handles autoscrolling and dynamic character spacing for improved readability. The TTY layer relies on the OLED display driver and font data modules. It abstracts text rendering as characters drawn via pixel data segments using the configured font, respecting display size and constraints. Compile-time options toggle support for autoscroll, dynamic text spacing, and printf functionality.
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

#include "tty.h"

#ifdef TTY_ENABLE_PRINTF
	static FILE std_tty = FDEV_SETUP_STREAM(tty_printf, NULL, _FDEV_SETUP_WRITE);
#endif

static unsigned char tty_character;
static unsigned char tty_line;

#ifdef TTY_AUTOSCROLL
    static unsigned char tty_scroll;
#endif

#ifdef TTY_DYNAMIC_TEXT
    static unsigned char tty_dynamic_text_rstrip;
#endif

/**
 * @brief Initializes the TTY text output interface.
 * 
 * @details
 * This function initializes internal state variables for cursor position and scrolling, resets dynamic text spacing if enabled, and initializes the underlying OLED display. It clears the OLED screen to prepare for new text output. If printf support is enabled, it also configures the standard output stream `stdout` to use the TTY's printf handler function, allowing formatted output via standard C library functions.
 * 
 * @note This function should be called once at system startup before any TTY text output operations.
 */
void tty_init(void)
{
	tty_character = 0;
	tty_line = 0;

    #ifdef TTY_AUTOSCROLL
        tty_scroll = 0;
    #endif
    
    #ifdef TTY_DYNAMIC_TEXT
        tty_dynamic_text_rstrip = 0;
    #endif
	
	oled_init();
	oled_clear();
	
	#ifdef TTY_ENABLE_PRINTF
		stdout = &std_tty;
	#endif
}

static void tty_newline()
{
	tty_character = 0;
	tty_line++;
	
	if(tty_line >= 8)
	{
		tty_line = 0;
		tty_scroll = 1;
	}
	
	#ifdef TTY_AUTOSCROLL
		
		unsigned char temp = (tty_line + 1);
		
		if(temp >= 8)
		{
			temp = 0;
		}
		
		if(tty_scroll)
		{
			oled_scroll_vertical((TTY_HEIGHT * temp));
		}
	#endif
	
    #ifdef TTY_DYNAMIC_TEXT
        tty_dynamic_text_rstrip = 0;
    #endif

	tty_clear_line(tty_line);
}

/**
 * @brief Clears a specific text line on the TTY interface.
 * 
 * @param line The line number (0-based) to clear. Must be less than `TTY_HEIGHT`.
 * 
 * @details
 * This function checks if the specified line is within the valid range of the display. If it is, the function clears the corresponding page on the OLED display associated with that text line.
 * 
 * @note Clearing effectively removes all characters and pixels on that line, preparing it for new content.
 */
void tty_clear_line(unsigned char line)
{
	if(line >= TTY_HEIGHT)
	{
		return;
	}
	oled_clear_page(line);
}

/**
 * @brief Sets the cursor position in the TTY text interface.
 * 
 * @param column The column index (0-based) to move the cursor to. Must be less than `TTY_WIDTH`.
 * @param line The line index (0-based) to move the cursor to. Must be less than `TTY_HEIGHT`.
 * 
 * @details
 * This function validates the requested cursor position to ensure it's within the display bounds. If valid, it updates the internal cursor tracking variables for character column and line. It then sets the corresponding pixel position on the OLED display by converting the column to pixel coordinates based on the font width and updating the OLED cursor to that position.
 */
void tty_cursor(unsigned char column, unsigned char line)
{
	if((column >= TTY_WIDTH) || (line >= TTY_HEIGHT))
	{
		return;
	}
	tty_character = column;
	tty_line = line;
	
	oled_position(column * FONT_WIDTH, line);
}

/**
 * @brief Writes a single character to the TTY text interface on the OLED display.
 * 
 * @param character The ASCII character to display.
 * @return Returns 0 on success or ignored character.
 * 
 * @details
 * This function validates the input character against the supported font ASCII range, ignoring characters outside this range except newline (`'\n'`). If the character is a newline, it triggers a line break via `tty_newline()`. For printable characters, it retrieves the pixel data bitmap from the font module. When dynamic text spacing (`TTY_DYNAMIC_TEXT`) is enabled, the function adjusts horizontal spacing to minimize large gaps between characters by manipulating `tty_dynamic_text_rstrip`. It draws the character on the current line and position using `oled_page_segment`. When dynamic spacing is disabled, fixed character width spacing is used.
 * 
 * @note After writing the character, the cursor advances horizontally. If the end of the line is reached, the function automatically inserts a newline to move to the next line.
 */
char tty_putchar(char character)
{
	if(	character != '\n' &&
		character < FONT_ASCII_START_CHAR &&
		character > FONT_ASCII_END_CHAR)
	{
		return 0;
	}
	else if(character == '\n')
	{
		tty_newline();
		return 0;
	}
	
	unsigned char *temp;
	
	temp = font_getchararray(character);

    #ifdef TTY_DYNAMIC_TEXT
        if((tty_dynamic_text_rstrip > 0) && (*(temp) == 0x00))
        {
            tty_dynamic_text_rstrip--;
        }

        oled_page_segment(temp, (tty_dynamic_text_rstrip + tty_character * FONT_WIDTH), (tty_dynamic_text_rstrip + (tty_character + 1) * FONT_WIDTH), tty_line);

        if(*(temp + FONT_WIDTH - 1) != 0x00)
        {
            tty_dynamic_text_rstrip++;
        }
    #else
        oled_page_segment(temp, (tty_character * FONT_WIDTH), ((tty_character + 1) * FONT_WIDTH), tty_line);
    #endif

	tty_character++;
	
    #ifdef TTY_DYNAMIC_TEXT
        if(tty_character >= ((TTY_WIDTH * FONT_WIDTH - tty_dynamic_text_rstrip)/FONT_WIDTH))
    #else
        if(tty_character >= TTY_WIDTH)
    #endif
	{
		tty_newline();
	}
	return 0;
}

/**
 * @brief Writes a null-terminated string to the TTY text interface.
 * 
 * @param string Pointer to the null-terminated character array (C-string) to display.
 * 
 * @details
 * This function iterates through each character of the input string until it encounters the string terminator (`'\0'`). Each character is passed to `tty_putchar` for rendering on the OLED display, thereby displaying the entire string sequentially.
 */
void tty_string(const char *string)
{
	while(*string != '\0')
	{
		tty_putchar(*(string++));
	}
}

#ifdef TTY_ENABLE_PRINTF
	/**
	 * @brief Writes a single character to the TTY interface (used as a printf output function).
	 * 
	 * @param data The character to output.
	 * @param stream Pointer to the output stream (unused, required by standard library interface).
	 * @return Returns the result of `tty_putchar`, typically 0.
	 * 
	 * @details
	 * This function acts as a character output handler compatible with the standard C library `printf` mechanism. It redirects characters from formatted output to the TTY text rendering function `tty_putchar`.
	 * 
	 * @note This enables using `printf`-style functions to display text on the OLED via the TTY interface.
	 */
	int tty_printf(char data, FILE *stream)
	{
		return tty_putchar(data);
	}
#endif






