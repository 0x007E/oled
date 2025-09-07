---
layout: post
title: twi.h - Hardware TWI parameter definition
categories: [Parameters]
introduction: "Descriptions of parameters that can be adapted in the twi header"
---

Hardware abstraction layer (`HAL`) for communicating over `TWI/I2C` with the display. Basic settings like the speed of the bus can be defined in `twi.h`. 

> Hardware-defined TWI makes communication inflexible with regard to ports, as these cannot be switched.

## TWI-Bus speed definition

The TWI transmission speed can be set up within the `F_TWI` macro (`100 kHz` is set by default).

```c
#define F_TWI 100000UL
```

## Prescaler for TWI clock generation

The TWI bitrate register can only take `8 Bits` so it is necessary to setup the correct prescaler. The faster the bus goes the lower should be the prescaler.

Calculation of the bitrate register (allowed values: `1-255`):

[![TWI Calculation](https://quicklatex.com/cache3/04/ql_745096d24c0735e73fb7f9fce478df04_l3.png)](https://www.quicklatex.com/)

```c
#define TWI_PRESCALER 0
```

> Valid prescaler values are `0`, `1`, `2`, and `3`, representing division factors of `1`, `4`, `16`, and `64` respectively.

---

For a more detailed view of the possible parameters that can be set in `twi.h`, take a look at the [OLED Doxygen documentation (`OLDD`)](https://0x007e.github.io/oled/doxygen/twi_8h.html).
