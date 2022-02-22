#ifndef F_CPU
#define F_CPU 16000000UL                    // set the CPU clock
#endif

#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#include "rtc.h"
#include "delay.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/eeprom.h> 
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <avr/sleep.h>

#define UBRRVAL 103

void usart_init(void);
void sendbyte(unsigned char);
void sendstr(unsigned char *);
unsigned char receivebyte(void);
unsigned char receivestr(void);
int ADC_Read(void);
void ADC_Init(void);
char * toArray(int);
void ADC_measure(uint8_t channel);
float make_measure(void);
void save_measures(float,int);
void read_measures();
void turn_on_light();
void scan_LED_turn_on();
void check_moisture_value(float moisture);
void trial_version();
void EEPROM_WriteString(uint16_t v_eepromAddress_u16, uint8_t *ptr_stringPointer_u8);
void EEPROM_ReadString(uint16_t v_eepromAddress_u16, uint8_t *ptr_destStringAddress_u8);
void EEPROM_WriteByte(uint16_t v_eepromAddress_u16, uint8_t v_eepromData_u8);
uint8_t EEPROM_ReadByte(uint16_t v_eepromAddress_u16);

unsigned char onmsg[] = "ON\n";
unsigned char offmsg[] = "OFF\n";
unsigned char defaultmsg[] = "LED Status:";
unsigned char rxdata;

int main()
{
	//rtc_t rtc;
	//DDRB |= (1<<PB1);
	//DDRB |= (1<<PB2);
	//RTC_Init();
	//usart_init();
	//ADC_Init();
	//char array[10];
	//char time_array[5];
	//char date_array[8];
	//int adc_value;
	//float moisture;
	//float moisture_values;
	//int i = 0;
	
	//Buttons init
	//DDRD &= ~(1<<PD1) | ~(1<<PD2);
	//PORTD |= (1<<PD1) | (1<<PD2);
	
	//LEDs init
	//DDRB |= (1<<PB1) | (1<<PB2) | (1<<PB3);
	//PORTB &= ~(1<<PB1) | ~(1<<PB2) | ~(1<<PB3);
	
	
	 

	 //rtc.hour = 0x16; //  10:40:20 am
	 //rtc.min =  0x22;

	 //rtc.date = 0x2; //1st Jan 2016
	 //rtc.month = 0x01;
	 //rtc.year = 0x22;
	 //rtc.weekDay = 7; // Friday: 5th day of week considering monday as first day.
	 //RTC_SetDateTime(&rtc);
	
	//UWAGAAGAGAGAGA
	usart_init();
	ADC_Init();
	unsigned char array[4];
	unsigned char time_array[6] = ";18:03;";
	unsigned char date_array[8] = "09.01.22;";
	unsigned char return_value[21];
	int adc_value;
	char sign;
	float avs = 0;
	float moisture;
	float moisture_values;
	int i = 0;
	int j = 0;
	int n = 0;
	int a = 0;
	int b = 0;
	int eeprom_address = 0x00;
	int eeprom_address2 = 0x00;
	unsigned char read_str[50];
	
	//Buttons init
	DDRD &= ~(1<<PD1) | ~(1<<PD2);
	PORTD |= (1<<PD1) | (1<<PD2);
	
	//LEDs init
	DDRB |= (1<<PB1) | (1<<PB2) | (1<<PB3);
	
	//turn_on_light();
	sendstr("START\n");
	eeprom_address = 0x00;
	
	while(1)
	{
			scan_LED_turn_on();
			avs = make_measure();
			//adc_value = ADC;
			//ADC_measure(0);
			/*
			if(avs <= 0)	moisture_values = 0;
			else
			{
				moisture = (avs*100.00)/550.00;
				moisture_values = moisture;
				dtostrf(moisture_values,3,2,array);
				strcat(array,"%\n");
			}
			*/	
			
			
			rxdata = receivebyte();
			if(rxdata == 'a')
			{
				PORTB &= ~(1 << PB1);
				PORTB &= ~(1 << PB2);
				PORTB &= ~(1 << PB3);
				sendstr("dupa\n");
				
				DELAY_sec(10);
				
				UART_Printf(" dupa\r\n");
				PORTB = (1 << PB1);
				PORTB = (1 << PB2);
				PORTB = (1 << PB3);
			}
			else if(rxdata == 'b')
			{
				
				//sendstr()
				moisture = (avs*100.00)/550.00;
				moisture_values = moisture;
				dtostrf(moisture_values,3,2,array);
				//strcat(array,"%\n");
				UART_Printf(" %f\n",avs);
				DELAY_sec(15);
				//sendstr(array);
				UART_Printf(array);
				//UART_Printf("%f\n",avs);
				//set_sleep_mode(SLEEP_MODE_IDLE);
				DELAY_sec(15);
				//}
			}
			else if(rxdata == 'c')
			{
				eeprom_address = 0x00;
				for(a = 0; a < 3; a++)
				{
					avs = make_measure();
					moisture = (avs*100.00)/550.00;
					moisture_values = moisture;
					dtostrf(moisture_values,3,2,array);
					
					if(a < 2)
					{
						/*
						for(i = 0; i < 8; i++)
						{
							return_value[i] = date_array[i];
						}
						for(i = 0, j = 9; j < 27 && i < 7; i++, j++)
						{
							return_value[j] = time_array[i];
						}
						for(i = 0, j = 19; j < 27 && i < 8; i++, j++)
						{
							return_value[j] = array[i];
						}
						*/
						//return_value = {";18:03;09.01.22;00.00"};
						EEPROM_WriteString(eeprom_address, ";18:03;09.01.22;00.00"); // Write the string at memoryLocation	0x00
						eeprom_address++;
					}
				}
				
				eeprom_address = 0x00;
				
				//for(i = 0; i < 3; i++)
				//{
					EEPROM_ReadString(eeprom_address, return_value); // Read the string from memoryLocation 0x00
					sendstr(return_value);
					DELAY_sec(15);
					UART_Printf(" %s",return_value);
					//sendstr("\n");
					//eeprom_address++;
					DELAY_sec(15);
				//}				
			}
			else
			{}
				
		}
		//check_moisture_value(moisture_values);
		return 0;
}

