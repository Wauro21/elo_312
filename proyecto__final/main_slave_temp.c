//******************************************************************************
//   MSP-FET430P140 Demo - I2C, Slave Transmits to MSP430 Master
//
//   Description: This demo connects two MSP430's via the I2C bus.  The master
//   reads from the slave. This is the slave code. The master code is called
//   fet140_i2c_04.c. The TX data begins at 0 and is incremented
//   each time it is sent. The master checks the data it receives for validity.
//   If it is incorrect, it stops communicating and the P1.0 LED will stay on.
//   The TXRDYIFG interrupt is used to know when to TX.
//   Run the code in Slave device before starting the Master device for proper
//   communication
//   ACLK = n/a, MCLK = SMCLK = I2CCLOCK = DCO ~ 800kHz
//  //* MSP430F169 Device Required *//
//
//                                 /|\  /|\
//                  MSP430F169     10k  10k     MSP430F169
//                    slave         |    |        master
//              -----------------|  |    |  -----------------
//             |             P3.1|<-|---+->|P3.1             |
//             |                 |  |      |             P1.0|-->LED
//             |                 |  |      |                 |
//             |             P3.3|<-+----->|P3.3             |
//             |                 |         |                 |
//
//
//  M. Mitchell
//  Texas Instruments Inc.
//  Feb 2005
//  Built with IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include  "msp430_version.h"
#include "i2c.h"
#include "osc.h"
#include <stdio.h>
static unsigned int count = 0x00;
static unsigned int PWM[6];
char TXData = 0xF0;
char read_address = 0; 
void main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  osc_init_xt2();
  display_init();
  i2c_init(0x48, I2C_SLAVE);
  display_send_data(0x30);
  P4DIR |= BIT1;
  P6SEL |= 0X01;
  P4SEL |= BIT1;
  TBCTL = TBSSEL_1 + MC_1;
  TBCCTL1 = OUTMOD_7;
  TBCCR0 = 654;
  TBCCR1 = 19;
  //display_set_pos(0x00);
  //printf("hola");
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}


// Common ISR for I2C Module
#pragma vector=USART0TX_VECTOR
__interrupt void I2C_ISR(void)
{
  
  int count = 0;
  int index = 0;
 switch( I2CIV )
 {
   case  2: break;                          // Arbitration lost
   case  4: break;                          // No Acknowledge
   case  6: break;                          // Own Address
   case  8: break;                          // Register Access Ready
   case 10: 
     //while(!((I2CIFG & RXRDYIFG)==RXRDYIFG));
     slave_reads(PWM);
     if(PWM[0] != 0x00)
     {
       display_set_pos(0x00);
       display_send_data(PWM[0]);
     }
     TBCCR1=(19 + ((PWM[1]<<8)|PWM[0])*(3.3/4095)*65);
     //asm("NOP");
//     if(((I2CDRB != 0x00)&&(I2CDRB != 0x02)))
//     {
//      *(data+counter) = I2CDRB;
//      counter++;
//      if(counter == 0x02)
//      {
//        asm("NOP");
//      }
//     }
//     if (index == 0x01)
//     {if (count ==0)
//     {PWM[index-1] = I2CDRB;}
//       if (count == 1)
//       {PWM[index] += I2CDRB<<8;}
//       if (count == 2)
//       {count = 0;}
//     count++;}
//     
//     if (index == 0x2){
//       if (count == 0)
//       {PWM[index-1] = I2CDRB;}
//       if (count == 1)
//       {PWM[index-1] += I2CDRB<<8;}
//       if (count == 2)
//       {count =0;}
//     count++;}
//           
       
           
      
          
//     if(count == 3 )
//     {
//       count == 0;
//     }
     break;                          // Receive Ready
   case 12:                                 // Transmit Ready
     //I2CDRB = TXData++;                     // Load I2CDRB and increment
     //count++;
     break;
   case 14: break;                          // General Call
   case 16: 
     //slave_reads(PWM);
     break;                          // Start Condition
 }
}

