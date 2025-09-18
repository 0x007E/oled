/**
 * @file drawing_elements.h
 * @brief Header file defining basic structures for 2D drawing positions and sizes.
 * 
 * This header file declares fundamental data structures used for graphical positioning and sizing in 2D space. It includes definitions for coordinate positions and dimension sizes, facilitating consistent handling of drawing-related parameters throughout the application.
 * 
 * @author g.raf
 * @date 2025-09-02
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the MIT License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger project and subject to the license specified in the repository. For updates and the complete revision history, see the GitHub repository.
 * 
 * @see https://github.com/0x007e/avr-common "AVR ATmega GitHub Repository"
 */

#ifndef DRAWING_ELEMETNS_H_
#define DRAWING_ELEMETNS_H_

	/**
	 * @brief Represents a 2D drawing position.
	 *
	 * @details
	 * This structure holds the x and y coordinates for a position in a 2D space, used for drawing or graphical positioning.
	 */
	struct DRAWING_Position_t
	{
		unsigned char x;	/**< X-coordinate of the drawing position */
		unsigned char y;	/**< Y-coordinate of the drawing position */
	};
	/**
     * @typedef DRAWING_Position
     * @brief Alias for enum DRAWING_Position_t representing a drawing point.
     */
	typedef struct DRAWING_Position_t DRAWING_Position;

	/**
	 * @brief Represents the size dimensions for drawing.
	 *
	 * @details
	 * This structure holds the width and height values used to define the size of a drawable area or object.
	 */
	struct DRAWING_Size_t
	{
		unsigned char width;	/**< Width dimension */
		unsigned char height;	/**< Height dimension */
	};
	/**
     * @typedef DRAWING_Size
     * @brief Alias for enum DRAWING_Size_t representing a drawing area/size.
     */
	typedef struct DRAWING_Size_t DRAWING_Size;

#endif /* DRAWING_ELEMETNS_H_ */