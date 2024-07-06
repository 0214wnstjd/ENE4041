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
void turn_on_led(int color){
    P2->OUT = P2->OUT&(~0xf)|color;
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
    led_init();
    int sensorcenter, sensorleftInner, sensorleftOuter, sensorrightInner, sensorrightOuter, sensorleftmost, sensorrightmost;
    int lab = 0;
    int statusLR = 0; //0: straight, 1: left, 2: right
    int statusAll = 0;
    int statusTurnComplete = 0;
    int labplus= 0;

    while(1){

        P5->OUT |= 0x08;
        P9->OUT |= 0x04;
        P7->DIR = 0xFF;
        P7->OUT = 0xFF;
        Clock_Delay1us(10);
        P7->DIR = 0x00;
        Clock_Delay1us(1600);

        sensorcenter = P7->IN & 0x18;
        sensorleftInner = P7->IN & 0x20;
        sensorleftOuter = P7->IN & 0x40;
        sensorrightInner = P7->IN & 0x04;
        sensorrightOuter = P7->IN & 0x02;
        sensorleftmost = P7->IN & 0x80;
        sensorrightmost = P7->IN & 0x01;

        turn_on_led(labplus);

        if(sensorcenter){
            if(statusTurnComplete == 1 && (!sensorleftInner || !sensorrightInner) && !sensorleftOuter && !sensorrightOuter && !sensorleftmost && !sensorrightmost){
                statusLR = 0;
                statusTurnComplete = 0;
            }

            if (sensorleftInner && sensorleftOuter && sensorrightInner && sensorrightOuter && !sensorleftmost && !sensorrightmost){
                labplus = 1;
                if(lab == 1){
                    move(0,0);
                    return;
                }
            }

            else if (sensorleftInner && sensorleftOuter && statusAll == 0){
                if(sensorrightInner && sensorrightOuter && sensorleftmost && sensorrightmost){
                    statusLR = 1;
                    statusAll = 1;
                }
                else if(!sensorrightOuter){
                    statusLR = 1; //90도 left
                }
            }
            else if (sensorrightInner && sensorrightOuter && statusAll == 0){
                if(!sensorleftOuter){

                    statusLR = 2; //90도 right
                }
            }
            else if(sensorleftInner && !sensorleftOuter){
                left_forward();
                right_forward();
                move(2300, 500);
            }
            else if(sensorrightInner && !sensorrightOuter){
                left_forward();
                right_forward();
                move(800, 2100);
            }
            else{
            left_forward();
            right_forward();
            move(2300, 2250);
            }
        }
        else if(!sensorcenter){
            if (statusLR == 1){
                left_backward();
                right_forward();
                move(2500, 2450);
                statusAll = 0;
                statusTurnComplete = 1;
            }
            else if (statusLR == 2){
                if(labplus == 1) {
                    labplus = 0;
                    lab ++;
                }
                right_backward();
                left_forward();
                move(2400,2350);
                statusTurnComplete = 1;

            }
        P5->OUT &= ~0x08;
        P9->OUT &= ~0x04;

        Clock_Delay1ms(5);
    }
    }
}


