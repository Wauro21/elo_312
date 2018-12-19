// Driver I2C - Basado en el driver creado por el Profesor Mauricio Solis.
// -----> Includes    <------ //
#include <msp430x16x.h>
#include "i2c.h"

// -----> Definitions <----- //
#define I2C_SCL_PIN BIT3
#define I2C_SDA_PIN BIT1
#define BYTE_NUMBER 0x02
// ----> Variables utiles <---- //
static contador_save = 0x00;
// Funcion incializacion I2C
void i2c_init(unsigned int slave_address, int i2c_mode)
{
  P3SEL |= I2C_SCL_PIN | I2C_SDA_PIN;
  U0CTL |= I2C + SYNC; //Inicialiazacion
  U0CTL &= ~I2CEN; // Apaga modulo I2C, para modificar registros
  I2CTCTL |= I2CSSEL1; //Reloj SMCLK
  I2CIE = STTIFG | RXRDYIE | ARDYIFG; // Interrupciones reception/transmision
  if(i2c_mode)
  {
    I2CNDAT = 0x03; //Cantidad de bytes transferidos
    I2CSA   = slave_address; //Direccion slave
  }
  else
  {
    I2COA = slave_address; //Direccion propia
    //I2CDCTL &= ~I2CRXOVR;
  }
  
  U0CTL |= I2CEN; //Activa modulo IC2
}

//Slave read
void slave_reads(unsigned int* save, unsigned int address)
{
  *(save + address) = I2CDRB;
  if(save_count == 0x01)
  {
    save_count = 0;
  }
  else
  {
    save_count = save_count+ 0x01;
  } 
}

// PWM 
void pwm_change(unsigned int * PWM)
{
  TBCCR1=(19 + ((PWM[2]<<8)|PWM[1])*(3.3/4095)*65);
}
