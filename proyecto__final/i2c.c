// Driver I2C
// -----> Includes    <------ //
#include <msp430x16x.h>
#include "i2c.h"

// -----> Definitions <----- //
#define I2C_SCL_PIN BIT3
#define I2C_SDA_PIN BIT1
#define BYTE_NUMBER 0x02
// ----> Variables utiles <---- //
static unsigned int PWM[2];
static unsigned int pwm_counter=0x00;
//Funcion incializacion I2C
void i2c_init(unsigned int slave_address, int i2c_mode)
{
  P3SEL |= I2C_SCL_PIN | I2C_SDA_PIN;
  U0CTL |= I2C + SYNC; //Inicialiazacion
  U0CTL &= ~I2CEN; // Apaga modulo I2C, para modificar registros
  I2CTCTL |= I2CSSEL1; //Reloj SMCLK
  I2CIE = RXRDYIE | TXRDYIE; // Interrupciones reception/transmision
  if(i2c_mode)
  {
    I2CNDAT = 0x02; //Cantidad de bytes transferidos
    I2CSA   = slave_address; //Direccion slave
  }
  else
  {
    I2COA = slave_address; //Direccion propia
  }
  
  U0CTL |= I2CEN; //Activa modulo IC2
}

//Slave read
unsigned int* slave_reads(unsigned int* save)
{
  unsigned int count = 0x00;
  for(count = 0x00; (count < 6); count++)
  {
    if(I2CDRB%3 != 0)
    {
      if((count == 0x01) | (count == 0x04))
      {
        *(PWM+count) = (I2CDRB<<8);
      }
      else
      {
        *(PWM + count) |= I2CDRB;
      }
    }
  }
}
