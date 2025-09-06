/**
 * @file oled.h
 * @brief Header file for OLED display control via hardware or software TWI/I2C.
 * 
 * This file provides function prototypes, macro definitions, and constants for  controlling a monochrome OLED display connected over TWI (I2C) interface. It supports both software and hardware TWI implementations selectable via macros. The file defines configuration parameters such as display dimensions, control bytes, timing settings, and command codes required to initialize and operate the OLED display.
 *
 * @note Functions declared here enable initializing the display, positioning the cursor, updating display content by pages or columns, clearing the display, and scrolling.
 * 
 * Macros abstract the underlying TWI communication method (software or hardware) for sending commands and data, to ensure flexible deployment depending on hardware capabilities.
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
 * @see https://github.com/0x007e/oled "AVR ATmega GitHub Repository"
 */

#ifndef OLED_H_
#define OLED_H_

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

    #ifndef OLED_USE_SOFT_TWI
		/**
		 * @def OLED_USE_SOFT_TWI
		 * @brief Enable software-based TWI (I2C) communication for the OLED display.
		 *
		 * @details
		 * Define this macro to use a software-implemented `TWI` (bit-banging) interface for communication with the OLED display. If this macro is not defined, the library defaults to using the hardware TWI (I2C) peripheral of the system. Software TWI can be useful on systems without hardware TWI support or if pin flexibility is required.
		 * 
		 * @note To disable software TWI and use hardware TWI instead, comment out or undefine this macro.
		 */
        #define OLED_USE_SOFT_TWI
    #endif

	#ifndef OLED_ADDRESS
		/**
		 * @def OLED_ADDRESS
		 * @brief I2C (TWI) slave address of the OLED display.
		 *
		 * @details
		 * This macro defines the 7-bit I2C address of the OLED display device used for communication over the `TWI` bus. The address is derived by right-shifting the 8-bit address `0x78` to conform to the 7-bit addressing scheme used by the hardware TWI interface. It is used when initiating communication with the display to ensure data is sent to the correct device on the bus.
		 */
		#define OLED_ADDRESS (0x78>>1)
	#endif

	#ifndef OLED_CONTROL_COMMAND
		/**
		 * @def OLED_CONTROL_COMMAND
		 * @brief Control byte indicating a command for the OLED display.
		 *
		 * @details
		 * This macro defines the control byte value sent before a command byte to the OLED display during `TWI` (I2C) communication. It signals to the display controller that the following byte(s) should be interpreted as commands rather than display data.
		 * 
		 * @note Proper use of this control byte is essential for configuring and controlling the OLED hardware.
		 */
		#define OLED_CONTROL_COMMAND 0x80
	#endif

	#ifndef OLED_CONTROL_DATA
		/**
		 * @def OLED_CONTROL_DATA
		 * @brief Control byte indicating data for the OLED display.
		 *
		 * @details
		 * This macro defines the control byte value sent before display data bytes to the OLED display during TWI (I2C) communication. It signals to the display controller that the following byte(s) should be treated as pixel or graphic data to be rendered on the screen.
		 * 
		 * @note Correct usage of this control byte ensures the proper update and display of graphical content.
		 */
		#define OLED_CONTROL_DATA 0x40
	#endif
	
	#ifndef OLED_PAGE_SIZE
		/**
		 * @def OLED_PAGE_SIZE
		 * @brief Number of pixels per OLED page.
		 *
		 * @details
		 * This macro defines the height of a single OLED page in pixels. The OLED display memory is organized in pages of this size, which is used for addressing and updating the display in page mode.
		 */
		#define OLED_PAGE_SIZE 8UL
	#endif
	
	#ifndef OLED_ROW_SIZE
		/**
		 * @def OLED_ROW_SIZE
		 * @brief Number of pixel rows in the OLED display.
		 *
		 * @details
		 * This macro defines the vertical resolution of the OLED display in pixels. It specifies how many rows of pixels the display has, used to calculate display memory size and addressing for rendering graphics.
		 */
		#define OLED_ROW_SIZE 64UL
	#endif
	
	#ifndef OLED_COLUMN_SIZE
		/**
		 * @def OLED_COLUMN_SIZE
		 * @brief Number of pixel columns in the OLED display.
		 *
		 * @details
		 * This macro defines the horizontal resolution of the OLED display in pixels. It specifies how many columns of pixels the display has, which is important for
		 * memory addressing and graphical rendering on the display.
		 */
		#define OLED_COLUMN_SIZE 128UL
	#endif
	
	#ifndef OLED_IDLE_TIME_US
		/**
		 * @def OLED_IDLE_TIME_US
		 * @brief Delay time in microseconds after OLED TWI/I2C stop condition.
		 *
		 * @details
		 * This macro defines the idle delay time in microseconds to wait after sending a stop condition to the OLED display over the TWI (I2C) bus. This delay ensures the display has sufficient time to process commands before the next communication starts.
		 */
		#define OLED_IDLE_TIME_US 10
	#endif

	#ifndef OLED_INIT_ROUTINE_IN_EEPROM
		/**
		 * @def OLED_INIT_ROUTINE_IN_EEPROM
		 * @brief Store OLED initialization routine commands in EEPROM.
		 *
		 * @details
		 * Defining this macro moves the OLED display initialization command sequence from program memory (flash) to EEPROM. This can help reduce flash usage and allow easier modification of initialization commands without recompiling the firmware.
		 * 
		 * @note If not defined, the initialization routine is stored in program memory by default.
		 */
		// #define OLED_INIT_ROUTINE_IN_EEPROM

		#ifdef _DOXYGEN_    // Used for documentation, can be ignored
            #define OLED_INIT_ROUTINE_IN_EEPROM
        #endif
	#endif

	/**
	 * @defgroup OLED_CMD_SET_DISPLAY Display Control Command
	 * @brief Command and options for controlling the OLED display power state.
	 * 
	 * @details
	 * The `OLED_CMD_SET_DISPLAY` macro defines the command byte used to control the display power state.
	 * The Macros:
	 * - `OLED_CMD_SET_DISPLAY_ON`
	 * - `OLED_CMD_SET_DISPLAY_OFF`
	 * specify the options to turn the display on or off, respectively. These macros are used to configure the display's operational status via I2C commands.
	 * 
	 * Usage Example:
	 * - To turn the display on, send the command `(OLED_CMD_SET_DISPLAY | OLED_CMD_SET_DISPLAY_ON)`.
	 * - To turn the display off, send the command `OLED_CMD_SET_DISPLAY` or `(OLED_CMD_SET_DISPLAY | OLED_CMD_SET_DISPLAY_OFF)` for better readability.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_DISPLAY
		#define OLED_CMD_SET_DISPLAY 0xAE
		
		/** Turn the OLED display on */
		#define OLED_CMD_SET_DISPLAY_ON 0x01

		/** Turn the OLED display off */
		#define OLED_CMD_SET_DISPLAY_OFF 0x00
	#endif
	/** @} */

	/**
	 * @defgroup OLED_CMD_SET_DISPLAY_CLOCK Display Clock Command
	 * @brief Command and options for configuring the OLED display clock timing and frequency.
	 * 
	 * @details
	 * The `OLED_CMD_SET_DISPLAY_CLOCK` macro defines the command byte used to configure the display's internal clock.
	 * 
	 * @note The options allow adjustment of the clock divide ratio and oscillator frequency, which determine the timing and refresh rate of the OLED display.
	 *
	 * Possible frequency settings:
	 * - Oscillator frequency setting bits can range from `0x00` to `0xF0` (higher nibble of the byte).
	 * - Common base oscillator frequency options include:
	 *   - 0x00: Lowest frequency
	 *   - 0x0F: Highest frequency (see display datasheet for exact mapping)
	 *
	 * Possible clock divide ratios:
	 * - Divide ratio can be set from 0x00 (divide by 1) up to 0x0F (divide by 16)
	 * - Lower divide ratios result in higher frame rates; higher divide ratios can reduce current consumption at the expense of slower updates.
	 *
	 * Macros:
	 * - `OLED_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO` (default: 0x00, divide by 1)
	 * - `OLED_CMD_SET_DISPLAY_CLOCK_FREQUENCY` (default: 0x80, typical frequency setting)
	 *
	 * Usage Example:
	 * To set the display clock, send the command `OLED_CMD_SET_DISPLAY_CLOCK` followed by the desired divide ratio and frequency setting ()`OLED_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO | OLED_CMD_SET_DISPLAY_CLOCK_FREQUENCY`).
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_DISPLAY_CLOCK
		#define OLED_CMD_SET_DISPLAY_CLOCK 0xD5
		
		/** Clock divide ratio setting */
		#define OLED_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO 0x00
		
		/** Oscillator frequency setting */
		#define OLED_CMD_SET_DISPLAY_CLOCK_FREQUENCY 0x80
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_SET_MULTIPLEX_RATIO Multiplex Ratio Command
	 * @brief Command and options for setting the OLED display multiplex ratio.
	 * 
	 * @details
	 * The `OLED_CMD_SET_MULTIPLEX_RATIO` macro defines the command byte used to configure the multiplex ratio of the OLED display.
	 * The macros:
	 * - `OLED_CMD_SET_MULTIPLEX_RATIO_16_MUX`
	 * - `OLED_CMD_SET_MULTIPLEX_RATIO_32_MUX`
	 * - `OLED_CMD_SET_MULTIPLEX_RATIO_64_MUX`
	 * specify the multiplex ratio options for `16`, `32`, and `64` multiplexed COM lines respectively.
	 * 
	 * @note These settings determine how many rows of the display are multiplexed and affect the vertical resolution and power consumption.
	 * 
	 * Usage Example:
	 * To set the multiplex ratio, send the command `OLED_CMD_SET_MULTIPLEX_RATIO` followed by one of the multiplex ratio macros corresponding to the desired configuration.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_MULTIPLEX_RATIO
		#define OLED_CMD_SET_MULTIPLEX_RATIO 0xA8
		
		/** Multiplex ratio for 16 COM lines */
		#define OLED_CMD_SET_MULTIPLEX_RATIO_16_MUX 0x1F
		
		/** Multiplex ratio for 32 COM lines */
		#define OLED_CMD_SET_MULTIPLEX_RATIO_32_MUX 0x2F
		
		/** Multiplex ratio for 64 COM lines */
		#define OLED_CMD_SET_MULTIPLEX_RATIO_64_MUX 0x3F
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_SET_DISPLAY_OFFSET Display Offset Command
	 * @brief Command and option for configuring the display offset of the OLED.
	 * 
	 * @details
	 * The `OLED_CMD_SET_DISPLAY_OFFSET` macro defines the command byte used to set the display's vertical offset. The macro `OLED_CMD_SET_DISPLAY_OFFSET_VALUE` specifies the offset value to be used, typically zero for standard alignment.
	 * 
	 * @note These macros allow adjustment of the display's vertical start line, which can be useful for scrolling or custom display layouts.
	 * 
	 * Usage Example:
	 * To set the display offset, send the command `OLED_CMD_SET_DISPLAY_OFFSET` followed by `OLED_CMD_SET_DISPLAY_OFFSET_VALUE`.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_DISPLAY_OFFSET
		#define OLED_CMD_SET_DISPLAY_OFFSET 0xD3
		
		/** Display offset value (usually zero for normal operation) */
		#define OLED_CMD_SET_DISPLAY_OFFSET_VALUE 0x00
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_SET_DISPLAY_START_LINE Display Start Line Command
	 * @brief Command and option for setting the vertical start line of the OLED display.
	 * 
	 * @details
	 * The macro `OLED_CMD_SET_DISPLAY_START_LINE_OFFSET` specifies the offset value for the display start line, with valid values in the range `0x00` to `0x3F`. The macro `OLED_CMD_SET_DISPLAY_START_LINE` combines the base command with the offset to set the vertical start line position for the display content.
	 * 
	 * @note These macros allow adjustment of which scan line is mapped to the top of the OLED screen, useful for shifting or custom screen arrangements.
	 * 
	 * Usage Example:
	 * To set the display start line, send the command `OLED_CMD_SET_DISPLAY_START_LINE`.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_DISPLAY_START_LINE
		/* Possible line offset 0x00 - 0x3F */
		#define OLED_CMD_SET_DISPLAY_START_LINE_OFFSET 0x00
		
		/** Command to set the vertical start line position */
		#define OLED_CMD_SET_DISPLAY_START_LINE (0x40 | OLED_CMD_SET_DISPLAY_START_LINE_OFFSET)
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_SET_CHARGE_PUMP Charge Pump Command
	 * @brief Command and options for configuring the OLED display's charge pump.
	 * 
	 * @details
	 * The macro `OLED_CMD_SET_CHARGE_PUMP` defines the command byte used to set the charge pump configuration of the OLED display.
	 * The macros:
	 * - `OLED_CMD_SET_CHARGE_PUMP_EXTERNAL` enables the use of an external charge pump.
	 * - `OLED_CMD_SET_CHARGE_PUMP_INTERNAL` enables the use of the internal charge pump circuit.
	 * 
	 * @warning These settings are critical for proper power management and operation of the OLED panel, depending on the system's hardware configuration.
	 * 
	 * Usage Example:
	 * To select a charge pump source, send the command `OLED_CMD_SET_CHARGE_PUMP` followed by either `OLED_CMD_SET_CHARGE_PUMP_EXTERNAL` or `OLED_CMD_SET_CHARGE_PUMP_INTERNAL`.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_CHARGE_PUMP
		#define OLED_CMD_SET_CHARGE_PUMP 0x8D

		/** Option to use external charge pump */
		#define OLED_CMD_SET_CHARGE_PUMP_EXTERNAL 0x10

		/** Option to use internal charge pump */
		#define OLED_CMD_SET_CHARGE_PUMP_INTERNAL 0x14
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_SET_SEGMENT_REMAP Segment Remap Command
	 * @brief Command and options for configuring segment remapping of the OLED display.
	 * 
	 * @details
	 * The macros within this group define options for controlling the mapping of the segment drivers to the display columns.
	 * - `OLED_CMD_SET_SEGMENT_REMAP_STANDARD` sets the default (standard) column mapping.
	 * - `OLED_CMD_SET_SEGMENT_REMAP_REVERSED` reverses the column mapping for horizontal mirroring of the display.
	 * 
	 * @note The macro `OLED_CMD_SET_SEGMENT_REMAP` combines the base command with the selected remap option. These settings allow flexible screen layouts, such as mirrored output.
	 * 
	 * Usage Example:
	 * To reverse the display segment mapping, send the command `OLED_CMD_SET_SEGMENT_REMAP`.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_SEGMENT_REMAP
		#define OLED_CMD_SET_SEGMENT_REMAP_STANDARD 0x00
		#define OLED_CMD_SET_SEGMENT_REMAP_REVERSED 0x01

		/** Command to set segment remap mode (here set to reversed layout) */
		#define OLED_CMD_SET_SEGMENT_REMAP (0xA0 | OLED_CMD_SET_SEGMENT_REMAP_REVERSED)
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION COM Output Scan Direction Command
	 * @brief Command and options for setting the COM output scan direction on the OLED display.
	 * 
	 * @details
	 * These macros define the scan direction for the display's COM (common) output signals.
	 * - `OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL` configures the normal (default) scanning direction.
	 * - `OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_REVERSED` reverses the scan direction, effectively flipping the vertical orientation of the display.
	 * 
	 * The macro `OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION` combines the base command with the selected scan direction option.
	 * 
	 * @note This setting is useful for adjusting display orientation without modifying graphic data.
	 * 
	 * Usage Example:
	 * Send the command `OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION`. with the desired configuration byte.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION
		#define OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL 0x00
		#define OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_REVERSED 0x08

		/** Command to set the COM output scan direction */
		#define OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION (0xC0 | OLED_CMD_SET_COM_OUTPUT_SCAN_DIRECTION_REVERSED)
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION COM Pins Hardware Configuration Command
	 * @brief Command and options for configuring the hardware setup of COM pins on the OLED display.
	 * 
	 * @details
	 * The macro `OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION` defines the command byte used to configure the COM pins hardware layout.
	 * The following macros specify configuration options:
	 * - `OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_SEQUENTIAL` sets sequential COM pin configuration.
	 * - `OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_ALTERNATIVE` sets alternative COM pin configuration.
	 * - `OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_NOREMAP` specifies no segment remapping.
	 * - `OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_REMAP` enables segment remapping.
	 * 
	 * @note The combined macro `OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_SETUP` applies the used configuration.
	 * 
	 * Usage Example:
	 * To configure the COM pins, send the command `OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION`
	 * followed by `OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_SETUP`.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION
		#define OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION 0xDA
		
		/** Sequential COM pin configuration */
		#define OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_SEQUENTIAL  0x02
		
		/** Alternative COM pin configuration */
		#define OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_ALTERNATIVE 0x12
		
		/** No segment remapping */
		#define OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_NOREMAP 0x02
		
		/** Enable segment remapping */
		#define OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_REMAP 0x22

		/** Current hardware setup */
		#define OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_SETUP (OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_ALTERNATIVE | OLED_CMD_SET_COM_PINS_HARDWARE_CONFIGURATION_NOREMAP)
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_SET_CONTRAST Contrast Control Command
	 * @brief Command and option for setting the OLED display contrast.
	 * 
	 * @details
	 * The macro `OLED_CMD_SET_CONTRAST` defines the command byte used to adjust the contrast level of the OLED display. The macro `OLED_CMD_SET_CONTRAST_VALUE` specifies the contrast intensity value, typically ranging from 0x00 (lowest) to 0xFF (highest).
	 * 
	 * @note This setting controls the brightness and visibility of the displayed content.
	 * 
	 * Usage Example:
	 * To set the display contrast, send the command `OLED_CMD_SET_CONTRAST` followed by `OLED_CMD_SET_CONTRAST_VALUE`.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_CONTRAST
		#define OLED_CMD_SET_CONTRAST 0x81

		/** Contrast level value (default: 0x7F, mid-level contrast) */
		#define OLED_CMD_SET_CONTRAST_VALUE 0x7F
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_ENTIRE_DISPLAY_ON Entire Display ON Command
	 * @brief Command and options for controlling the entire display illumination.
	 * 
	 * @details
	 * The macros control whether the OLED display outputs pixels based on the RAM content or turns all pixels fully on.
	 * - `OLED_CMD_ENTIRE_DISPLAY_ON_RESUME_RAM` configures the display to show the actual RAM content (normal operation).
	 * - `OLED_CMD_ENTIRE_DISPLAY_ON_IGNORE_RAM` forces all pixels on regardless of RAM content (useful for testing or visual effects).
	 * 
	 * The macro `OLED_CMD_ENTIRE_DISPLAY_ON` combines the base command with the option to resume normal RAM-based display.
	 * 
	 * Usage Example:
	 * To turn the whole display on, send the command `(OLED_CMD_ENTIRE_DISPLAY_ON | OLED_CMD_ENTIRE_DISPLAY_ON_IGNORE_RAM)` and to resume that data is red by ram send the command `OLED_CMD_ENTIRE_DISPLAY_ON` or for better readability`(OLED_CMD_ENTIRE_DISPLAY_ON (0xA4 | OLED_CMD_ENTIRE_DISPLAY_ON_RESUME_RAM)`.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_ENTIRE_DISPLAY_ON
		#define OLED_CMD_ENTIRE_DISPLAY_ON_RESUME_RAM 0x00
		#define OLED_CMD_ENTIRE_DISPLAY_ON_IGNORE_RAM 0x01

		/** Command to resume normal display (show RAM content) */
		#define OLED_CMD_ENTIRE_DISPLAY_ON (0xA4 | OLED_CMD_ENTIRE_DISPLAY_ON_RESUME_RAM)
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_SET_DISPLAY_MODE Display Mode Command
	 * @brief Command and options for setting the OLED display mode (normal or inverse).
	 * 
	 * @details
	 * The macros control the display pixel rendering mode:
	 * - `OLED_CMD_SET_DISPLAY_MODE_NORMAL` sets the display to normal mode, where pixels are lit according to the RAM content.
	 * - `OLED_CMD_SET_DISPLAY_MODE_INVERSE` sets the display to inverse mode, where pixels are inverted (lit pixels become off and vice versa).
	 * 
	 * The macro `OLED_CMD_SET_DISPLAY_MODE` combines the base command with the normal mode option by default.
	 * 
	 * @note This setting allows for visual effects or visibility adjustments.
	 * 
	 * Usage Example:
	 * To set the display to normal mode, send the command `OLED_CMD_SET_DISPLAY_MODE`.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_DISPLAY_MODE
		#define OLED_CMD_SET_DISPLAY_MODE_NORMAL 0x00
		#define OLED_CMD_SET_DISPLAY_MODE_INVERSE 0x01

		/** Command to set display mode (default: normal) */
		#define OLED_CMD_SET_DISPLAY_MODE (0xA6 | OLED_CMD_SET_DISPLAY_MODE_NORMAL)
	#endif
	/** @} */
	
	/**
	 * @defgroup OLED_CMD_SET_MEMORY_ADDRESSING_MODE Memory Addressing Mode Command
	 * @brief Command and options for configuring the OLED display memory addressing mode.
	 * 
	 * @details
	 * The `OLED_CMD_SET_MEMORY_ADDRESSING_MODE` macro defines the command byte used to set how the display RAM is accessed and updated.
	 * The addressing modes includes:
	 * - `OLED_CMD_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL`: Addresses increment horizontally across columns then move vertically.
	 * - `OLED_CMD_SET_MEMORY_ADDRESSING_MODE_VERTICAL`: Addresses increment vertically down rows then move horizontally.
	 * - `OLED_CMD_SET_MEMORY_ADDRESSING_MODE_PAGE`: Addresses increment within pages (typically 8 pixel rows) before moving to the next page.
	 * 
	 * @note These modes affect how data is written to the display and can be chosen based on application needs for memory access patterns and performance.
	 * 
	 * Usage Example:
	 * To set page addressing mode, send the command `OLED_CMD_SET_MEMORY_ADDRESSING_MODE` followed by `OLED_CMD_SET_MEMORY_ADDRESSING_MODE_PAGE`.
	 * 
	 * @{
	 */
	#ifndef OLED_CMD_SET_MEMORY_ADDRESSING_MODE
		#define OLED_CMD_SET_MEMORY_ADDRESSING_MODE 0x20

		/** Horizontal addressing mode */
		#define OLED_CMD_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL 0x00

		/** Vertical addressing mode */
		#define OLED_CMD_SET_MEMORY_ADDRESSING_MODE_VERTICAL   0x01

		/** Page addressing mode */
		#define OLED_CMD_SET_MEMORY_ADDRESSING_MODE_PAGE       0x02
	#endif
	/** @} */
	
	/**
	 * @def OLED_CMD_SET_PAGE_START_ADDRESS
	 * @brief Command to set the start address of a page in the OLED display memory.
	 * 
	 * @details
	 * This macro defines the command byte used to select the page (row group) in the display RAM to begin writing data. The OLED display memory is organized into pages, each typically 8 pixels tall.
	 * 
	 * @note Setting the page start address allows targeted updates to specific vertical sections of the display.
	 * 
	 * Usage Example:
	 * To write data to a specific page, send the command `OLED_CMD_SET_PAGE_START_ADDRESS` followed by the page number.
	 */
	#ifndef OLED_CMD_SET_PAGE_START_ADDRESS
		#define OLED_CMD_SET_PAGE_START_ADDRESS 0xB0
	#endif
	
	/**
	 * @def OLED_CMD_SET_LOWER_START_COLUMN_ADDRESS
	 * @brief Command to set the lower nibble of the start column address in the OLED display memory.
	 * 
	 * @details
	 * This macro defines the command byte used to set the lower 4 bits (nibble) of the column start address, specifying the horizontal position in display RAM where data writing begins. It works in conjunction with the higher nibble address command to address the full (`0`-`127`) column range of the display.
	 * 
	 * Usage Example:
	 * To set the column start address, send this command with the lower nibble data `(OLED_CMD_SET_LOWER_START_COLUMN_ADDRESS | data)` followed by the command for the higher nibble.
	 */
	#ifndef OLED_CMD_SET_LOWER_START_COLUMN_ADDRESS
		#define OLED_CMD_SET_LOWER_START_COLUMN_ADDRESS 0x00
	#endif

	/**
	 * @def OLED_CMD_SET_HIGHER_START_COLUMN_ADDRESS
	 * @brief Command to set the higher nibble of the start column address in the OLED display memory.
	 * 
	 * @details
	 * This macro defines the command byte used to set the upper 4 bits (nibble) of the column start address, specifying the higher portion of the horizontal position in display RAM where data updates will begin.
	 * 
	 * @note Combined with the lower nibble command, it allows full addressing across all columns.
	 * 
	 * Usage Example:
	 * Follow the lower nibble command and data with this higher nibble command and data `(OLED_CMD_SET_HIGHER_START_COLUMN_ADDRESS | (data>>4))` to set the full column start address.
	 */
	#ifndef OLED_CMD_SET_HIGHER_START_COLUMN_ADDRESS
		#define OLED_CMD_SET_HIGHER_START_COLUMN_ADDRESS 0x10
	#endif
	
    #include <avr/io.h>
	#include <util/delay.h>
	
	#ifdef OLED_INIT_ROUTINE_IN_EEPROM
		#include <avr/eeprom.h>
	#else
		#include <avr/pgmspace.h>
	#endif
	
	/**
	 * @defgroup OLED_TWI_Control_Macros OLED Software/Hardware TWI (I2C) Control Macros
	 * @brief Macros for abstracting OLED TWI (I2C) communication interface.
	 * 
	 * @details
	 * These macros provide a unified interface for starting, stopping, and sending data over the TWI bus to the OLED display, abstracting the use of either software-based or hardware-based TWI implementations.
	 * 
	 * @note When `OLED_USE_SOFT_TWI` is defined, the macros use software TWI functions; otherwise, hardware TWI is used.
	 * 
	 * Individual macros:
	 * - `OLED_START(MODE)`: Initiates a TWI start condition and sends the OLED’s I2C address with the specified mode (`MODE` indicates read or write) to begin communication.
	 * - `OLED_STOP()`: Sends a TWI stop condition to end communication and then inserts a delay (`OLED_IDLE_TIME_US`) to allow the OLED sufficient processing time before the next communication.
	 * - `OLED_DATA()`: Sends the control byte indicating that the following bytes will be data (pixel or graphic data) to the OLED display, rather than commands.
	 * 
	 * This abstraction simplifies OLED communication by hiding the underlying method, enabling easy switching between hardware and software TWI implementations without changing higher-level code.
     * @{
     */
    #ifdef OLED_USE_SOFT_TWI
        #include "../twi_soft/twi_soft.h"

        #define OLED_START(MODE) { twi_soft_start(); twi_soft_address(OLED_ADDRESS, MODE); }
        #define OLED_STOP() { twi_soft_stop(); _delay_us(OLED_IDLE_TIME_US); }
        #define OLED_DATA() (twi_soft_set(OLED_CONTROL_DATA))
    #else
        #include "../twi/twi.h"

        #define OLED_START(MODE) { twi_start(); twi_address(OLED_ADDRESS, MODE); }
        #define OLED_STOP() { twi_stop(); _delay_us(OLED_IDLE_TIME_US); }
        #define OLED_DATA() (twi_set(OLED_CONTROL_DATA))
    #endif
	/** @} */

	/**
	 * @def OLED_HOME
	 * @brief Sets the cursor position to the top-left corner of the OLED display.
	 *
	 * @details
	 * This macro calls the function `oled_position` with coordinates (0, 0), which moves the cursor to the first column and first page (row) of the OLED screen.
	 * 
	 * @note It is typically used to reset the write position before updating or clearing the display.
	 */
    #define OLED_HOME() (oled_position(0, 0))
	
    void oled_init(void);
    void oled_disable(void);
	void oled_home(void);
	void oled_position(unsigned char column, unsigned char row);
	void oled_scroll_vertical(unsigned char scroll);
	void oled_frame(unsigned char *frame);
	void oled_page(unsigned char *data, unsigned char page);
	void oled_page_segment(unsigned char *data, unsigned char column_start, unsigned char column_stop, unsigned char page);
	void oled_column(unsigned char data, unsigned char column, unsigned char page);
	void oled_clear(void);
	void oled_clear_page(unsigned char page);
	void oled_clear_page_segment(unsigned char column_start, unsigned char column_stop, unsigned char page);
	void oled_clear_column(unsigned char column, unsigned char page);

#endif /* OLED_H_ */