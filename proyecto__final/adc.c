/*****************
* Nombre del modulo: adc.c
*
* Modulo creado para la asignatura Elo312
* Laboratorio de Estructura de Computadores
* del departamento de Electrónica de la Universidad
* Técnica Federico Santa María. El uso o copia
* está permitido y se agracede mantener el nombre 
* de los creadores.
*
* Escrito inicialmente el 01/01/2004 Por Michael Kusch & Wolfgang Freund
* Modificado el 24/09/2014           Por Andrés Llico.
* Modificado el 31/10/2017           Por Mauricio Solís
*Modificado el 14/12/2017            Por 
* Descripción del módulo:
* Modulo driver adc
* Contiene las funciones que permiten manejar el ADC
*****************/

/*  Include section
*
*****************/
#include "adc.h"
#include <stdio.h>


/*  Defines section
*
*****************/
#define CONV(x)      (3.3*x)/4095.0

/*  Local Function Prototype Section
*
*****************/

/*  Global Variables Section
*
*****************/
extern int data[2];


////////////////////////////////////////
//       RUTINAS IMPLEMENTADAS        //
//                                    //

/******************
* Nombre    		: int adc_single_channel_single_conv(int arg1)
* returns			: Resultado de la conversión
* arg1				: Canal a convertir
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Realiza una conversión simple
de un solo canal.
******************/
int adc_single_channel_single_conv(int ch)
{
	ADC12CTL0 &= ~ENC;
	ADC12MCTL0 &= 0xF0; 		// Se limpia canal anterior
	ADC12MCTL0 |= (ch & 0x0F);  // Selecciona canal a convertir
	ADC12CTL0 |= ENC;           // habilita conversión
	ADC12CTL0 |= ADC12SC;
	while (ADC12CTL0 & ADC12SC);
	
	return ADC12MEM0;    		// Los resultados de la conversión
}

/******************
* Nombre    		: void adc_init_single_channel_single_conv(void)
* returns			: void
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Inicializa el ADC en modo de
conversión simple de un solo canal.
******************/
void adc_init_single_channel_single_conv()
{
    P6SEL |= 0xFF;
    P6DIR &= 0x00;
    ADC12CTL0 &= ~ENC;
    ADC12CTL0 = SHT1_15 | SHT0_15 | ADC12ON;
    ADC12CTL1 = CSTARTADD_0 | SHS_0 | SHP | ADC12DIV_7 | ADC12SSEL_2 | CONSEQ_0;
    //CONSEQx = 1 => MODO DEL ADC12: Secuencia de canales.
    ADC12MCTL0 = SREF_0;
    ADC12IE = 0x00; // deshabilitar interrupciones
}





/******************
* Nombre    		: __interrupt void adc_ADC12_interrupt(void)
* returns			: void
* Creada por		: Mauricio Solis
* Fecha creación	: 01/01/2013
* Descripción		: Vector de interrupciones del
ADC.
******************/
#pragma vector = ADC12_VECTOR
		
__interrupt void adc_ADC12_interrupt(void)
{
    data[0]=ADC12MEM0;
   
}
  
//                                    //
////////////////////////////////////////
