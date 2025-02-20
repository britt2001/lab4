/*
 * ejemploSlave.c
 *
 * Created: 2/10/2025 8:25:46 PM
 * Author : Britany Barillas
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include "SPI.h"

int main(void)
{
	DDRC |= (1<<DDC1)|(1<<DDC2)|(1<<DDC3)|(1<<DDC4)|(1<<DDC5);
	DDRD |= (1<<DDD2)|(1<<DDD3)|(1<<DDD4);
	PORTC &= ~((1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5));
	PORTD &= ~((1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4));

	spiInit(SPI_SLAVE_SS, SPI_DATA_ORDER_MSB, SPI_CLOCK_IDLE_LOW, SPI_CLOCK_FIRST_EDGE);
	initADC();
	SPCR |= (1<<SPIE);
	sei();
	
	while (1)
	{
		// Iniciando secuencia de adc
		ADCSRA |= (1<<ADSC);
		refreshPORT(valorADC);
		_delay_ms(100);
	}
}

void initADC(void) {
	ADMUX = 0;

	// Vref = AVCC = 5V
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);

	// Justificando hacia la izquierda
	ADMUX |= (1<<ADLAR);

	ADCSRA = 0;
	// Encendiendo ADC
	ADCSRA |= (1<<ADEN);

	// Habilitamos ISR ADC
	ADCSRA |= (1<<ADIE);
	
	// Prescaler de 128 ? 16M / 128 = 125kHz
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

	// Deshabilitando la entrada digital PC0
	DIDR0 |= (1<<ADC0D);
	
}

ISR(ADC_vect) {
	valorADC = ADCH;
	ADCSRA |= (1<<ADIF);
}

ISR(SPI_STC_vect) {
	uint8_t spiValor = SPDR;
	if(spiValor == 'c') {
		spiWrite(valorADC);
	}
}



