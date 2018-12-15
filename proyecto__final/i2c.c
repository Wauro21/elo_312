// Driver I2C
// -----> Includes    <------ //
#include <msp430x16x.h>
#include "i2c.h"
// -----> Definitions <----- //
#define I2C_SCL_PIN BIT
#define I2C_SDA_PIN BIT1
#define BYTE_NUMBER 0x02
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
  unsigned int data = 0x00;
  unsigned int counter = 0x00;
  U0CTL &= ~I2CEN; //Permite modificar ciertos registros
  I2CNDAT = 0x01 ; //Tamano direccion a leer
  U0CTL |= MST; //Modo maestro
  U0CTL |= I2CEN;
  
  //Inicio transferencia
  I2CTCTL |= I2CSTT + I2CTRX; // Condicion start y modo transmision maestro
  while(!(I2CDCTL & I2CBUSY)); //
  I2CIFG &= ~ARDYIFG;
  while((I2CIFG & TXRDYIFG) == 0);
  I2CDRB = read_address;
  while(!(I2CIFG & ARDYIFG));
  // Se lee la información
  I2CNDAT = BYTE_NUMBER;
  U0CTL |= MST; //Se apaga después de una condicion de stop
  I2CTCTL &= !I2CTRX; //Modo recepcion
  I2CTCTL |= I2CSTT + I2CSTP;
  while(!(I2CDCTL & I2CBUSY));
  for(counter = 0; counter < BYTE_NUMBER; counter++)
  {
    while((I2CIFG & RXRDYIFG) == 0);
    data[counter] = I2CDRB;
  }
  while(I2CTCTL & I2CSTP);
  return data;
}
