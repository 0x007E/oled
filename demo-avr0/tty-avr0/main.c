
#define F_CPU 20000000UL

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! IMPORTANT NOTICE                                  !!
// !! Setup following symbols:                          !!
// !! Project-Settings -> AVR/GNU C-Compiler -> Symbols !!
// !!                                                   !!
// !! F_CPU=20000000UL                                  !!
// !! OLED_HAL_PLATFORM=avr0                            !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include <avr/io.h>
#include <util/delay.h>

#include "../../lib/hal/avr0/system/system.h"
#include "../../lib/oled/tty/tty.h"

int main(void)
{
    system_init();
    tty_init();

    PORTA.DIRCLR = 0x0F;
    PORTA.PIN0CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN1CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    
    char temp1 = 0;
    char temp2 = 0;

    while (1)
    {
        if(temp1 < FONT_ASCII_START_CHAR || temp1 > FONT_ASCII_END_CHAR)
        {
            temp1 = ' ';
        }

        if(temp2 < FONT_ASCII_START_CHAR || temp2 > FONT_ASCII_END_CHAR)
        {
            temp2 = '~';
        }
        printf("%c%c", (temp1++), (temp2--));
        _delay_ms(1000);

        if(!(PORTA.IN & PIN0_bm))
        {
            _delay_ms(10);

            for (unsigned char i=0; i < TTY_HEIGHT; i++)
            {
                tty_clear_line(i);
            }

            while(!(PORTA.IN & PIN0_bm));
        }
        else if(!(PORTA.IN & PIN1_bm))
        {
            _delay_ms(10);

            tty_cursor(0, 2);

            while(!(PORTA.IN & PIN1_bm));
        }
        else if(!(PORTA.IN & PIN2_bm))
        {
            _delay_ms(10);

            tty_cursor((TTY_WIDTH>>1), 4);

            while(!(PORTA.IN & PIN2_bm));
        }
        _delay_ms(10);

    }
}

