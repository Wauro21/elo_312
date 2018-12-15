#ifndef __I2C__H
#define __I2C__H

// ----> Definiciones <---- //

#define I2C_MASTER 0x01
#define I2C_SLAVE  0x00

// ---> Funciones < ---- //

void i2c_init(unsigned int slave_address, int i2c_mode);
unsigned int* master_reads(unsigned int read_address);
bool i2c_is_busy();
#endif
