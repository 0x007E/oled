---
layout: post
title: frame.h - FRAME parameter definition
categories: [Parameters]
introduction: "Descriptions of parameters that can be adapted in the frame header"
---

The FRAME `frame.h` library renders pixel per page. It is possible to draw text/numbers or a bargraphs onto the display.

> When text/numbers or bar charts are rendered between two pages, it is not possible to write anything else vertically on that pages (except background, which is rendered at all times!).

![FRAME Text 2 Pages]({{ '/assets/images/display-frame-text-2pages.png' | relative_url }})

The filled areas in the image above and below the text can no longer be written on.

## Width/Height of the TTY

The `FRAME_WIDTH` and `FRAME_HEIGHT` is normally set to the full width and height of the display but can be defined by user if necessary.

![FRAME full frame]({{ '/assets/images/display-frame-full-frame.png' | relative_url }})

```c
#define FRAME_WIDTH OLED_COLUMN_SIZE
#define FRAME_HEIGHT OLED_ROW_SIZE
```

## Specific background of the FRAME

It is possible to set up a standard background with the `FRAME_SPECIFIC_BACKGROUND` macro that is renderd into every function call in the `frame.h`. So a persisent background gets overlayed over every text/number or bar write to the display (is set by default).

> This uses `1 KiB` of program space!

![FRAME background]({{ '/assets/images/display-frame-background.png' | relative_url }})

```c
#define FRAME_SPECIFIC_BACKGROUND
```

> By commenting out the macro, the background gets deactivated!

## Enable/Disable support for floating point numbers

Activating the macro `FRAME_NUMBER_ENABLE_FLOATING_POINT` enables the output of floating point numbers  (not enabled by default).

```c
#define FRAME_NUMBER_ENABLE_FLOATING_POINT
```

> The use of floating point numbers increases program memory consumption!

### Set floating point number prescision

If floating-point numbers are enabled (`FRAME_NUMBER_ENABLE_FLOATING_POINT`) it is possible to set up the number precision. This can be done with the `FRAME_NUMBER_PRECISION` macro.

```c
#define FRAME_NUMBER_PRECISION
```

> Increasing the precision allows for more detailed numbers, but may increase program memory usage and processing time.

---

For a more detailed view of the possible parameters that can be set in `frame.h`, take a look at the [OLED Doxygen documentation (`OLDD`)](https://0x007e.github.io/oled/doxygen/frame_8h.html).
