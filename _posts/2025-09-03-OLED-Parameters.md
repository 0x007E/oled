---
layout: post
title: oled.h - OLED parameter definition
categories: [Parameters]
introduction: "Descriptions of parameters that can be adapted in the oled header"
---

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
