---
layout: post
title: font.h - FONT parameter definition
categories: [Parameters]
introduction: "Descriptions of parameters that can be adapted in the font header"
---

The font header `font.h` defines parameters to setup the size of a character/number or symbol, if only uppercase letters should be used and where font data should be stored.

![FONT characters]({{ '/assets/images/font-characters.png' | relative_url }})

## Width/Height of the FONT

The `FONT_WIDTH` and `FONT_HEIGHT` macro defines the width and the height of the font. The installed font has a size of (`w=5*h=7`). The font itself should not exceed `8` in height to prevent using more than one page!

```c
#define FONT_WIDTH 5
#define FONT_HEIGHT 7
```

### Characters in bold (`8x8`)

![FONT bold character]({{ '/assets/images/font-bold-character.png' | relative_url }})

Within the [OLED Graphics Designer (`OLGD`)](https://0x007e.github.io/oled-designer/) it is possible to create own characters in different sizes. Just start a new design with `8x8` and design your own character.

> It is important that all characters, numbers and symbols have the same size!

## Ignore lowercase letters

To save memory or if lower case letters are not needed they can be disabled with the ´FONT_IGNORE_LOWER_CASE` macro (not enabled by default). Every lower case letter is automatically converted to an upper case letter.

> To do this, simply remove the comment before the macro.

```c
#define FONT_IGNORE_LOWER_CASE
```

## Store font in EEPROM

It is possible to move the font data from programm memory to eeprom. This can be done with the `FONT_STORE_IN_EEPROM` macro (not enabled by default).

> To do this, simply remove the comment before the macro.

```c
#define FONT_STORE_IN_EEPROM
```

> There should be `470 bytes` left in EEPROM for all characters.

---

For a more detailed view of the possible parameters that can be set in `font.h`, take a look at the [OLED Doxygen documentation (`OLDD`)](https://0x007e.github.io/oled/doxygen/font_8h.html).
