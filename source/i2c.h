#ifndef I2C_H_
#define I2C_H_

#include "common.h"

/*
 * macros for I2C0
 */

//start signal is generated and master mode is selected
#define I2C0_M_START (I2C0->C1 |= I2C_C1_MST_MASK)
//stop signal is generated and slave mode is selected
#define I2C0_M_STOP  (I2C0->C1 &= ~I2C_C1_MST_MASK)
//setting the Repeat Start bit to generate repeated start condition
#define I2C0_M_RSTART (I2C0->C1 |= I2C_C1_RSTA_MASK)
//transmit mode is selected
#define I2C0_TRAN (I2C0->C1 |= I2C_C1_TX_MASK)
//receive mode is selected
#define I2C0_REC (I2C0->C1 &= ~I2C_C1_TX_MASK)
//wait till getting the interrupt
#define I2C0_WAIT while((I2C0->S & I2C_S_IICIF_MASK)==0);\
					I2C0->S |= I2C_S_IICIF_MASK;

//no acknowledgment signal is sent to the bus
#define I2C0_NACK (I2C0->C1 |= I2C_C1_TXAK_MASK)
//acknowledgment signal is sent to the bus
#define I2C0_ACK (I2C0->C1 &= ~I2C_C1_TXAK_MASK)
//checking if transfer is completed
#define ACK_SIGNAL while((I2C0->S & I2C_S_TCF_MASK)==0);
//checking if acknowledgment signal is received
#define NACK_SIGNAL while(!(I2C0->S & I2C_S_RXAK_MASK)==0);


/*
 * macros from I2C1
 */

//start signal is generated and master mode is selected
#define I2C1_M_START (I2C1->C1 |= I2C_C1_MST_MASK)
//stop signal is generated and slave mode is selected
#define I2C1_M_STOP 	(I2C1->C1 &= ~I2C_C1_MST_MASK)
//setting the Repeat Start bit to generate repeated start condition
#define I2C1_M_RSTART (I2C1->C1 |= I2C_C1_RSTA_MASK)
//transmit mode is selected
#define I2C1_TRAN (I2C1->C1 |= I2C_C1_TX_MASK)
//receive mode is selected
#define I2C1_REC (I2C1->C1 &= ~I2C_C1_TX_MASK)
//wait till getting the interrupt
#define I2C1_WAIT while((I2C1->S & I2C_S_IICIF_MASK)==0);\
					I2C1->S |= I2C_S_IICIF_MASK;
//no acknowledgment signal is sent to the bus
#define I2C1_NACK (I2C1->C1 |= I2C_C1_TXAK_MASK)
//acknowledgment signal is sent to the bus
#define I2C1_ACK (I2C1->C1 &= ~I2C_C1_TXAK_MASK)

/*
 *  Functions for TSL2561 sensor (light sensor)
 */
void I2C0_Init(void);
void I2C0_Set_Flags(void);
void Write_Luminosity(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
uint8_t Read_Luminosity(uint8_t dev_addr,uint8_t reg_addr);

/*
 *  Functions for SHT21 sensor (temperature and humidity sensor)
 */
void I2C1_Init(void);
void I2C1_Set_Flags(void);
uint16_t Read_Temp_Hum(uint8_t dev_addr,uint8_t reg_addr);

#endif /* I2C_H_ */
