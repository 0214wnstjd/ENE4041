#include "msp.h"
#include "Clock.h"
#include <stdio.h>

void main(void)
{
    int i;

	Clock_Init48MHz();

	P2->SEL0 &= ~0x07;
	P2->SEL1 &= ~0x07;
	P2->DIR |= 0x07;
	P2->OUT &= ~0x07;

	i=1;
	while (1) {
	    P2->OUT = P2->OUT&(~0xf)|i;
	    printf("%x\n",P2->OUT);
	    printf("%d\n", i);
	    i<<=1;
	    if(i==0x08) i = 1;
	    Clock_Delay1ms(1000);
	}

}

