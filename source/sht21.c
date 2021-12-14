
#include "common.h"

#define SHT21_ADDR (0x40) //device address i.e. SHT21
#define TEMPERATURE (0xE3) //address of the register to get the temperature value
#define HUMIDITY (0xE5) //address of the register to get the humidity value

/**
 * @function: the function returns the temperature in degree Celsius using datasheet (formula)
 *
 * @parameter: NULL
 * @return: computed temperature in degree celsius in float
 */
float Get_Temperature(void)
{
	//getting the data from the SHT21, temperature register
	uint16_t raw_data = Read_Temp_Hum(SHT21_ADDR,TEMPERATURE);
	//clearing last 2 bits (mentioned in the datasheet)
	raw_data &= ~0x003;
	//calculation to convert the raw data to degree Celcius
	return (-46.85 + 175.72/65536 * (float)raw_data);
}

/**
 * @function: the function returns the humidity in percentage using datasheet (formula)
 *
 * @parameter: NULL
 * @return: computed humidity in percentage
 */
float Get_Humidity(void)
{
	//getting the data from the SHT21, humidity register
	uint16_t raw_data = Read_Temp_Hum(SHT21_ADDR,HUMIDITY);
	//clearing last 2 bits (mentioned in the datasheet)
	raw_data &= ~0x003;
	//calculation to convert the raw data to humidity (in percentage)
	return (-6.0 + 125.0/65536 * (float)raw_data);
}
