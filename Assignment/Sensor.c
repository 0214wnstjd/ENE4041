#include "msp.h"
#include "Clock.h"
#include "stdio.h"

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

void ir_init(){
    P5->SEL0 &= ~0x08;
    P5->SEL1 &= ~0x08;
    P5->DIR |= 0x08;
    P5->OUT &= ~0x08;

    P9->SEL0 &= ~0x04;
    P9->SEL1 &= ~0x04;
    P9->DIR |= 0x04;
    P9->OUT &= ~0x04;

    P7->SEL0 &= ~0xFF;
    P7->SEL1 &= ~0xFF;
    P7->DIR &= ~0xFF;
}

void main(void)
{
    Clock_Init48MHz();
    led_init();
    ir_init();
    int sensor;

    while(1){
        P5->OUT |= 0x08;
        P9->OUT |= 0x04;
        P7->DIR = 0xFF;
        P7->OUT = 0xFF;
        Clock_Delay1us(10);
        P7->DIR = 0x00;
        Clock_Delay1us(1800);

        sensor = P7->IN & 0x18;
        if (sensor) {
            P2->OUT |= 0x01;
        }
        else
        {
            P2->OUT &= ~0x07;
        }

        P5->OUT &= ~0x08;
        P9->OUT &= ~0x04;

        Clock_Delay1ms(10);
    }
}

