
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
    system_init();
    frame_init();

    GFX_Point position = { 106, 2 };
    GFX_Size size = { 10, 10 };
    
    frame_draw_text("Init", position);
    
    position.x = 100;
    position.y = 35;
    
    unsigned int value = 1337;
    frame_draw_number(&value, 4, NUMERIC_Unsigned_Int, NUMERIC_Decimal, position);
    
    // Short version:
    //frame_draw_number(&(unsigned int){16}, 4, NUMERIC_Unsigned_Int, NUMERIC_Decimal, position);
    
    position.x = 75;
    position.y = 35;
    frame_draw_number_int(-116, 4, NUMERIC_Decimal, position);
    
    _delay_ms(2000);
    
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
        position.y = 47;
        frame_draw_number_uint(temp, 3, NUMERIC_Decimal, position);
        
        _delay_ms(50);
    }
}

