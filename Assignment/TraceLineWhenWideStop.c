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

void motor_init(void){
    P3->SEL0 &= ~0xC0;
    P3->SEL1 &= ~0xC0;
    P3->DIR |= 0xC0;
    P3->OUT &= ~0xC0;

    P5->SEL0 &= ~0x30;
    P5->SEL1 &= ~0x30;
    P5->DIR |= 0x30;
    P5->OUT &= ~0x30;

    P2->SEL0 &= ~0xC0;
    P2->SEL1 &= ~0xC0;
    P2->DIR |= 0xC0;
    P2->OUT &= ~0xC0;
}

void main(void)
{
    Clock_Init48MHz();

    ir_init();
    motor_init();
    int sensor1, sensor2;
    int speed = 4000;

    while(1){
        P5->OUT |= 0x08;
        P9->OUT |= 0x04;
        P7->DIR = 0xFF;
        P7->OUT = 0xFF;
        Clock_Delay1us(10);
        P7->DIR = 0x00;
        Clock_Delay1us(1600);

        sensor1 = P7->IN & 0x18;
        sensor2 = P7->IN & 0x66;

        if (sensor1 && !sensor2) {
            P5->OUT &= ~0x30;
            P2->OUT |= 0xC0;
            P3->OUT |= 0xC0;
            Clock_Delay1us(speed);

            P2->OUT &= ~0xC0;
            Clock_Delay1us(10000-speed);
        }
        else
        {

            P2->OUT &= ~0xC0;
            Clock_Delay1ms(1000);
        }

        P5->OUT &= ~0x08;
        P9->OUT &= ~0x04;

        Clock_Delay1ms(10);
    }
}

