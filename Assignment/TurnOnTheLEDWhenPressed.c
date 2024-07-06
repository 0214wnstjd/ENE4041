#include "msp.h"
#include "Clock.h"
#include <stdio.h>

void led_init(){
      P2->SEL0 &= ~0x07;
      P2->SEL1 &= ~0x07;
      P2->DIR |= 0x07;
      P2->OUT &= ~0x07;
}

void turn_on_led(){
    P2->OUT |= 0x07;
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
    Clock_Init48MHz();
    led_init();
    switch_init();
    while(1){
        sw1 = P1->IN & 0x02;
        if(!sw1){
            turn_on_led();
        }
        if(!!sw1){
            turn_off_led();
        }
        Clock_Delay1ms(100);
    }
}

