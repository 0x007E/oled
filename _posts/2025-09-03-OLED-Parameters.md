---
layout: post
title: oled.h - OLED parameter definition
categories: [Parameters]
introduction: "Descriptions of parameters that can be adapted in the oled header"
modified_date: 2025-09-19 13:37:37 +0200
---

## Platform selection

The hardware abstraction layer (`HAL`) of the OLED display currently supports the microcontrollers `avr` (ATmega16A, ...) and `avr0` (ATmegaXX08/ATTinyXX06, ...), which can be easily switched using the `OLED_HAL_PLATFORM` macro.
 
```c
// For AVR platform
#define OLED_HAL_PLATFORM avr

// For AVR0 platform
#define OLED_HAL_PLATFORM avr0
```

### Folder structure setup:

#### For AVR platform

```
- ./lib/ + oled.h
         + oled.c
         + hal/ + avr/ + twi/ + twi.h
                       |      + twi.c
                       + twi_soft/ + twi_soft.h
                                   + twi_soft.c
         ...  
```

#### For AVR0 platform

```
- ./lib/ + oled.h
         + oled.c
         + hal/ + avr0/ + twi/ + twi.h
                        |      + twi.c
                        + twi_soft/ + twi_soft.h
                                    + twi_soft.c
         ...  
```

> `Microchip Studio` compiles all libraries in the folders by default. Therefore, either `avr` or `avr0` should be created in the folder structure and the library that is not used should be removed. To select the correct platform and set the clock speed correctly in the libraries, it is best to create a `global define` under `Project-Settings -> AVR/GNU C-Compiler -> Symbols` in `Microchip studio`. 

```c 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! GLOBAL DEFINES FOR AVR0 PLATFORM                  !!
// !! F_CPU=20000000UL                                  !!
// !! OLED_HAL_PLATFORM=avr0                            !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
```

## Communication mode of the display

Communication between the display and the microcontroller can be achieved either via software-defined TWI or via the microcontroller's internal hardware in `oled.h`. Software-defined TWI makes communication more flexible because the ports can be switched.

To use software defined TWI it is necessary to setup the `OLED_USE_SOFT_TWI` macro (is set by default).

```c
#define OLED_USE_SOFT_TWI
```

> TWI bus details can be set up in `twi.h` for hardware or in `twi_soft.h` for software setup.

## Address of the display

The address of the display can be defined within `OLED_ADDRESS` macro. 

```c
#define OLED_ADDRESS 0x7F
```

> On some displays the Address is directly in 8-bit mode `[8:1]`. Note that the `OLED_ADDRESS` macro takes only 7-bit addresses `[6:0]`!

## Display size settings

There are different displays with different sizes on the market. The library supports different formats that can be set up with the `OLED_COLUMN_SIZE` and `OLED_ROW_SIZE` macros.

```c
#define OLED_ROW_SIZE 64UL
#define OLED_COLUMN_SIZE 128UL
```

> It is also possible to adjust the page size (`OLED_PAGE_SIZE`) but normally on every display it should be `8` (bits).

## Save init routine in EEPROM

It is possible to store the `init` commands in the `EEPROM` of the microcontroller.

```c
#define OLED_INIT_ROUTINE_IN_EEPROM
```

> This can save some program memory in tiny systems (not enabled by default).

---

For a more detailed view of the possible parameters that can be set in `oled.h`, take a look at the [OLED Doxygen documentation (`OLDD`)](https://0x007e.github.io/oled/doxygen/oled_8h.html).
