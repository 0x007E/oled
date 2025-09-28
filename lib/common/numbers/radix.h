/**
 * @file radix.h
 * @brief Enumeration for numerical radix options.
 * 
 * This header file defines an enumeration representing different radix (base) options for numerical values. The enumeration includes binary, decimal, and hexadecimal bases commonly used for representing numbers.
 * 
 * @note The provided typedef `NUMBER_Radix` offers a convenient alias for the enumeration type, improving code clarity and consistency when specifying numeric bases.
 * 
 * @author g.raf
 * @date 2025-09-02
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the GPLv3 License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger project and subject to the license specified in the repository. For updates and the complete revision history, see the GitHub repository.
 * 
 * @see https://github.com/0x007e/avr-common "AVR Common GitHub Repository"
 */

#ifndef NUMBERS_RADIX_H_
#define NUMBERS_RADIX_H_

/**
 * @enum NUMBER_Radix_t
 * @brief Defines numerical radix (base) options.
 * 
 * @details
 * This enumeration specifies the numeric base used for representing numbers, including binary (base 2), decimal (base 10), and hexadecimal (base 16).
 */
enum NUMBER_Radix_t
{
    NUMBER_Binary = 2,      /**< Binary radix (base 2) */
    NUMBER_Decimal = 10,    /**< Decimal radix (base 10) */
    NUMBER_Hexadecimal = 16 /**< Hexadecimal radix (base 16) */
};

/**
 * @typedef NUMBER_Radix
 * @brief Alias for enum NUMBER_Radix_t representing numerical radix options.
 */
typedef enum NUMBER_Radix_t NUMBER_Radix;

#endif /* NUMBERS_RADIX_H_ */