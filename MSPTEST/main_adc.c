#include "msp430_version.h"
#include <stdio.h>
#include "osc.h"
#include "adc.h"
#define CONV(x)     (*(x/4095.0))
void main(void)
{
  osc_init_xt2();
  WDCTL = WDTPW + WDTHOLD;
  
  _BIS_SR(GIE + LPM0_bits); //ultima linea main
  
}
//Interrupciones
 #pragma vector = TIMERB0_VECTOR
 __interrupt void TIMER_B(void=
 {
  adc_single_channel_single_conv_it(0); //Pin 0, puerto 6
 }
 
 #pragma vector = ADC12VECTOR
 __interrupt void ADC(void) 
 {
  TACCR1 = CONV(ADC12MEM0);
 }
