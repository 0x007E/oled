---
layout: post
title: tty.h - TTY parameter definition
categories: [Parameters]
introduction: "Descriptions of parameters that can be adapted in the tty header"
---

The TTY is a console (terminal) that renders lines of text using a font and can output text in different modes.

## Width/Height of the TTY

The `TTY_WIDTH` and `TTY_HEIGHT` is normally set to the full width and height of the display but can be defined by user if necessary.

![TTY A-Z 0-6 xy]({{ '/assets/images/display-tty-a-z-0-6-xy.png' | relative_url }})

```c
#define TTY_WIDTH (OLED_COLUMN_SIZE/FONT_WIDTH)
#define TTY_HEIGHT (OLED_ROW_SIZE/OLED_PAGE_SIZE)
```

## Autoscroll text on display

This mode allows the TTY to scroll automatically to the next line if all lines of the display are used. After a newline `\n` the display automatically scrolls to the next line and removes the first one (is set by default).

![TTY Autoscroll 1-8]({{ '/assets/images/display-tty-autoscroll-1-8.png' | relative_url }})

![TTY Autoscroll 2-9]({{ '/assets/images/display-tty-autoscroll-2-9.png' | relative_url }})

```c
#define TTY_AUTOSCROLL
```

## Dynamic text adjustment in line

The gap between numbers and characters or characters and characters could be between 1-3 pixel. To harmonize the gap between them to 1 it is possible to set the `TTY_DYNAMIC_TEXT` macro (not enabled by default).

![TTY Dynamic Text]({{ '/assets/images/display-tty-harmonized.png' | relative_url }})

> The Text in the second line is harmonized to a gap with 1 pixel.

## Enable/Disable support for printf

Sometimes it's necessary to save some space. Therefore, it's possible to disable `printf` support by commenting out the `TTY_ENABLE_PRINTF` macro.

```c
#define TTY_ENABLE_PRINTF
```

> If `printf` is disabled, text has to be written with `tty_putchar` and `tty_string` function.

---

For a more detailed view of the possible parameters that can be set in `tty.h`, take a look at the [OLED Doxygen documentation (`OLDD`)](https://0x007e.github.io/oled/doxygen/tty_8h.html).
