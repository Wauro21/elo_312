// Driver I2C
// -----> Includes    <------ //
#include "i2c.h"
// -----> Definitions <----- //
#define IC2_SCL_PIN BIT3
#define IC2_SDA_PIN BIT1


void i2c_init(unsigned int slave_address, int i2c_mode)
{
  P3SEL |= I2C_SCL_PIN | I2C_SDA_PIN;
  U0CTL |= I2C + SYNC; //Inicialiazacion
  UOCTL &= ~I2CEN; // Apaga modulo I2C, para modificar registros
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
