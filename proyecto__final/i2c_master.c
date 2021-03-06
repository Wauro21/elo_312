/**
 * @file i2c.c
 * @author Mauricio Solis
 * @date 07/12/2017
 * @brief API implementation of i2c.h

 * This File was created by Mauricio Solis in order to make more easy
 * the use of I2C of the MSP430F1612.
 * Please use it with caution and responsability.
*/
/*--->> Includes <<-----------------------------------------------------------*/
#include <msp430x16x.h>
#include "i2c.h"
#include <string.h>
#include <stdlib.h>

/*--->> Definition <<---------------------------------------------------------*/
#define I2C_SCL_PIN		BIT3
#define I2C_SDA_PIN		BIT1

#define I2C_DISABLE_INTERRUPTS	(I2CIE = 0x00)
#define I2C_ENABLE_INTERRUPTS	(I2CIE = TXRDYIE | RXRDYIE | ARDYIE | NACKIE)

#define DEVICE_ADDRESS_LENGHT	1
#define REGISTER_LENGTH			1


/*--->> Private Prototypes Functions <<---------------------------------------*/
/*--->> Private Functions <<--------------------------------------------------*/

static t_i2c_usm hi2c;
/**
 * API: i2c_init
 */
bool i2c_init(uint8_t slave_address, e_i2c_ms_mode i2c_ms_mode)
{
	P3SEL |= I2C_SCL_PIN | I2C_SDA_PIN;		// Select I2C pins
	U0CTL |= I2C + SYNC;					// Switch USART0 to I2C mode
	U0CTL &= ~I2CEN;                       	// Disable I2C
 	I2CTCTL = I2CSSEL_2;					// SMCLK Do not use ACLK, there is an issue when NO LPM.
	//this sets the I2C clk up to 100[KHz]
	I2CPSC = I2C_PSC_DIV_BY_08;				// Divider by 8
	I2CSCLH = 0x03;							// this value can not be less than 0x03// high period = (3 + 2) x I2CPSC
	I2CSCLL = 0x03;							// this value can not be less than 0x03// low  period = (3 + 2) x I2CPSC
	
	I2CSA = slave_address;					// Slave address
	I2C_DISABLE_INTERRUPTS;
	U0CTL |= I2CEN;							// Enable I2C
	
	if(i2c_ms_mode == I2C_MODE_MASTER)
	{
		U0CTL |= MST;							// Master
	}
	else
	{
		#warning THE SLAVE MODE IS NOT PART OF THIS DRIVER
		U0CTL &= ~MST;							// Slave
	}
	
	return true;
}

/**
 * API: i2c_master_write
 */
bool i2c_master_write(uint8_t register_address, uint8_t* data, uint8_t data_len)
{
	uint8_t counter = 0;
	if(i2c_is_busy())
	{
		return false;
	}
	
	U0CTL &= ~I2CEN;
	I2CNDAT = data_len + DEVICE_ADDRESS_LENGHT;
	U0CTL |= MST;
	U0CTL |= I2CEN;
	I2CTCTL |= I2CSTT + I2CTRX + I2CSTP;					// Initiate transfer
	
	while ((I2CIFG & TXRDYIFG) == 0);         				// Wait for transmitter to be ready
	I2CDRB = register_address;								// Register Address to read
	
	for(counter = 0; counter < data_len; counter++)
	{
		while ((I2CIFG & TXRDYIFG) == 0);         // Wait for Receiver to be ready
		I2CDRB = data[counter];
	}
	//while(i2c_is_busy());
    while(I2CTCTL & I2CSTP);//wait until the STOP be cleared.
	return true;
}

/**
 * API: i2c_master_read
 */
