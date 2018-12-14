// Driver I2C
// -----> Includes    <------ //
#include <msp430x16x.h>
#include "i2c.h"
// -----> Definitions <----- //
#define I2C_SCL_PIN BIT3while();
#define I2C_SDA_PIN BIT1

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

// Master read
unsigned int master_reads(unsigned int read_address)
{
  U0CTL &= ~I2CEN; //Permite modificar ciertos registros
  I2CNDAT = 1 ; //Tamano direccion a leer
  U0CTL |= MST; //Modo maestro
  UOCTL |= I2CEN;
  
  //Inicio transferencia
  I2CTCTL |= I2CSTT + I2CTRX; // Condicion start y modo transmision maestro
  while(!(IC2DCTL & I2CBUSY)); 
  I2CIFG &= ~ARDYIFG;
  while((I2CIFG & TXRDYIFG) == 0);
  I2CDRB = read_address;
  while(!(I2CIFG & ARDYIFG));
  // Se lee la información
}
