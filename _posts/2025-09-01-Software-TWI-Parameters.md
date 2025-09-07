---
layout: post
title: twi_soft.h - Software TWI parameter definition
categories: [Parameters]
introduction: "Descriptions of parameters that can be adapted in the twi_soft header"
---

Hardware abstraction layer (`HAL`) for communicating over `TWI/I2C` with the display. Basic settings like the speed of the bus and pins can be defined in `twi_soft.h`. 

> Software-defined TWI makes communication more flexible because the ports can be switched.

## TWI-Bus speed definition

The TWI transmission speed can be set up within the `F_SOFT_TWI` macro (`100 kHz` is set by default).

```c
#define F_SOFT_TWI 100000UL
```

## Internal/External pull-up resistors

The software-twi bus can either be run with external or with internal pull-up resistors. The internal pull-up resistors can be enabled with the `TWI_SOFT_PULLUP_ENABLE` macro (not enabled by default).

```c
#define TWI_SOFT_PULLUP_ENABLE
```

> The internal pull-up resistors depend on the microcontroller and have a wide range. Therefore, low bus transfer rates should be selected without measuring with an oscilloscope.

## Pin definition of TWI-SCL/SDA

The advantage of the software TWI bus is that the pins for SCL and SDA can be freely selected. They just have to be in the same register. Therefore it is necessary to define the whole bunch of registers and pins.

```c
// Register set for SCL/SDA pins
#define TWI_SOFT_DDR  DDRB
#define TWI_SOFT_PORT PORTB
#define TWI_SOFT_PIN  PINB

// SCL/SDA pins
#define TWI_SOFT_SCL  PINB0
#define TWI_SOFT_SDA  PINB1
```

## Interrupt blocking during transmission

If a transmission should not be interrupted, the software TWI library can be stopped to block interrupts during transmissions (not enabled by default).

> If enabled, no interrupt will be processed until the transfer is complete!

```c
#define TWI_SOFT_BLOCK_IE
```

---

For a more detailed view of the possible parameters that can be set in `twi_soft.h`, take a look at the [OLED Doxygen documentation (`OLDD`)](https://0x007e.github.io/oled/doxygen/twi__soft_8h.html).
