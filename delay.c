#include <intrins.h>
void Delay(unsigned int xms)		//@11.0592MHz
{
	unsigned int temp=0;
	for(temp=0;temp<xms;temp++)
		{
			unsigned char i;

			_nop_();
			_nop_();
			i = 1;
			while (--i);
		}
}
