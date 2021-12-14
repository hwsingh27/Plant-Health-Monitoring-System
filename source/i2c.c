/**
 * @file: i2c.c
 * @brief: functions to initialize I2C0 and I2C1
 * and reading and writting to I2C0 and I2C1
 *
 * @author: Harshwardhan Singh, harshwardhan.singh@colorado.edu
 * @date: 13th December 2021
 * @references/citations:
 * Embedded Systems Fundamentals with
 * Arm Cortex-M based Microcontrollers (Textbook)
 * by Alexander G.Dean, GitHub repo and
 * KL25Z Reference Manual.
 */

#include "common.h"

/**
 * @function: function sets the clock for I2C0 and PORTC
 *
 * @parameter: NULL
 * @return: NULL
 */
void I2C0_Init(void)
{
	//enabling the clock for I2C0
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	//enabling the clock for PORTC
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
}

/**
 * @function: the function sets the flags ICR, MULT and IICEN
 *
 * @parameter: NULL
 * @return: NULL
 */
void I2C0_Set_Flags(void)
{
	//setting the I2C0 SCL port
	PORTC->PCR[8] |= PORT_PCR_MUX(2);
	//setting the I2C0 SDA port
	PORTC->PCR[9] |= PORT_PCR_MUX(2);
	//setting the C1 register as 0 to clear all the bits
	I2C0->C1 = 0;
	//setting the clock rate by dividing 24MHz by 64k baud to get 375KHz and selecting the multiplying factor as 1
	I2C0->F = I2C_F_ICR(0X14) | I2C_F_MULT(0);
	//enable the I2C0 by setting the IICEN mask
	I2C0->C1 = I2C_C1_IICEN_MASK;
}

/**
 * @function: function is used to write into the TSL2561 sensor
 *
 * @parameter: device address, register address and data
 * @return: NULL
 */
void Write_Luminosity(uint8_t dev_addr, uint8_t reg_addr, uint8_t data)
{
	I2C0_TRAN;
	I2C0_M_START;

	I2C0->D = dev_addr<<1; //shifted to left by 1 bit to make the LSB as 0 for writing to the register.
	ACK_SIGNAL
	NACK_SIGNAL
	I2C0_WAIT;

	I2C0->D = reg_addr; //passing the register address
	ACK_SIGNAL
	NACK_SIGNAL
	I2C0_WAIT;

	I2C0->D = data; //writing data to the register
	ACK_SIGNAL
	NACK_SIGNAL
	I2C0_WAIT;

	I2C0_M_STOP;
}

/**
 * @function: function reads the raw value of lux from the TSL2561 sensor
 *
 * @parameter: device address and register address
 * @return: the raw value of luminosity
 */
uint8_t Read_Luminosity(uint8_t dev_addr,uint8_t reg_addr)
{
	uint8_t data;

	I2C0_TRAN;
	I2C0_M_START;

	I2C0->D = dev_addr<<1; //shifted to left by 1 bit to make the LSB as 0 for writing to the register.
	ACK_SIGNAL
	I2C0_WAIT;

	I2C0->D = reg_addr; //passing the register address
	ACK_SIGNAL
	I2C0_WAIT;

	I2C0_M_RSTART;

	I2C0->D = (dev_addr<<1)|0x01; //making the LSB as 1 to read from the register
	ACK_SIGNAL
	I2C0_WAIT;

	I2C0_REC;
	I2C0_NACK;

	data = I2C0->D; //reading the dummy value from the register to get the actual value in next read
	ACK_SIGNAL
	I2C0_WAIT;

	I2C0_M_STOP;

	data = I2C0->D; //reading the value from the register and storing in the data variable

	return data;
}

/**
 * @function: function sets the clock for I2C1 and PORTE
 *
 * @parameter: NULL
 * @return: NULL
 */
void I2C1_Init(void)
{
	//enabling the clock for I2C1
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;
	//enabling the clock for PORTE
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
}

/**
 * @function: the function sets the flags ICR, MULT and IICEN
 *
 * @parameter: NULL
 * @return: NULL
 */
void I2C1_Set_Flags(void)
{
	//setting the I2C1 SDA port
	PORTE->PCR[0] |= PORT_PCR_MUX(6);
	//setting the I2C1 SCL port
	PORTE->PCR[1] |= PORT_PCR_MUX(6);
	//setting the C1 register as 0 to clear all the bits
	I2C1->C1 = 0;
	//setting the clock rate by dividing 24MHz by 400KHz baud to get 60KHz and selecting the multiplying factor as 1
	I2C1->F = I2C_F_ICR(0x11) | I2C_F_MULT(0);
	//enable the I2C0 by setting the IICEN mask
	I2C1->C1 |= (I2C_C1_IICEN_MASK);
	//select high drive mode
	I2C1->C2 |= (I2C_C2_HDRS_MASK);
}

/**
 * @function: function reads the raw value of temperature and humidity from SHT21 sensor
 *
 * @parameter: device address and register address
 * @return: returns the raw values of temperature and humidity
 */
uint16_t Read_Temp_Hum(uint8_t dev_addr,uint8_t reg_addr)
{
	uint8_t data[2];
	uint16_t result;

	I2C1_TRAN;
	I2C1_M_START;

	I2C1->D = dev_addr<<1; //shifted to left by 1 bit to make the LSB as 0 for writing to the register.
	TRANS_COM
	I2C1_WAIT;

	I2C1->D = reg_addr; //passing the register address
	TRANS_COM
	I2C1_WAIT;

	I2C1_M_RSTART;

	I2C1->D = ((dev_addr<<1)|0x01); //making the LSB as 1 to read from the register
	TRANS_COM
	I2C1_WAIT;

	I2C1_REC;
	I2C1_NACK;

	data[0] = I2C1->D; //reading the dummy value from the register to get the actual value in next read
	TRANS_COM
	I2C1_WAIT;

	data[0] = I2C1->D; //reading the value from the register to make it MSB
	TRANS_COM
	I2C1_WAIT;
	data[1] = I2C1->D; //reading the value from the register to make it LSB

	I2C1_M_STOP;

	result = (data[0]<<8); //making it MSB
	result += data[1]; //adding it to data[1] to get the complete result

	return result;
}
