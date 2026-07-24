---
layout: post
title: SSD130X.h - SSD130X OLED parameter definition
categories: [Parameters]
introduction: "Descriptions of parameters that can be adapted in the ssd130x header"
modified_date: 2026-07-10 13:37:37 +0200
---

## Platform selection

The hardware abstraction layer (`HAL`) of the OLED display currently supports the microcontrollers `avr` (ATmega16A, ...) and `avr0` (ATmegaXX08/ATTinyXX06, ...), which can be easily switched using the `SSD130X_HAL_PLATFORM` macro in `ssd130x.h`.
 
```c
// For AVR platform
#define SSD130X_HAL_PLATFORM avr

// For AVR0 platform
#define SSD130X_HAL_PLATFORM avr0
```

### (Complete) Folder structure:

```
drivers/
└── display/
    └── ssd130x/
        ├── ssd130x.c
        ├── ssd130x.h
        ├── frame/
        |   ├── frame.c
        |   └── frame.h
        └── tty/
            ├── tty.c
            └── tty.h

hal/
├── common/
|   ├── defines/
|   |   └── TWI_defines.h
|   └── enums/
|       └── TWI_enums.h
├── avr/
|   ├── twi/
|   |   ├── twi.c
|   |   └── twi.h
|   └── twi_soft/
|       ├── twi_soft.c
|       └── twi_soft.h
└── avr0/
    ├── twi/
    |   ├── twi.c
    |   └── twi.h
    └── twi_soft/
        ├── twi_soft.c
        └── twi_soft.h

utils/
├── macros/
|   └── stringify.h
└── systick/
    ├── systick.c
    └── systick.h
```

> `Microchip Studio` compiles all libraries in the folders by default. Therefore, either `avr` or `avr0` should be created in the folder structure and the library that is not used should be removed. To select the correct platform and set the clock speed correctly in the libraries, it is best to create a `global define` under `Project-Settings -> AVR/GNU C-Compiler -> Symbols` in `Microchip studio`. 

```c 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! GLOBAL DEFINES FOR AVR0 PLATFORM                  !!
// !! F_CPU=20000000UL                                  !!
// !! SSD130X_HAL_PLATFORM=avr0                         !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
```

## Communication mode of the display

Communication between the display and the microcontroller can be achieved either via software-defined TWI or via the microcontroller's internal hardware in `ssd130x.h`. Software-defined TWI makes communication more flexible because the pins can be switched.

To use software defined TWI it is necessary to setup the `SSD130X_USE_SOFT_TWI` macro (is disabled by default).

```c
#define SSD130X_USE_SOFT_TWI
```

> TWI bus details can be set up in `twi.h` for hardware or in `twi_soft.h` for software setup.

## Address of the display

The address of the display can be defined within `SSD130X_ADDRESS` macro. 

```c
#define SSD130X_ADDRESS 0x7F
```

> On some displays the Address is directly in 8-bit mode `[8:1]`. Note that the `SSD130X_ADDRESS` macro takes only 7-bit addresses `[6:0]`!

## Display size settings

There are different displays with different sizes on the market. The library supports different formats that can be set up with the `SSD130X_COLUMN_SIZE` and `SSD130X_ROW_SIZE` macros.

```c
#define SSD130X_ROW_SIZE 64UL
#define SSD130X_COLUMN_SIZE 128UL
```

> It is also possible to adjust the page size (`SSD130X_PAGE_SIZE`) but normally on every display it should be `8` (bits).

## Save init routine in EEPROM

It is possible to store the `init` commands in the `EEPROM` of the microcontroller.

```c
#define SSD130X_INIT_ROUTINE_IN_EEPROM
```

> This can save some program memory in tiny systems (not enabled by default).

---

For a more detailed view of the possible parameters that can be set in `ssd130x.h`, take a look at the `SSD130X` [documentation](https://0x007e.github.io/drivers-display-ssd130x/ssd130x_8h.html). The library itself can be downloades from [here](https://github.com/0x007E/drivers-display-ssd130x).
