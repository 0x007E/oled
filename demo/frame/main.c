
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
	
	position.x = 100;
	position.y = 35;
	
	unsigned int value = 1337;
	frame_draw_number(&value, 4, NUMBER_Unsigned_Int, NUMBER_Decimal, position);
	
	// Short version:
	//frame_draw_number(&(unsigned int){16}, 4, NUMBER_Unsigned_Int, NUMBER_Decimal, position);
	
	position.x = 75;
	position.y = 35;
	frame_draw_number_int(-116, 4, NUMBER_Decimal, position);
	
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
		position.y = 46;
		frame_draw_number_uint(temp, 3, NUMBER_Decimal, position);
		
		_delay_ms(50);
	}
}

