/**
 * @file: main.c
 * @brief: function has the infinite loop for reading the values from the sensors
 *
 * @author: Harshwardhan Singh, harshwardhan.singh@colorado.edu
 * @date: 13th December 2021
 * @references/citations: Previous assignments and
 * Embedded Systems Fundamentals with
 * Arm Cortex-M based Microcontrollers (Textbook)
 * by Alexander G.Dean, GitHub repo and
 * KL25Z Reference Manual.
 */

#include "common.h"

#define BAUD_RATE (115200) //setting the baud rate as 115200

#define TEMP_WARN (24) //threshold temperature for warning situation
#define HUM_WARN (30) //threshold humidity for warning situation
#define LUX_WARN (50) //threshold lux for warning situation

#define TEMP_DANG (25) //threshold temperature for danger situation
#define HUM_DANG (50) //threshold humidity for danger situation
#define LUX_DANG (100) //threshold lux for danger situation

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootPeripherals();

    sysclock_init(); //initialized the system clock
    Init_Systick(); //initialized the systick
    Init_UART0(BAUD_RATE); //initialized UART0 for printing the terminal

    //declared floating point variables for storing temperature, humidity and lux
    float temperature,lux,humidity;

    Init_Pwm(); //initialized the PWM for RGB LED
    I2C1_Init(); //initialized I2C1
    I2C1_Set_Flags(); //setting the flags for I2C1

    I2C0_Init(); //initialized I2C0
    I2C0_Set_Flags(); //setting the flags for I2C0
    Tsl_Init(); //calling the TSL2561 function for setting the appropriate registers

    Test_Cbuff(); //test function for circular buffer
    Test_Sensors(); //test function for the testing of hardware (SHT21 and TSL2561 sensors)

    printf("Plant Health Monitoring Systems\n\r\n\r");

    /*
     * infinite while loop for getting the values in every 2sec
     */
    while(1)
    {
    	//getting the value from the temperature (SHT21) sensor
    	I2C1_Set_Flags(); //setting the flags for I2C1
    	temperature = Get_Temperature(); //reading temperature

    	//getting the value from the humidity (SHT21) sensor
    	I2C1_Set_Flags(); //setting the flags for I2C1
    	humidity = Get_Humidity(); //reading humidity

    	//getting the value from the light (TSL2561) sensor
    	I2C0_Set_Flags(); //setting the flags for I2C0
    	lux = Get_Lux(); //reading luminosity

    	/*
    	 * glowing the RED LED and printing the danger message when the threshold is reached
    	 * glowing the YELLOW LED and printing the warning message when the threshold is reached
    	 * glowing the GREEN LED when the values are in the safer range
    	 */
    	if(temperature > TEMP_DANG && humidity > HUM_DANG && lux > LUX_DANG)
    	{
    		Set_Color(255,0,0); //RED LED
    		printf("Danger....Danger....Danger....\n\r");
    	}
    	else if(temperature > TEMP_WARN && humidity > HUM_WARN && lux > LUX_WARN)
    	{
    		Set_Color(255,255,0); //YELLOW LED
    		printf("Warning....Warning....Warning....\n\r");
    	}
    	else
    	{
    		Set_Color(0,255,0); //GREEN LED
    		printf("Safe....Safe....Safe....\n\r");
    	}

    	printf("Temperature = %0.2f Celsius\n\r", temperature);
    	printf("Humidity = %0.2f percent \n\r", humidity);
    	printf("Illuminance = %0.2f lux\n\r", lux);
    	printf("\n\r");

    	Reset_Timer(); //resetting the systick timer
    	while(Get_Timer()!=200); //checking if 2sec are completed to get the next reading
    }
    return 0 ;
}
