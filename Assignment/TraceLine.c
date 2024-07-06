#include "msp.h"
#include "Clock.h"
#include "stdio.h"

void systick_init(void){
    SysTick -> LOAD = 0x00FFFFFF;
    SysTick -> CTRL = 0x00000005;
}
void systick_wait1ms(){
    SysTick->LOAD =48000;
    SysTick->VAL = 0;
    while((SysTick->CTRL & 0x00010000)==0){}
}
void systick_wait1s(){
    int i;
    int count = 1000;
    for(i=0; i< count; i++){
        systick_wait1ms();
    }
}
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

    pwm_init34(7500, 0, 0);
}

void pwm_init34(uint16_t period, uint16_t duty3, uint16_t duty4){
    TIMER_A0->CCR[0] = period;
    TIMER_A0->EX0 = 0x0000;

    TIMER_A0->CCTL[3] = 0x0040;
    TIMER_A0->CCR[3] = duty3;
    TIMER_A0->CCTL[4] = 0x0040;
    TIMER_A0->CCR[4] = duty4;

    TIMER_A0->CTL = 0x02F0;

    P2->DIR |= 0xC0;
    P2->SEL0 |= 0xC0;
    P2->SEL1 &= ~0xC0;
}

void move(uint16_t leftDuty, uint16_t rightDuty){
    P3->OUT |= 0xC0;
    TIMER_A0->CCR[3] = leftDuty;
    TIMER_A0->CCR[4] = rightDuty;
}

void left_forward(){
    P5->OUT &= ~0x10;
}

void left_backward(){
    P5->OUT |= 0x10;
}

void right_forward(){
    P5->OUT &= ~0x20;
}

void right_backward(){
    P5->OUT |= 0x20;
}


void main(void)
{
    Clock_Init48MHz();
    systick_init();
    ir_init();
    motor_init();
    int sensor1, sensorleft, sensorright;

//    while (1) {
//        left_forward();
//        right_forward();
//        move(2000, 2000);
//        systick_wait1s();
//
//        left_forward();
//        right_forward();
//        move(1000, 9000);
//        systick_wait1s();
//    }

    while(1){
        P5->OUT |= 0x08;
        P9->OUT |= 0x04;
        P7->DIR = 0xFF;
        P7->OUT = 0xFF;
        Clock_Delay1us(10);
        P7->DIR = 0x00;
        Clock_Delay1us(1600);

        sensor1 = P7->IN & 0x18;
        sensorleft = P7->IN & 0x60;
        sensorright = P7->IN & 0x06;
        if (sensorleft && !sensorright){
            left_forward();
            right_forward();
            move(1500, 500);
        }
        else if (sensorright && !sensorleft){
            left_forward();
            right_forward();
            move(500,1500);
        }
        else if (sensor1) {
            left_forward();
            right_forward();
            move(1000, 1000);
        }

        P5->OUT &= ~0x08;
        P9->OUT &= ~0x04;

        Clock_Delay1ms(10);
    }
}