bool i2c_master_read(uint8_t register_address, uint8_t* data, uint8_t data_len)
{
	uint8_t counter = 0;
	if(i2c_is_busy())
	{
		return false;
	}
	
	U0CTL &= ~I2CEN;
	I2CNDAT = 1;
	U0CTL |= MST;
	U0CTL |= I2CEN;
	
	//first: set the address to read by write it.
	I2CTCTL |= I2CSTT + I2CTRX;					// Initiate transfer
	
	while(!i2c_is_busy());						//wait until the I2C starts to work.
	
	I2CIFG &= ~ARDYIFG;							//put ARDYIFG flag down.
	while ((I2CIFG & TXRDYIFG) == 0);         	// Wait for transmitter to be ready
	I2CDRB = register_address;					// Register Address to read
	
	while(!(I2CIFG & ARDYIFG));					//wait until the reg address be sent
	
	//second: read the data
	I2CNDAT = data_len;
	U0CTL |= MST;
	I2CTCTL &= !I2CTRX;							//Not transmit,
	I2CTCTL |= I2CSTT + I2CSTP;
	while(!i2c_is_busy());
	
	for(counter = 0; counter < data_len; counter++)
	{
		while ((I2CIFG & RXRDYIFG) == 0);         // Wait for Receiver to be ready
		data[counter] = I2CDRB;
	}
    while(I2CTCTL & I2CSTP);//wait until the STOP be cleared.

	return true;
}

/**
 * API: i2c_init_it
 */
bool i2c_init_it(uint8_t slave_address, e_i2c_ms_mode i2c_ms_mode, void (*read_fun)(), void (*write_fun)())
{
	memset(&hi2c, 0x00, sizeof(hi2c));

	hi2c.i2c_status = I2C_NOT_INITED;
	P3SEL |= I2C_SCL_PIN | I2C_SDA_PIN;		// Select I2C pins
	U0CTL |= I2C + SYNC;					// Switch USART0 to I2C mode
	U0CTL &= ~I2CEN;                       	// Disable I2C
 	I2CTCTL = I2CSSEL_2;					// SMCLK Do not use ACLK, there is an issue when NO LPM.
	//this sets the I2C clk up to 100[KHz]
	I2CPSC = I2C_PSC_DIV_BY_08;				// Divider by 8
	I2CSCLH = 0x03;							// this value can not be less than 0x03// high period = (3 + 2) x I2CPSC
	I2CSCLL = 0x03;							// this value can not be less than 0x03// low  period = (3 + 2) x I2CPSC
	I2CSA = slave_address;					// Slave address
	I2C_ENABLE_INTERRUPTS;					// Enable I2C interrupts
	U0CTL |= I2CEN;							// Enable I2C
	if(i2c_ms_mode == I2C_MODE_MASTER)
	{
		U0CTL |= MST;							// Master
	}
	else
	{
		U0CTL &= ~MST;							// Slave
	}
	
	hi2c.i2c_status = I2C_IDLE;
	hi2c.i2c_ms_mode = i2c_ms_mode;
	hi2c.i2c_rw_mode = I2C_RW_MODE_NONE;
	hi2c.rx_buffer = NULL;
	hi2c.tx_buffer = NULL;
	hi2c.i2c_read_finished_it = read_fun;
	hi2c.i2c_write_finished_it = write_fun;
	
	return true;
}

/**
 * API: i2c_master_write_it
 */
bool i2c_master_write_it(uint8_t register_address, uint8_t* data, uint8_t data_len)
{
	if(i2c_is_busy())
	{
		return false;
	}
	if(hi2c.i2c_status != I2C_IDLE)
	{
		return false;	
	}
	
	U0CTL &= ~I2CEN;
	I2C_ENABLE_INTERRUPTS;
	I2CNDAT = data_len + REGISTER_LENGTH;
	U0CTL |= MST;
	U0CTL |= I2CEN;
	hi2c.number_of_data_to_write = data_len + REGISTER_LENGTH;//add the register address.
	hi2c.number_of_data_written = 0;
	hi2c.tx_buffer = data;
	hi2c.register_start_address = register_address;
	hi2c.i2c_status = I2C_BUSY;
	hi2c.i2c_rw_mode = I2C_RW_MODE_WRITE;
	
	I2CTCTL |= I2CSTT + I2CTRX + I2CSTP;					// Initiate transfer
	
	return true;
}


/**
 * API: i2c_master_read_it
 */
