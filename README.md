[![Version: 1.0 Release](https://img.shields.io/badge/Version-1.0%20Release-green.svg)](https://github.com/0x007e/oled) [![License GPLv3](https://img.shields.io/badge/License-GPLv3-lightgrey)](https://www.gnu.org/licenses/gpl-3.0.html) ![Release](https://github.com/0x007e/oled/actions/workflows/release.yml/badge.svg)

# OLED Display Library (SSD1306)

This project includes a easy to use OLED library for an `SSD1306`/`SH1106` that can be used with an `ATmega`- or `ATmega0`-Series controller. The library itself can be found in the [lib](./lib) directory and demo applications for `avr` are pushed to [demo](./demo) and for `avr0` to [demo-avr0](./demo-avr0) directory. A short description of how to use the library for `avr` platforms can be found below.

## Advanced guides

| Link                                                                | Description                                       |
|:--------------------------------------------------------------------|:--------------------------------------------------|
| [Extended Documentation](https://0x007e.github.io/oled/)            | Detailed documentation of howto using the library |
| [Doxygen Code Documentation](https://0x007e.github.io/oled/doxygen) | Detailed documentation of defines and functions   |
| [Graphics Design Panel](https://0x007e.github.io/oled-designer/)    | Panel to design graphics for the `frame` library  |

## Demo Binaries

> Download all firmware releases as [zip](https://github.com/0x007E/oled/releases/latest/download/firmware.zip) | [tar.gz](https://github.com/0x007E/oled/releases/latest/download/firmware.tar.gz)

### ATmega16A

| Name                                                                                                   | Type  | Controller | CLOCK | SCL | SDA | Description             |
|:------------------------------------------------------------------------------------------------------:|:-----:|:----------:|:-----:|:---:|:---:|:------------------------|
| [tty_sw_twi_m16a.hex](https://github.com/0x007E/oled/releases/latest/download/tty_sw_twi_m16a.hex)     | tty   | ATmega16A  | 12MHz | PB0 | PB1 | TTY with software TWI   |
| [tty_hw_twi_m16a.hex](https://github.com/0x007E/oled/releases/latest/download/tty_hw_twi_m16a.hex)     | tty   | ATmega16A  | 12MHz | PC0 | PC1 | TTY with hardware TWI   |
| [frame_sw_twi_m16a.hex](https://github.com/0x007E/oled/releases/latest/download/frame_sw_twi_m16a.hex) | frame | ATmega16A  | 12MHz | PB0 | PB1 | FRAME with software TWI |
| [frame_hw_twi_m16a.hex](https://github.com/0x007E/oled/releases/latest/download/frame_hw_twi_m16a.hex) | frame | ATmega16A  | 12MHz | PC0 | PC1 | FRAME with hardware TWI |

### ATmega4808

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

The `oled` library currently supports the `avr` and `avr0` plattform. The platform can be defined within the `oled.h` library. By default `avr` is selected.

```c
// For AVR platform
#define OLED_HAL_PLATFORM avr

// For AVR0 platform
#define OLED_HAL_PLATFORM avr0
```

> `Microchip Studio` compiles all libraries in the folders by default. Therefore, either `avr` or `avr0` should be created in the folder structure and the library that is not used should be removed. To select the correct platform and set the clock speed correctly in the libraries, it is best to create a `global define` under `Project-Settings -> AVR/GNU C-Compiler -> Symbols` in `Microchip studio`. 

```c 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! IMPORTANT NOTICE                                  !!
// !! Setup following symbols:                          !!
// !! Project-Settings -> AVR/GNU C-Compiler -> Symbols !!
// !!                                                   !!
// !! F_CPU=20000000UL                                  !!
// !! OLED_HAL_PLATFORM=avr0                            !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
```

### Communication

The `oled` library supports two communication modes. It is possible to use the `hardware` or the `software-twi` mode. These settings can be adapted in the [oled.h](./lib/oled/oled.h).

#### Hardware TWI-Mode

To use `hardware-twi` the following macro has to be undefined in [oled.h](./lib/oled/oled.h).

```c
// #define OLED_USE_SOFT_TWI
```

> The standard `pins` described in the controller datasheet are used for TWI communication.

#### Software TWI-Mode

To use `software-twi` the following macro has to be defined in [oled.h](./lib/oled/oled.h).

```c
#define OLED_USE_SOFT_TWI
```

> In addition, the “pins” on which the communication takes place can be adjusted in [oled.h](./lib/oled/oled.h).

### TTY-Mode

The `tty`-mode allows to write a text to the display `line-by-line`.

```c
#define F_CPU 12000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "../../lib/oled/tty/tty.h"

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

Autoscroll in `tty` mode can be enabled via the `autoscroll` macro (this is usually the case) in [tty.h](./lib/oled/tty/tty.h).

```c
#define TTY_AUTOSCROLL
```

### Frame-Mode

The `frame`-mode allows you to use a background graphic for text or a status bar.

> The background graphic itself consumes a huge amount of program memory (`1KiB`). The background can be disabled on smaller controllers to prevent the system from loading it when the display initializes to save space.

The background graphic is enabled by default with the `FRAME_SPECIFIC_BACKGROUND` macro in [frame.c](./lib/oled/frame/frame.c) and can be deactivated by commenting it out.

```c
#define FRAME_SPECIFIC_BACKGROUND
```

To simplify the graphic design process, the project has a built-in tool called the [OLED Graphics Design Panel](https://0x007e.github.io/oled-designer/). Within this tool it is possible to create any specific background for the display.

> After designing the background the created array has to be placed in the `frame_background` variable at [frame.c](./lib/oled/frame/frame.c).

A background is already designed and filed in [frame.c](./lib/oled/frame/frame.c) for testing purposes.

```c
#define F_CPU 12000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "../../lib/oled/frame/frame.h"

int main(void)
{
	frame_init();

	DRAWING_Position position = { 106, 1 };
	DRAWING_Size size = { 10, 10 };
	
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

## Datasheets

- [SSD1306](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [ATmega16A](https://ww1.microchip.com/downloads/en/devicedoc/atmel-8154-8-bit-avr-atmega16a_datasheet.pdf)
- [ATmega4808](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf)

---

R. GAECHTER