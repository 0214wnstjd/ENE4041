#include "msp.h"
#include "Clock.h"
#include <stdio.h>

void systick_init(void){
    SysTick -> LOAD = 0x00FFFFFF;
    SysTick -> CTRL = 0x00000005;
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
    switch_init();
    systick_init();
    int m =0, s=0, ms=0;

    while(1){
        SysTick->LOAD = 48000;
        SysTick->VAL = 0;
        while((SysTick->CTRL&0x00010000) == 0){
            sw1 = P1->IN & 0x02;
            if(!sw1&&k==0){
                printf("Current Time : %dm %ds %dms\n", m, s, ms);
                k=1;
            }
            if(!!sw1){
                k=0;
            }
        }
        ms++;
        if(ms==1000){
            s++;
            ms=0;
        }
        if(s==60){
            m++;
            s=0;
        }
    }
}

