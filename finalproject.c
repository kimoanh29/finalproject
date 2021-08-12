#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic16f877a.h>
#include "lcd.h"

#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program M
#define _XTAL_FREQ 16000000
void ADC_Init();
void testwaterlevel();
unsigned int ADC_Read(unsigned char channel);
void read_ldr();

void main()
{
    TRISA=0xFF;
    TRISB=0x00;
    TRISD=0x00;
    TRISC=0x00;
    PORTB=0x00;
    ADC_Init();
    Lcd_Start();
    while (1)
    {
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Print_String("Water Level");
        testwaterlevel();
        read_ldr();
       
    }
}

void ADC_Init()
{
  ADCON0 = 0x81;               
  ADCON1 = 0x00;               
}
unsigned int ADC_Read(unsigned char channel)
{
  if(channel > 7)              
    return 0;
  ADCON0 &= 0xC5;              
  ADCON0 |= channel<<3;        
  __delay_ms(2);               
  GO_nDONE = 1;                
  while(GO_nDONE !=0);             
  return ((ADRESH<<8)+ADRESL); 
}

double a;
void testwaterlevel() {
    
    unsigned int adc_value = 0;
    adc_value = ADC_Read(0);
    a = (adc_value/1024);
 
    if (a==0) {
        PORTB = 0x00;
        RB7 = 1;
        Lcd_Set_Cursor(2,1);
        Lcd_Print_String("LEVEL 1");
        __delay_ms(10);
        
    }
    
    if (a >=20 && a < 35) {
        PORTB = 0x00;
        RB6 = 1;
        Lcd_Set_Cursor(2,1);
        Lcd_Print_String("LEVEL 2");
        __delay_ms(10);
    }
    if (a > 35) {
        PORTB = 0x00;
        RB5 = 1;
        RB4 = 1;
        Lcd_Set_Cursor(2,1);
        Lcd_Print_String("WARNING!!!");
        __delay_ms(10);
    }
}
double light;
void read_ldr()
{
unsigned int adc_value=0;
adc_value= ADC_Read(1);
light = 100 - adc_value/(1024);
if(light<=50) // SWITCH of the light when light is 80 percent
{
RB3=1;
Lcd_Set_Cursor(2,1);
Lcd_Print_String("Warning!!!");
}
else
{
    PORTB=0x00;

}
}
