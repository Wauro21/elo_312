#include "msp430_version.h"
#include <stdio.h>
#include "display.h"
#include "osc.h"
#include "adc.h"

#define ARR_LEN 10

int muestras[ARR_LEN] = {0};
unsigned int count_muestras = 0;

volatile float promedio = 0;
int d;

void main(void)
{
    osc_init_xt2();
    WDTCTL = WDTPW + WDTHOLD;

    display_init();

    CCTL0 = CCIE;             	// CCR0 habilita interrupción del timer
    CCR0 = 32768-1;
    TACTL = TASSEL_1 + MC_1;    // reloj ACLK, modo up to CCR0
	
	_BIS_SR(GIE);
    
	adc_init_single_channel_single_conv_it();
	adc_single_channel_single_conv_it(0);

    _BIS_SR(LPM0_bits);	// modo bajo consumo

    // Interrupciones
}


#define CONV(x)      (3.3/4095.0)*x			// convertir x en voltaje
 
//Timer A0 rutina de servicio de interrupción
//#pragma vector = TIMERA0_VECTOR
//__interrupt void Timer_A (void)
//{
//	int pos = display_get_pos();
//	int d;
//	printf("Vol=%1.3f\n", CONV(d));
//	display_set_pos(pos);
//}

//#pragma vector = TIMERB0_VECTOR
//__interrupt void Timer_B (void)
//{

//}

#pragma vector = ADC12_VECTOR
__interrupt void ADC(void) //Cuando se escribe un valor en la memoria de ADC, se llama a la rutina y se envia el resultado a la pantalla
{
  	float dato = ADC12MEM0;
  	muestras[count_muestras] = dato;
	float otra= CONV(dato);
	count_muestras = (count_muestras+1)%ARR_LEN;	
	int pos = display_get_pos();
	printf("Vol=%1.3f\n", CONV(dato));
	display_set_pos(pos);
	
	adc_single_channel_single_conv_it(0);
}
