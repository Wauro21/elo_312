
#include "msp430_version.h"
#include <stdio.h>
#include "display.h"
#include "osc.h"
#include "adc.h"

void main(void)
{
    osc_init_xt2();
    WDTCTL = WDTPW + WDTHOLD;

    display_init();

    CCTL0 = CCIE;             	// CCR0 habilita interrupción del timer
    CCR0 = 32768-1;

    TACTL = TASSEL_1 + MC_1;    // reloj ACLK, modo up to CCR0

    adc_init_single_channel_single_conv();

    _BIS_SR(LPM0_bits + GIE);	// modo bajo consumo

    // Interrupciones
}


#define CONV(x)      x			// convertir x en voltaje

// Timer A0 rutina de servicio de interrupción
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	int pos = display_get_pos();
	int d;
	d = adc_single_channel_single_conv(0);
	printf("Vol=%1.3f\n", CONV(d));
	display_set_pos(pos);
}

#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B (void)
{

}
#

#pragma vector = ADC12_VECTOR
__interrupt void ADC(void)
{
  
}
