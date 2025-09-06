
#define F_CPU 12000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "../../lib/oled/tty/tty.h"

int main(void)
{
    DDRA = 0x00;
    PORTA = 0x07;

	tty_init();
	
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

        if(!(PINA & (1<<PINA0)))
        {
            _delay_ms(10);

            for (unsigned char i=0; i < TTY_HEIGHT; i++)
            {
                tty_clear_line(i);
            }

            while(!(PINA & (1<<PINA0)));
        }
        else if(!(PINA & (1<<PINA1)))
        {
            _delay_ms(10);

            tty_cursor(0, 2);

            while(!(PINA & (1<<PINA1)));
        }
        else if(!(PINA & (1<<PINA2)))
        {
            _delay_ms(10);

            tty_cursor((TTY_WIDTH>>1), 4);

            while(!(PINA & (1<<PINA2)));
        }
        _delay_ms(10);

	}
}

