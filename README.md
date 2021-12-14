# PES-Final-Project
# Plant Health Monitoring System

## Objective of the Assignment <br/>
1) The proposed system indicates whether the temperature, humidity and light are sufficient for the plants to survive in the greenhouse.<br/>
2) This is done by using the two external sensors (temperature and light, SHT21 and TSL2561) that are interfaced with the board over I2C protocol.<br/>
3) The Safe, Warning and Danger message is displayed through UART on the serial terminal.<br/>
4) REG, GREEN and YELLOW LEDs glow using PWM indicating different conditions.<br/><br/>

## Implementation
1) FRDM-KL25Z reads the temperature, humidiy and illumination values from the SHT21 and TSL2561 (temperature, humidity and light sensors).<br/>
2) This interfacing is done over the I2C communication protocol.<br/>
3) I2C0 and I2C1 are used for TSL2561 and SHT21 sensors respectively.<br/>
4) Datasheets of TSL2561 and SHT21 were used to get the values from the sensor.<br/>
5) UART is used to print the safe, warning and danger messages on the serial terminal.<br/>
6) RGB LEDs are used to indicate different situations.<br/>
- GREEN: indicates the safe state.<br/>
- RED: indicates the danger state.<br/>
- YELLOW: indicates the warning state.<br/>
7) Systick is used to get the delay of 2sec (reading the values from the sensor in every 2sec).<br/>
8) For demonstration (demo video), the threshold values of temperature, humidity and luminosity was changed.<br/><br/>

## Files used from the previous assignments
1) systick.c<br/>
2) circular_buffer.c<br/>
3) uart.c<br/>
4) sysclock.c<br/>
5) pwm.c<br/>
6) test.c (only for testing the circular buffer and not for testing the hardware(sensors)).<br/><br/>

## Testing (test.c)
1) Arduino UNO is used to measure the actual values of temperature, humidity and light as it has APIs that are easily avaiable are accurate.<br/>
2) The measured values by KL25Z are compared with the values received from the Arudino.<br/>
3) Screenshots of Arduino UNO's output are provided in the repo (foldername: Images of Project).<br/><br/>

## URL for google drive
https://drive.google.com/drive/u/4/folders/1r83D3b3eRKWKd0NEbJiSYxHF2RcEOPhN <br/>
Part-1: Introduction<br/>
Part-2: Demonstration<br/>
Part-3: Testing <br/><br/>

## Bonus (Extra Implementation)
1) Along with temperature and illumination, humidity is also implemented and the data of humidity is extracted from the SHT21 sensor along with the temperature value.<br/>
2) Implementation is Humidity was not mentioned in the Project Proposal.<br/><br/>

## Changes
1) Instead of using MAX44009 (light sensor with I2C), TSL2561 (light sensor with I2C) is used. TSL2561 was not mentioned in the proposal. Although both are same as they<br/>
both use I2C for interfacing. Both the sensors measure light in 'lux'.<br/>
2) SHT21 is used as the temperature sensor (mentioned in project proposal). And measures both temperature and humidity.<br/>
