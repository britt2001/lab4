/*
 * SPI.c
 *
 * Created: 2/6/2025 7:39:54 PM
 *  Author: Britany Barillas
 */ 


#include "SPI.h"

void spiInit(Spi_Type sType, Spi_Data_Order sDataOrder, Spi_Clock_Polarity sClockPolarity, Spi_Clock_Phase sClockPhase)
{
	// PB2 -> SS
	// PB3 -> MOSI
	// PB4 -> MISO
	// PB5 -> SCK

	if (sType & (1 << MSTR)) // If Master Mode
	{
		DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2); // MOSI, SCK, NEGADO_SS salida
		DDRB &= ~(1 << DDB4); // MISO entrada
		SPCR |= (1 << MSTR);  // Master
	

		uint8_t temp = sType & 0b00000111;
		switch (temp){
			case 0: // DIV2
			SPCR &= ~((1 << SPR1) | (1 << SPR0));
			SPSR |= (1 << SPI2X);
			break;

			case 1: // DIV4
			SPCR &= ~((1 << SPR1) | (1 << SPR0));
			SPSR &= ~(1 << SPI2X);
			break;

			case 2: // DIV8
			SPCR |= (1 << SPR0);
			SPCR &= ~(1 << SPR1);
			SPSR |= (1 << SPI2X);
			break;

			case 3: // DIV16
			SPCR |= (1 << SPR0);
			SPCR &= ~(1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;

			case 4: // DIV32
			SPCR &= ~(1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR |= (1 << SPI2X);
			break;

			case 5: // DIV64
			SPCR &= ~(1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;

			case 6: // DIV128
			SPCR |= (1 << SPR0) | (1 << SPR1);
			SPSR &= ~(1 << SPI2X);
			break;
		}
	}
	else// If Slave Mode
	{
		DDRB |= (1 << DDB4); // MISO salidas
		DDRB &= ~((1 << DDB3) | (1 << DDB5) | (1 << DDB2)); // MOSI, SCK, SS entradas
		SPCR &= ~(1 << MSTR); // Slave
	}
/* Enable SPI, Data Order, Clock Polarity, Clock Phase */
	SPCR |=  (1 << SPE)|sDataOrder|sClockPolarity|sClockPhase;

}

static void spiReceiveWait() //verifica que SPIF no este encendido IMPORTANTE
{
	while (!(SPSR & (1 << SPIF))); // Wait for Data Receive complete
}

//Carga el dato en el registro de dato
void spiWrite(uint8_t dat) // Write data to SPI bus
{
	SPDR = dat;
}
//no es necesaria pero x
unsigned spiDataReady() // Check whether the data is ready to read
{
	if (SPSR & (1 << SPIF))
	return 1;
	else
	return 0;
}

uint8_t spiRead(void) // Read the received data
{
	while (!(SPSR & (1 << SPIF))); // Wait for Data Receive complete
	return (SPDR); // Read the received data from the buffer
}