void usart_init(void){
	UBRR0H= (unsigned char)(UBRRVAL>>8);   //high byte
	UBRR0L=(unsigned char)UBRRVAL;     			//low byte
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0);		//Enable Transmitter and Receiver
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00); 	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit siz
}

void sendbyte(unsigned char MSG){
	while((UCSR0A & (1<<UDRE0)) == 0);     // Wait if a byte is being transmitted
	UDR0 = MSG;
}

void sendstr(unsigned char *s){
	unsigned char i = 0;
	while(s[i] != '\0'){
		sendbyte(s[i]);
		i++;
	}
}

unsigned char receivebyte(void){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void ADC_Init(void)
{
	DDRC |= 0x0;		/*  Make ADC port as input  */
	ADCSRA = 0x87;		/*  Enable ADC, fr/128  */
}

void ADC_measure(uint8_t channel)
{
	ADMUX |= (0<<REFS1) | (1<<REFS0);
	ADCSRA |= (1<<ADEN) | (0<<ADIE) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
	ADMUX = ((ADMUX&0xE0)+channel);
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
};

float make_measure()
{
	int adc_value = ADC;
	float moisture;
	ADC_measure(0);
	
	if(adc_value <= 0) moisture = 0;
	else moisture = (adc_value*100.00)/550.00;
	
	return moisture;
}

void save_measures(float moisture, int numberOfMeasure)
{
	eeprom_write_float((uint8_t*)46 + (4*numberOfMeasure), moisture);
}

void read_measures()
{
	uint8_t i;
	char array[10];
	
	for (i = 0; i <= 23; i++)
	{
		dtostrf(eeprom_read_float((uint8_t*)46+(4*i)),3,2,array);
		strcat(array,"%\n");
		sendstr(array);
	}
}

void turn_on_light()
{
	PORTB = (1 << PB1);
	DELAY_ms(4000);
	PORTB &= ~(1 << PB1);
	DELAY_ms(4000);
	PORTB |= (1 << PB2);
	DELAY_ms(4000);
	PORTB &= ~(1 << PB2);
	DELAY_ms(4000);
	PORTB |= (1 << PB3);
	DELAY_ms(4000);
	PORTB &= ~(1 << PB3);
	DELAY_ms(4000);
}

void check_moisture_value(float moisture)
{
	PORTB &= ~(1 << PB3) | ~(1 << PB1) | ~(1 << PB2);
	
	if(moisture <= 30 && moisture >= 0)
	{
		PORTB = (1 << PB3);
	}
	else if(moisture > 30 && moisture <= 60)
	{
		PORTB |= (1 << PB3) | (1 << PB1);
		PORTB &= ~(1 << PB2);
	}
	else
	{
		PORTB = (1 << PB1);
	}
}

void scan_LED_turn_on()
{
	PORTB |= (1 << PB3) | (1 << PB1) | (1 << PB2);
}

/*

void trial_version()
{
	while(1)
	{
				
		for(n = 0; n < 3; n++)
		{
			scan_LED_turn_on();
			
			DELAY_ms(10000);
			
			adc_value = ADC;
			ADC_measure(0);
			if(adc_value <= 0)	moisture_values = 0;
			else
			{
				moisture = (adc_value*100.00)/550.00;
				moisture_values = moisture;
				dtostrf(moisture_values,3,2,array);
				strcat(array,"%\n");	
			}
			
			if(a < 2)
			{
				for(i = 0; i < 11; i++)
				{
					return_value[i] = date_array[i];
				}
				for(i = 0, j = 11; j < 27 && i < 8; i++, j++)
				{
					return_value[j] = time_array[i];
				}
				for(i = 0, j = 19; j < 27 && i < 8; i++, j++)
				{
					return_value[j] = array[i];
				}
				EEPROM_WriteString(eeprom_address, return_value); // Write the string at memoryLocation	0x00
				eeprom_address++;
				a++;
			}
			else
			{
				eeprom_address = 0x00;
				while (n < 2)
				{
					EEPROM_ReadString(eeprom_address, return_value); // Read the string from memoryLocation 0x00
					sendstr(return_value); //Print the message on UART
					sendstr("Proba\n");
					DELAY_ms(5000);
					n = n+1;
					eeprom_address++;
					if(n == 2) break;
				}
			}
		}
			
		//check_moisture_value(moisture_values);
		DELAY_ms(10000);
	}
}
*/


void EEPROM_WriteByte(uint16_t v_eepromAddress_u16, uint8_t v_eepromData_u8)
{
	while(util_IsBitSet(EECR,EEPE)); // Wait for completion of previous write.
	                                 // EEWE will be cleared by hardware once Eeprom write is completed.

	EEAR = v_eepromAddress_u16;  //Load the eeprom address and data
	EEDR = v_eepromData_u8;

	util_BitSet(EECR,EEMPE);    // Eeprom Master Write Enable
	util_BitSet(EECR,EEPE);     // Start eeprom write by setting EEWE
}






/***************************************************************************************************
               uint8_t EEPROM_ReadByte(uint16_t v_eepromAddres_u16)
 ***************************************************************************************************
 * I/P Arguments: uint16_t: eeprom_address from where eeprom_data is to be read.
 * Return value	: uint8_t: data read from Eeprom.

 * description: This function is used to read the data from specified EEPROM_address.        
 ***************************************************************************************************/
uint8_t EEPROM_ReadByte(uint16_t v_eepromAddress_u16)
{
	while(util_IsBitSet(EECR,EEPE));  //Wait for completion of previous write if any.

	EEAR = v_eepromAddress_u16;    //Load the address from where the data needs to be read.
	util_BitSet(EECR,EERE);   // start eeprom read by setting EERE

	return EEDR;             // Return data from data register
}

void EEPROM_WriteString(uint16_t v_eepromAddress_u16, uint8_t *ptr_stringPointer_u8)
{
	do
	{
		EEPROM_WriteByte(v_eepromAddress_u16,*ptr_stringPointer_u8); //Write a byte from RAM to EEPROM
		ptr_stringPointer_u8++;								//Increment the RAM Address
		v_eepromAddress_u16++;								//Increment the Eeprom Address
	}while(*(ptr_stringPointer_u8-1) !=0);
}




/***************************************************************************************************
void EEPROM_ReadString(uint16_t v_eepromAddress_u16, uint8_t *ptr_destStringAddress_u8)
 ***************************************************************************************************
 * I/P Arguments: uint16_t,: eeprom_address from where the String is to be read.
                  char*: Pointer into which the String is to be read.

 * Return value	: none

 * description:This function is used to Read a String from specified EEPROM_address.
           The string read from eeprom will be copied to specified buffer along with NULL character
 ***************************************************************************************************/

void EEPROM_ReadString(uint16_t v_eepromAddress_u16, uint8_t *ptr_destStringAddress_u8)
{
	char eeprom_data;

	do
	{
		eeprom_data = EEPROM_ReadByte(v_eepromAddress_u16); //Read a byte from EEPROM to RAM
		*ptr_destStringAddress_u8 = eeprom_data;			 //Copy the data into String Buffer
		ptr_destStringAddress_u8++;						 //Increment the RAM Address
		v_eepromAddress_u16++;							 //Increment the Eeprom Address
	}while(eeprom_data!=0);
}

