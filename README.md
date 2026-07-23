[![Version: 1.0 Release](https://img.shields.io/badge/Version-1.0%20Release-green.svg)](https://github.com/0x007e/oled) ![Release](https://github.com/0x007e/oled/actions/workflows/release.yml/badge.svg) [![License GPLv3](https://img.shields.io/badge/License-GPLv3-lightgrey)](https://www.gnu.org/licenses/gpl-3.0.html)

# OLED Display Library (SSD1306)

[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/0x007E/oled)

This project includes a easy to use OLED library for an `SSD1306`/`SH1106` that can be used with an `ATmega`- or `ATmega0`-series controller. The library itself can be found in the [drivers-display-ssd130x](https://github.com/0x007e/drivers-display-ssd130x) repository and a demo  applications for `avr(0)` is pushed to [demo](./demo) directory. A short description on how to use the library for `avr` platforms can be found [below](#advanced-guides).

## Advanced guides

| Link                                                                                   | Description                                       |
|:---------------------------------------------------------------------------------------|:--------------------------------------------------|
| [Extended Documentation](https://0x007e.github.io/oled/)                               | Detailed documentation on howto using the library |
| [Doxygen Code Documentation](https://0x007e.github.io/drivers-display-ssd130x/doxygen) | Detailed documentation of defines and functions   |
| [Graphics Design Panel](https://0x007e.github.io/oled-designer/)                       | Panel to design graphics for the `frame` library  |

## Binaries / Libraries

> Download all firmware releases as [zip](https://github.com/0x007E/oled/releases/latest/download/firmware.zip) | [tar.gz](https://github.com/0x007E/oled/releases/latest/download/firmware.tar.gz) or the whole ssd130x library as [zip](https://github.com/0x007E/drivers-display-ssd130x/releases/latest/download/library.zip) / [tar](https://github.com/0x007E/drivers-display-ssd130x/releases/latest/download/library.tar.gz).

### `ATmega16A`

| Name                                                                                                   | Type  | Controller | CLOCK | SCL | SDA | Description             |
|:------------------------------------------------------------------------------------------------------:|:-----:|:----------:|:-----:|:---:|:---:|:------------------------|
| [tty_sw_twi_m16a.hex](https://github.com/0x007E/oled/releases/latest/download/tty_sw_twi_m16a.hex)     | tty   | ATmega16A  | 12MHz | PB0 | PB1 | TTY with software TWI   |
| [tty_hw_twi_m16a.hex](https://github.com/0x007E/oled/releases/latest/download/tty_hw_twi_m16a.hex)     | tty   | ATmega16A  | 12MHz | PC0 | PC1 | TTY with hardware TWI   |
| [frame_sw_twi_m16a.hex](https://github.com/0x007E/oled/releases/latest/download/frame_sw_twi_m16a.hex) | frame | ATmega16A  | 12MHz | PB0 | PB1 | FRAME with software TWI |
| [frame_hw_twi_m16a.hex](https://github.com/0x007E/oled/releases/latest/download/frame_hw_twi_m16a.hex) | frame | ATmega16A  | 12MHz | PC0 | PC1 | FRAME with hardware TWI |

### `ATmega4808`

| Name                                                                                                   | Type  | Controller | CLOCK | SCL | SDA | Description             |
|:------------------------------------------------------------------------------------------------------:|:-----:|:----------:|:-----:|:---:|:---:|:------------------------|
| [tty_sw_twi_m4808.hex](https://github.com/0x007E/oled/releases/latest/download/tty_sw_twi_m4808.hex)     | tty   | ATmega4808  | 20MHz | PD2 | PD3 | TTY with software TWI   |
| [tty_hw_twi_m4808.hex](https://github.com/0x007E/oled/releases/latest/download/tty_hw_twi_m4808.hex)     | tty   | ATmega4808  | 20MHz | PC3 | PC2 | TTY with hardware TWI   |
| [frame_sw_twi_m4808.hex](https://github.com/0x007E/oled/releases/latest/download/frame_sw_twi_m4808.hex) | frame | ATmega4808  | 20MHz | PD2 | PD3 | FRAME with software TWI |
| [frame_hw_twi_m4808.hex](https://github.com/0x007E/oled/releases/latest/download/frame_hw_twi_m4808.hex) | frame | ATmega4808  | 20MHz | PC3 | PC2 | FRAME with hardware TWI |

## Quick startup guide

The library to control the display supports two modes.

- `tty` - Text Mode
- `frame` - Graphical Mode

### Platform selection

The [`ssd130x`](https://github.com/0x007E/drivers-display-ssd130x) library currently supports the `avr` and `avr0` plattform. The platform can be defined within the [`ssd130x.h`](https://github.com/0x007E/drivers-display-ssd130x/blob/main/ssd130x.h) library. By default `avr` is selected.

```c
// For AVR platform (Standard)
#define SSD130X_HAL_PLATFORM avr

// For AVR0 platform
#define SSD130X_HAL_PLATFORM avr0
```

> `Microchip Studio` compiles all libraries in the folders by default. Therefore, either `avr` or `avr0` should be created in the folder structure and the library that is not used should be removed. To select the correct platform and set the clock speed correctly in the libraries, it is best to create a `global define` under `Project-Settings -> AVR/GNU C-Compiler -> Symbols` in `Microchip studio`. 

```c 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! IMPORTANT NOTICE                                  !!
// !! Setup following symbols:                          !!
// !! Project-Settings -> AVR/GNU C-Compiler -> Symbols !!
// !!                                                   !!
// !! -> AVR (Megacard)                                 !!
// !! F_CPU=12000000UL                                  !!
// !! SSD130X_HAL_PLATFORM=avr (Standard)               !!
// !!                                                   !!
// !! -> AVR0                                           !!
// !! F_CPU=20000000UL                                  !!
// !! SSD130X_HAL_PLATFORM=avr0                         !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
```

### Communication

The [`ssd130x`](https://github.com/0x007E/drivers-display-ssd130x) library supports two communication modes. It is possible to use the `hardware` or the `software-twi` mode. These settings can be adapted in the [`ssd130x.h`](https://github.com/0x007E/drivers-display-ssd130x/blob/main/ssd130x.h) header.

#### Hardware TWI-Mode (Standard)

To use `hardware-twi` the following macro has to be undefined in [`ssd130x.h`](https://github.com/0x007E/drivers-display-ssd130x/blob/main/ssd130x.h).

```c
// #define SSD130X_USE_SOFT_TWI
```

> The standard `pins` described in the controller datasheet are used for TWI communication.

#### Software TWI-Mode

To use `software-twi` the following macro has to be defined in [`ssd130x.h`](https://github.com/0x007E/drivers-display-ssd130x/blob/main/ssd130x.h).

```c
#define SSD130X_USE_SOFT_TWI
```

> In addition, the “pins” on which the communication takes place can be adjusted (if `portmux` is supported or `software-twi` is used) in the specific `twi`-header file.

### TTY-Mode

The `tty`-mode allows to write a text to the display `line-by-line`.

```c
#define F_CPU 12000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "../../lib/drivers/display/ssd130x/tty/tty.h"

void systick_timer_wait_us(unsigned int us)
{
	us = (((us - 2)>>1) + 1);

	for(unsigned int i = 0; i < us; i++)
	{
    	_delay_us(1);
	}
}

int main(void)
{
	tty_init();

    unsigned char temp = 0;
	while (1)
	{
		printf("Text %u\n", (temp++));
        _delay_ms(10);
	}
}
```

#### Autoscroll

Autoscroll in `tty` mode can be enabled via the `autoscroll` macro (this is usually the case) in [tty.h](https://github.com/0x007E/drivers-display-ssd130x/blob/main/tty/tty.h).

```c
#define TTY_AUTOSCROLL
```

### Frame-Mode

The `frame`-mode allows you to use a background graphic for text or a status bar.

> The background graphic itself consumes a huge amount of program memory (`1KiB`). The background can be disabled on smaller controllers to prevent the system from loading it when the display initializes to save space.

The background graphic is enabled by default with the `FRAME_SPECIFIC_BACKGROUND` macro in [frame.h](https://github.com/0x007E/drivers-display-ssd130x/blob/main/frame/frame.h) and can be deactivated by commenting it out.

```c
#define FRAME_SPECIFIC_BACKGROUND
```

To simplify the graphic design process, the project has a built-in tool called the [OLED Graphics Design Panel](https://0x007e.github.io/oled-designer/). Within this tool it is possible to create any specific background for the display.

> After designing the background the created array has to be placed in the `frame_background` variable at [frame.c](https://github.com/0x007E/drivers-display-ssd130x/blob/main/frame/frame.c).

A background is already designed and added to [frame.c](https://github.com/0x007E/drivers-display-ssd130x/blob/main/frame/frame.c) for testing purposes.

```c
#define F_CPU 12000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "../../lib/drivers/display/ssd130x/frame/frame.h"

void systick_timer_wait_us(unsigned int us)
{
	us = (((us - 2)>>1) + 1);

	for(unsigned int i = 0; i < us; i++)
	{
    	_delay_us(1);
	}
}

int main(void)
{
	frame_init();

	GFX_Position position = { 106, 1 };
	GFX_Size size = { 10, 10 };
	
	frame_draw_text("Init", position);
	
	unsigned char temp = 0;
	
	while (1)
	{
		position.x = 1;
		position.y = 56;
		size.width = 126;
		size.height = 6;
		
		frame_draw_bar(position, size, temp++);
		
		if(temp >= 100UL)
		{
			temp = 0;
		}
		
		position.x = 2;
		position.y = 46;
		frame_draw_number_uint(temp, 4, NUMBER_Decimal, position);
		
		_delay_ms(50);
	}
}
```

# Additional Information

| Type        | Link                                                                                                 | Description                                     |
|:-----------:|:----------------------------------------------------------------------------------------------------:|-------------------------------------------------|
| SSD1306     | [pdf](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)                                          | OLED/PLED Segment/Common Driver with Controller |
| ATmega16A   | [pdf](https://ww1.microchip.com/downloads/en/devicedoc/atmel-8154-8-bit-avr-atmega16a_datasheet.pdf) | Datasheet of ATmega16A                          |
| ATmega4808  | [pdf](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf)   | Datasheet of ATmega4808                         |

---

R. GAECHTER