bool i2c_master_read_it(uint8_t register_address, uint8_t* data, uint8_t data_len)
{
	if(i2c_is_busy())
	{
		return false;
	}
	
	if(hi2c.i2c_status != I2C_IDLE)
	{
		return false;	
	}
	
	U0CTL &= ~I2CEN;
	I2C_ENABLE_INTERRUPTS;
	I2CNDAT = 1;	//in the first step just send the register address
	U0CTL |= MST;
	U0CTL |= I2CEN;
	hi2c.number_of_data_to_write = REGISTER_LENGTH;//add the register address.
	hi2c.number_of_data_written = 0;
	hi2c.number_of_data_to_read = data_len;
	hi2c.number_of_data_read = 0;
	hi2c.rx_buffer = data;
	hi2c.register_start_address = register_address;
	hi2c.i2c_status = I2C_BUSY;
	hi2c.i2c_rw_mode = I2C_RW_MODE_READ;
	
	//first: set the address to read by write it.
	I2CTCTL |= I2CSTT + I2CTRX;					// Initiate transfer
	
	return true;
}

/**
 * API: i2c_is_busy
 */
bool i2c_is_busy()
{
	return (I2CDCTL & I2CBUSY);
}

/*Not tested yet*/
void i2c_master_stop_it()
{
	U0CTL &= ~I2CEN;							// Disable I2C
	I2C_DISABLE_INTERRUPTS;						// Disable I2C interrupts
	hi2c.i2c_status = I2C_IDLE;
}


#pragma vector=USART0TX_VECTOR

__interrupt void I2C_ISR(void)
{
	switch(I2CIV)
 	{
		case I2CIV_NONE: break;							// No interrupt
		case I2CIV_AL: break;							// Arbitration lost
		case I2CIV_NACK: break;							// No Acknowledge
		case I2CIV_OA: break;							// Own Address
		case I2CIV_ARDY:								// Register Access Ready
			if(hi2c.i2c_rw_mode == I2C_RW_MODE_READ)	// IN READ or RECEPTION MODE
			{
				if(hi2c.number_of_data_written == 1)	//the registry address was sent
				{
					I2CNDAT = hi2c.number_of_data_to_read;
					U0CTL |= MST;
					I2CTCTL &= !I2CTRX;							//Not transmit,
					I2CTCTL |= I2CSTT + I2CSTP;
					hi2c.number_of_data_written = 2;// this is for stops the reading
				}
				else if(hi2c.number_of_data_written == 2)
				{
					hi2c.i2c_read_finished_it();
					hi2c.i2c_status = I2C_IDLE;
				}
			}
			else if(hi2c.i2c_rw_mode == I2C_RW_MODE_WRITE)
			{
				hi2c.i2c_write_finished_it();
				hi2c.i2c_status = I2C_IDLE;
			}
			break;

		case I2CIV_RXRDY:								// Receive Ready
			hi2c.rx_buffer[hi2c.number_of_data_read] = I2CDRB;
			hi2c.number_of_data_read += 1;
		 break;
		 
		case I2CIV_TXRDY:								// Transmit Ready 
			
			if(hi2c.i2c_rw_mode == I2C_RW_MODE_WRITE)	// IN WRITE or TRANSMIT MODE
			{
				if(hi2c.number_of_data_written < hi2c.number_of_data_to_write)
				{
					if(hi2c.number_of_data_written != 0)
					{
						I2CDRB = *(hi2c.tx_buffer + hi2c.number_of_data_written - 1);
					}
					else
					{
						I2CDRB = hi2c.register_start_address;
					}
					hi2c.number_of_data_written += 1;
				}
				else
				{
					hi2c.i2c_status = I2C_IDLE; // Never must be here
				}
			}
			else if(hi2c.i2c_rw_mode == I2C_RW_MODE_READ)	//IN READ or RECEPTION MODE
			{
				if(hi2c.number_of_data_written == 0)
				{
					I2CDRB = hi2c.register_start_address;
				}
				else if(hi2c.number_of_data_written == 1)
				{
					hi2c.i2c_status = I2C_ERROR;
				}
				hi2c.number_of_data_written += 1;
				
			}
		   break;

	   case I2CIV_GC: break;                          // General Call

	   case I2CIV_STT: break;                          // Start Condition
	}
}
