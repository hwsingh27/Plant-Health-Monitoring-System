/**
 * @file: test.c
 * @brief: testing of circular buffer and sensors (SHT21 and TSL2561)
 * is performed in this file to make circular buffers solid for working with the UART.
 * Asserts are used to give the failure message at that line.
 *
 * @author: Harshwardhan Singh, harshwardhan.singh@colorado.edu
 * @date: 13th December 2021
 * @references/citations: Prof. Howdy Pierce's test suit shared
 * over the Canvas for testing the circular buffer
 */

#include "common.h"

#define TEMP_LOW (23) //lower threshold of temperature
#define TEMP_HIGH (24) //upper threshold of temperature
#define HUM_LOW (7) //lower threshold of humidity
#define HUM_HIGH (15) //upper threshold of humidity
#define LUX_LOW (400) //lower threshold of illuminance
#define LUX_HIGH (450) //upper threshold of illuminance

void Test_Cbuff(void)
{
	char *str ="To be, or not to be: that is the question:\n"
	    "Whether 'tis nobler in the mind to suffer\n"
	    "The slings and arrows of outrageous fortune,\n"
	    "Or to take arms against a sea of troubles,\n"
	    "And by opposing end them? To die, to sleep--\n"
	    "No more--and by a sleep to say we end\n"
	    "The heart-ache and the thousand natural shocks\n"
	    "That flesh is heir to, 'tis a consummation\n"
	    "Devoutly to be wish'd. To die, to sleep;\n"
	    "To sleep: perchance to dream: ay, there's the rub;\n"
	    "For in that sleep of death what dreams may come\n"
	    "When we have shuffled off this mortal coil,\n"
	    "Must give us pause.";

	//temporary buffer used for dequeue
	char buf[1024];

	//initialization of Tx buffer for test as multiple instance
	cbuff_t Tx;
	cbuff_init(&Tx);
	const int cap = cbuff_capacity(&Tx);

	assert(strlen(str) >= cap*2);
	assert(sizeof(buf)>cap);
	assert(cap==256);

	assert(cbuff_length(&Tx)==0);
	assert(cbuff_dequeue(&Tx,buf,cap)==0);
	assert(cbuff_dequeue(&Tx,buf,1)==0);

	assert(cbuff_enqueue(&Tx,str,10)==10);
	assert(cbuff_length(&Tx)==10);
	assert(cbuff_dequeue(&Tx,buf,10)==10);
	assert(strncmp(buf,str,10)==0);
	assert(cbuff_length(&Tx)==0);

	assert(cbuff_enqueue(&Tx,str,20)==20);
	assert(cbuff_length(&Tx)==20);
	assert(cbuff_dequeue(&Tx,buf,5)==5);
	assert(cbuff_length(&Tx)==15);
	assert(cbuff_dequeue(&Tx,buf+5,20)==15);
	assert(cbuff_length(&Tx)==0);
	assert(strncmp(buf,str,20)==0);

	assert(cbuff_enqueue(&Tx,str,cap)==cap);
	assert(cbuff_length(&Tx)==cap);
	assert(cbuff_enqueue(&Tx,str,1)==0);
	assert(cbuff_dequeue(&Tx,buf,cap)==cap);
	assert(cbuff_length(&Tx)==0);
	assert(strncmp(buf,str,cap)==0);

	assert(cbuff_enqueue(&Tx,str,20)==20);
	assert(cbuff_length(&Tx)==20);
	assert(cbuff_dequeue(&Tx,buf,18)==18);
	assert(cbuff_length(&Tx)==2);
	assert(strncmp(buf,str,18)==0);

	int chunk_size = (cap-2)/4;
	for(int i=0;i<4;i++)
	{
		assert(cbuff_enqueue(&Tx,str+i*chunk_size,chunk_size)==chunk_size);
		assert(cbuff_length(&Tx)==(i+1)*chunk_size+2);
	}
	assert(cbuff_length(&Tx) == 4*chunk_size+2);

	assert(cbuff_dequeue(&Tx,buf,2)==2);
	assert(strncmp(buf,str+18,2)==0);

	for(int i=0;i<chunk_size*4;i++)
	{
		assert(cbuff_dequeue(&Tx,buf+i,1)==1);
		assert(cbuff_length(&Tx)==chunk_size*4 - i - 1);
	}
	assert(strncmp(buf,str,chunk_size*4)==0);

	assert(cbuff_enqueue(&Tx,str, 65) == 65);
	assert(cbuff_enqueue(&Tx,str+65, cap) == cap-65);
	assert(cbuff_length(&Tx) == cap);
	assert(cbuff_dequeue(&Tx,buf,cap) == cap);
	assert(cbuff_length(&Tx) == 0);
	assert(strncmp(buf, str, cap) == 0);

	assert(cbuff_enqueue(&Tx,str,0) == 0);
	assert(cbuff_length(&Tx) == 0);

	assert(cbuff_enqueue(&Tx,str,32) == 32);
	assert(cbuff_length(&Tx) == 32);
	assert(cbuff_dequeue(&Tx,buf,16) == 16);
	assert(cbuff_length(&Tx) == 16);
	assert(strncmp(buf, str, 16) == 0);

	assert(cbuff_enqueue(&Tx,str+32, 32) == 32);
	assert(cbuff_length(&Tx) == 48);
	assert(cbuff_enqueue(&Tx,str+64, cap-64) == cap-64);
	assert(cbuff_length(&Tx) == cap-16);
	assert(cbuff_dequeue(&Tx,buf+16, cap-16) == cap-16);
	assert(strncmp(buf, str, cap) == 0);

	assert(cbuff_enqueue(&Tx,str, 32) == 32);
	assert(cbuff_length(&Tx) == 32);
	assert(cbuff_dequeue(&Tx,buf, 16) == 16);
	assert(cbuff_length(&Tx) == 16);
	assert(strncmp(buf, str, 16) == 0);

	assert(cbuff_enqueue(&Tx,str+32, cap-20) == cap-20);
	assert(cbuff_length(&Tx) == cap-4);
	assert(cbuff_dequeue(&Tx,buf, cap-8) == cap-8);
	assert(strncmp(buf, str+16, cap-8) == 0);
	assert(cbuff_length(&Tx) == 4);
	assert(cbuff_dequeue(&Tx,buf, 8) == 4);
	assert(strncmp(buf, str+16+cap-8, 4) == 0);
	assert(cbuff_length(&Tx) == 0);

	assert(cbuff_enqueue(&Tx,str, 49) == 49);
	assert(cbuff_length(&Tx) == 49);
	assert(cbuff_dequeue(&Tx,buf, 16) == 16);
	assert(cbuff_length(&Tx) == 33);
	assert(strncmp(buf, str, 16) == 0);

	assert(cbuff_enqueue(&Tx,str+49, cap-33) == cap-33);
	assert(cbuff_length(&Tx) == cap);
	assert(cbuff_dequeue(&Tx,buf, cap) == cap);
	assert(cbuff_length(&Tx) == 0);
	assert(strncmp(buf, str+16, cap) == 0);

	assert(cbuff_enqueue(&Tx,str, 32) == 32);
	assert(cbuff_length(&Tx) == 32);
	assert(cbuff_dequeue(&Tx,buf, 16) == 16);
	assert(cbuff_length(&Tx) == 16);
	assert(strncmp(buf, str, 16) == 0);

	assert(cbuff_enqueue(&Tx,str+32, cap) == cap-16);
	assert(cbuff_dequeue(&Tx,buf, 1) == 1);
	assert(cbuff_length(&Tx) == cap-1);
	assert(cbuff_dequeue(&Tx,buf+1, cap-1) == cap-1);
	assert(cbuff_length(&Tx) == 0);
	assert(strncmp(buf, str+16, cap) == 0);

	int wpos=0, rpos=0;
	assert(cbuff_enqueue(&Tx,str, cap-4) == cap-4);
	wpos += cap-4;
	assert(cbuff_length(&Tx) == cap-4);
	assert(cbuff_dequeue(&Tx,buf, 32) == 32);
	rpos += 32;
	assert(cbuff_length(&Tx) == cap-36);
	assert(strncmp(buf, str, 32) == 0);
	assert(cbuff_enqueue(&Tx,str+wpos, 12) == 12);
	wpos += 12;
	assert(cbuff_length(&Tx) == cap-24);

	assert(cbuff_enqueue(&Tx,str+wpos, 16) == 16);
	assert(cbuff_length(&Tx) == cap-8);
	assert(cbuff_dequeue(&Tx,buf, cap) == cap-8);
	assert(cbuff_length(&Tx) == 0);
	assert(strncmp(buf, str+rpos, cap-8) == 0);

	wpos=0;
	rpos=0;
	assert(cbuff_enqueue(&Tx,str, cap-4) == cap-4);
	wpos += cap-4;
	assert(cbuff_length(&Tx) == cap-4);
	assert(cbuff_dequeue(&Tx,buf, 32) == 32);
	rpos += 32;
	assert(cbuff_length(&Tx) == cap-36);
	assert(strncmp(buf, str, 32) == 0);
	assert(cbuff_enqueue(&Tx,str+wpos, 12) == 12);
	wpos += 12;
	assert(cbuff_length(&Tx) == cap-24);

	assert(cbuff_enqueue(&Tx,str+wpos, 24) == 24);
	assert(cbuff_length(&Tx) == cap);
	assert(cbuff_dequeue(&Tx,buf, cap) == cap);
	assert(cbuff_length(&Tx) == 0);
	assert(strncmp(buf, str+rpos, cap) == 0);

	wpos=0;
	rpos=0;
	assert(cbuff_enqueue(&Tx,str, cap-4) == cap-4);
	wpos += cap-4;
	assert(cbuff_length(&Tx) == cap-4);
	assert(cbuff_dequeue(&Tx,buf, 32) == 32);
	rpos += 32;
	assert(cbuff_length(&Tx) == cap-36);
	assert(strncmp(buf, str, 32) == 0);
	assert(cbuff_enqueue(&Tx,str+wpos, 12) == 12);
	wpos += 12;
	assert(cbuff_length(&Tx) == cap-24);

	assert(cbuff_enqueue(&Tx,str+wpos, 64) == 24);
	assert(cbuff_length(&Tx) == cap);
	assert(cbuff_dequeue(&Tx,buf, cap) == cap);
	assert(cbuff_length(&Tx) == 0);
	assert(strncmp(buf, str+rpos, cap) == 0);

	printf("%s: Test Cases Passed for Circular Buffer\n\r",__FUNCTION__);
}

/**
 * @function: the function tests the readings from the sensors whether they are correct
 * or not (Arduino UNO is used to get the correct readings from the sensors as it
 * uses APIs) and these actual values were compared with the KL25Z's
 *
 * @parameter: NULL
 * @return: NULL
 */
void Test_Sensors(void)
{
	uint32_t temperature,lux,humidity;

	//getting the temperature value from the SHT21 sensor
	I2C1_Set_Flags();
	temperature = Get_Temperature();

	//getting the humidity value from the SHT21 sensor
	I2C1_Set_Flags();
	humidity = Get_Humidity();

	//getting the illuminance value from the TSL2561 sensor
	I2C0_Set_Flags();
	lux = Get_Lux();

	//checking if temperature falls within the actual range
	assert(temperature >= TEMP_LOW && temperature < TEMP_HIGH);
	//checking if humidity falls within the actual range
	assert(humidity >= HUM_LOW && humidity < HUM_HIGH);
	//checking if illuminance falls within the actual range
	assert(lux >= LUX_LOW && lux < LUX_HIGH);

	printf("%s: Test Cases Passed for Sensors\n\n\r",__FUNCTION__);
}
