#include "common.h"

#define OVERSAMPLE_RATE (16)
#define BUS_CLOCK (24E6) // bus clock of 24MHz
#define SYSTEM_CLOCK (24E6) // system clock of 24MHz
#define STOP_BIT (1) // 0 for 1-Stop Bit and 1 for 2-Stop Bit
#define PARITY (0) // 0 for No Hardware Parity and 1 to Enable the Parity
#define DATA_SIZE (0) // 0 to use 8-bit data character and 1 to use 9 bit data character

cbuff_t Txbuf, Rxbuf;

/**
 * @brief: function configures the UART by setting and clearing the
 * fields of registers as per the requirement
 *
 * @param: takes baud rate as a parameter (in this case it 38400)
 * @return: NULL
 */
void Init_UART0(uint32_t baud_rate)
{
	uint16_t sbr;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 24 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);

	// Setting baud rate and oversampling ratio
	sbr = (uint16_t)((SYSTEM_CLOCK)/(baud_rate * OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(OVERSAMPLE_RATE-1);

	// Disable RX Input Active Edge Interrupt and LIN Break Detect Interrupt, select 2-Stop bits by setting SBNS field
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BIT) | UART0_BDH_LBKDIE(0);

	//Loopback mode is disabled, 8 data bit mode is used and parity is not used
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_SIZE) | UART0_C1_PE(PARITY);

	// Transmit data not inverted, OR interrupt disabled, NF interrupt disabled, FE interrupt disabled, PF interrupt disabled
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// LSB bit is transmitted first, receive data not inverted
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	// initialize Txbuf and Rxbuf buffers
	cbuff_init(&Txbuf);
	cbuff_init(&Rxbuf);

	//UART interrupt has been given second priority
	NVIC_SetPriority(UART0_IRQn, 2);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts
	UART0->C2 |= UART_C2_RIE(1);

	//Receiver and Transmitter are enabled
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);
}

/**
 * @brief: handler function performs the operations whenever the
 * interrupt triggers. In this case, it performs enqueue and
 * dequeue operations accordingly.
 *
 * @param: NULL
 * @return: NULL
 */
void UART0_IRQHandler(void)
{
	uint8_t ch;
	if(UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			ch = UART0->D;
	}

	//check if receive data buffer is empty, then enqueue into Rxbuf buffer
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		ch = UART0->D;
		cbuff_enqueue(&Rxbuf,&ch,1);
	}

	//check if hardware interrupt for TDRE is enabled, if enabled then perform dequeue operation from Txbuf (transmitter buffer)
	if((UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK))
	{
		if(cbuff_dequeue(&Txbuf,&ch,1)==1)
		{
			UART0->D = ch;
		}
		else
		{
			UART0->C2 &= ~UART0_C2_TIE_MASK; //clear the TIE mask
		}
	}
}

/**
 * @brief: UART code is tied with system input/output functions
 * such as printf(), putchar(), etc.
 *
 * @param: pointer to the string which is to be computed and
 * size of the string as count in this case.
 * @return: returns -1 on error and 0 on success
 */
int __sys_write(int handle, char* buf, int size)
{
	if(buf==NULL)
	{
		return -1;
	}
	while(ifFull(&Txbuf)); //check if Txbuf is full, if full then wait for the vacant space
	if(cbuff_enqueue(&Txbuf, buf, size) != size)
	{
		return -1;
	}
	if(!(UART0->C2 & UART0_C2_TIE_MASK)) //check if TIE mask is enabled, if disable then set the TIE mask
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}
	return 0;
}

/**
 * @brief: UART code is tied with system input/output functions
 * such as getchar(), etc.
 *
 * @param: NULL
 * @return: returns -1 if there is no data available to be read
 * and returns the character which is read by the serial connection.
 */
int __sys_readc(void)
{
	char ch;
	while(ifEmpty(&Rxbuf)); //check if Rxbuf buffer is empty, if empty then wait for data to arrive
	if(cbuff_dequeue(&Rxbuf, &ch, 1) != 1)
	{
		return -1;
	}
	return ch;
}
