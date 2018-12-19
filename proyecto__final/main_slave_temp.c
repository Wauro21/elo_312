// -> Includes <- //

#include  "msp430_version.h"
#include "i2c.h"
#include "osc.h"

// -> Define <- //
#define data_limit  3


// -> Variables Estaticas <- //
static unsigned int count = 0x00; // Contados para circular por arreglo
static unsigned int PWM[3]; // Arreglo donde se guardara la conversion
char read_address = 0; 

// -> Inicio Programa

void main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Desactiva WTD
  osc_init_xt2();                           // Inicializa el oscilador
  i2c_init(0x48, I2C_SLAVE);                // Inicializa I2C -> Address Slave 0x48
  P4DIR = BIT1;                             // Salida PWM / TIMER_B                           
  P4SEL |= BIT1;                            // Pin -> Periferico
  TBCTL = TBSSEL_1 + MC_1;                  // ACLK = 32768 / Modo UP
  TBCCTL1 = OUTMOD_7;                       // Modo RESET/SET
  TBCCR0 = 654;                             // Frecuencia = 60
  TBCCR1 = 19;                              // Limite inferior PWM
  _BIS_SR(LPM0_bits + GIE);                 // Habilita interrupciones y CPU en bajo consumo
}


#pragma vector=USART0TX_VECTOR
__interrupt void I2C_ISR(void)
{
 switch( I2CIV )                            // Se analiza el vector de interrupcion de I2C
 {
   case 10:                                 // Se verifica flag de recepcion de datos -> RXRDYIE
     if(count < data_limit)                          
     {
       PWM[count] = I2CDRB;                 // Se registra el valor recibido
       count = count + 1;                   //Se aumenta el contador para el array
     }
     else
     {
       count = 0;                           //Si se excede el maximo del array, contador vuelve a cero
     }
      pwm_change(PWM);                      //Modificacion de duty cycle
     break;                     
 }
}

