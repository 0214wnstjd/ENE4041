#include "msp.h"
#include "Clock.h"
#include <stdio.h>

void led_init(){
      P2->SEL0 &= ~0x07;
      P2->SEL1 &= ~0x07;
      P2->DIR |= 0x07;
      P2->OUT &= ~0x07;
}

void turn_on_led(int color){
    P2->OUT = P2->OUT&(~0xf)|color;
}
void turn_off_led(){
    P2->OUT &= ~0x07;
}
void switch_init(){
    P1->SEL0 &= ~0x12;
    P1->SEL1 &= ~0x12;
    P1->DIR &= ~0x12;
    P1->REN |= 0x12;
    P1->OUT |= 0x12;
}
void main(void)
{
    int sw1;
    int i=1, k=0;
    Clock_Init48MHz();
    led_init();
    switch_init();
    turn_on_led(i);

    while(1){
        sw1 = P1->IN & 0x02;
        if(!sw1&&k==0){
            i<<=1;
            if(i==0x08) i = 1;
            turn_on_led(i);
            k=1;
        }
        if(!!sw1){
            k=0;
        }
        Clock_Delay1ms(100);
    }
}

