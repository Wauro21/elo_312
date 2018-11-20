#include "msp430_version.h"
#include <stdio.h>
#include "display.h"
#include "osc.h"

int line = 0;
int sec = 0;
long int timer = 0;



char new_1[] = {BIT3+BIT2+BIT1,BIT4+BIT3+BIT2+BIT1+BIT0,BIT4+BIT3+BIT2+BIT1+BIT0,BIT4+BIT3+BIT2+BIT1+BIT0,BIT4+BIT3+BIT2+BIT1+BIT0,BIT4+BIT3+BIT2+BIT1+BIT0,BIT4+BIT3+BIT2+BIT1+BIT0, BIT3+BIT2+BIT1};
char new_2[] = {BIT3+BIT2+BIT1+BIT0,BIT4+BIT3+BIT2+BIT1,BIT4+BIT3+BIT2,BIT4+BIT3,BIT4+BIT3,BIT4+BIT3+BIT2,BIT4+BIT3+BIT2+BIT1, BIT3+BIT2+BIT1+BIT0};
char new_3[] = {0x00, BIT2, BIT2, BIT3 + BIT2 +BIT1, BIT3+BIT2+BIT1, BIT2, BIT2, 0x00};
char new_4[] = {0x00,0x00,0x00, BIT2, BIT3 + BIT2 +BIT1, BIT2, 0x00,0x00};
char new_5[] = {0x00,0x00,0x00,0x00, BIT2, 0x00, 0x00, 0x00};

void display_clock(void)
{
   int time = 0;
   int i;
   while(1)
      { 
		char c1, c2, c3, c4;
		if (time%6==0){
		  c1=0x02; c2=0x03; c3=0x04; c4=0x05;}
		else if (time%6==1){
		  c1=' '; c2=0x01; c3=0x04; c4=0x05;}
		else if (time%6==2){
		  c1=' '; c2=0x02; c3=0x04; c4=0x05;}
		else if (time%6==3){
		  c1=' '; c2=' '; c3=0x01; c4=0x05;}
		else if (time%6==4){
		  c1=' '; c2=' '; c3=0x02; c4=0x05;}
		else if (time%6==5){
		  c1=' '; c2=' '; c3=' '; c4=0x01;}
		
		int min = time/60;
        int sec = time%60;
        display_clear();
        printf("     %02d:%02d", min, sec);
        putchar('\n');
		
        for (i=0;i<10;i++)
		{
			udelay(10000);
		}
		
//		display_set_pos(0x40 + (unsigned char)t);
		printf("%c%c%c%c",c1,c2,c3,c4);
		display_set_pos(0x50);
		for (i=0;i<90;i++)
		{
			udelay(10000);
		}
		//display_set_pos(0x50);
		time += 1;
		pos+=1;

      }
}


void main(void)
{
	osc_init_xt2();
	WDTCTL = WDTPW + WDTHOLD;
	TBCTL = TBSSEL_2 + MC_1 + ID_3;
	TBCCR0 = 0x0001;
	display_init();
	TBR=0;
	udelay(64);
	
	TACCTL0 = CCIE;              // CCR0 habilita interrupción del timer
	TACCR0 = 32768 >> 2;

	TACTL = TASSEL_1 + MC_1;   // reloj auxiliar, modo up to CCR0
//	display_test_Write_CGRAM_MS();
	display_set_pos(0x00);
	
	display_new_character(0x01,new_1);
	display_new_character(0x02,new_2);
	display_new_character(0x03,new_3);
	display_new_character(0x04,new_4);
	display_new_character(0x05,new_5);
//	printf("LAB ELO312");
//	putchar('\n');
//	printf("%c",0x01);
	
	display_clock();


	
	_BIS_SR(LPM0_bits + GIE);  // modo bajo consumo


    // interrupciones
}

// Timer A0 rutina de servicio de interrupción
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	P2OUT ^= 0x1;
}


#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
	if (timer < 1000000)
	  timer+=1;
	else
	  timer = 0;	
}