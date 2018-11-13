// ******** Timer.c **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 9/18/18
// Description: File for timer functionality
// Lab Number: Lab03
// TA: Zee Lv
// Date of last revision: 9/19/2018
// Hardware Configuration: N/A

#include "ADC.h"
#include "Timer.h"
#include "UART.h"
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include <stdint.h>

uint16_t const ADCdata[53] = {0, 183, 231, 278, 326, 375,
	426, 477, 529, 581, 635, 690, 746, 803, 862, 921, 981,
	1042, 1105, 1169, 1233, 1300, 1367, 1435, 1505, 1576, 1648, 
	1722, 1797, 1873, 1950, 2029, 2109, 2191, 2274, 2359, 2444,
	2532, 2621, 2711, 2803, 2896, 2991, 3088, 3186, 3285, 3386,
	3489, 3594, 3700, 3807, 3916, 4095};

uint16_t const Tdata[53] = {4000, 4000, 3950, 3900, 3850,
	3800, 3750, 3700, 3650, 3600, 3550, 3500, 3450, 3400, 
	3350, 3300, 3250, 3200, 3150, 3100, 3050, 3000, 2950, 
	2900, 2850, 2800, 2750, 2700, 2650, 2600, 2550, 2500, 
	2450, 2400, 2350, 2300, 2250, 2200, 2150, 2100, 2050, 
	2000, 1950, 1900, 1850, 1800, 1750, 1700, 1650, 1600, 
	1550, 1550, 1500};

#define PB7 (*((volatile uint32_t*)0x40005200))
#define PF2 (*((volatile uint32_t*)0x40025010))

#define GRAPH_OFFSET 50
#define GRAPH_HEIGHT (ST7735_TFTHEIGHT - GRAPH_OFFSET)
	
uint8_t CurrentSeconds = 0;
uint8_t CurrentMinutes = 0;
uint8_t CurrentHours = 0;

void SysTick_Init(void)
{
	NVIC_ST_CTRL_R = 0;           		// disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF;       	// reload value for 1kHz
	NVIC_ST_CURRENT_R = 0;        		// any write to current clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x00000000; // priority 1
	NVIC_ST_CTRL_R = 0x00000006;  		// enable interrupts, not clock
}

void SysTick_Handler(void){
    NVIC_ST_RELOAD_R = 0x00FFFFFF;     // reload value for high phase
}

void Timer0A_Init(uint32_t reloadValue)
{
    volatile uint32_t delay;
    SYSCTL_RCGCTIMER_R |= 0x01;			 	// activate timer0
    delay = SYSCTL_RCGCTIMER_R; 			// allow time to finish activating
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;		// disable timer0A during setup
    TIMER0_CFG_R = 0; 						// configure for 32-bit timer mode
	
    // configure for periodic mode
    TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
    TIMER0_TAILR_R = reloadValue; 			// start value for 1 Hz interrupts
    TIMER0_IMR_R |= TIMER_IMR_TATOIM; 		// enable timeout (rollover) interrupt
    TIMER0_ICR_R = TIMER_ICR_TATOCINT; 		// clear timer0A timeout flag
    TIMER0_CTL_R |= TIMER_CTL_TAEN; 		// enable timer0A 32-b, periodic, interrupts
    
    // Timer0A=priority 2
    NVIC_PRI4_R = (NVIC_PRI4_R & 0x00FFFFFF) | 0x00000000;//0x40000000; // top 3 bits
    NVIC_EN0_R = 1 << 19; // enable interrupt 19 in NVIC
}

void Timer0A_Handler(void)
{
	static uint8_t screenCursor = 0;
	static int32_t buffer[ST7735_TFTWIDTH] = {-1};
	static int32_t min = 5000;
	static int32_t max = 1500;
	
	ST7735_SetCursor(0,1); 
	ST7735_OutString("ADC Value = ");	//Current ADC Value
	int32_t ADCVal = ADC0_CurrentValue();
	ST7735_OutUDec(ADCVal);
	if(ADCVal < 1000) ST7735_OutString(" ");
	ST7735_SetCursor(0,2); 
	int i = 0;
	uint32_t temp;
	while(ADCdata[i] < ADCVal) i++;
	if(i == 0) temp = 40;
	else {
		double percent = (double)((ADCdata[i] - ADCVal)/(ADCdata[i] - ADCdata[i-1]));
		temp = (uint32_t)(Tdata[i-1] - (percent * (Tdata[i-1] - Tdata[i])));
	}
	ST7735_OutString("T = ");		//TODO Converted Temperature
	ST7735_OutUDec((temp)/100);		//Output temp maybe in float (use lab 3 or something)
	ST7735_OutString(".");
	ST7735_OutUDec((temp)%100);
	
    TIMER0_ICR_R = TIMER_ICR_TATOCINT; // acknowledge timer0A timeout
	
	
	if (buffer[screenCursor] >= GRAPH_OFFSET)
		ST7735_DrawPixel(screenCursor, buffer[screenCursor], ST7735_BLACK);
		
	
	int32_t currentValue = ADC0_InSeq3();
	/*
	UART_OutUDec((uint32_t)currentValue);
	UART_OutChar('\n');
	*/
	
	if (currentValue < min)
		min = currentValue;
	if (currentValue > max)
		max = currentValue;
	
	currentValue -= min;
	int32_t range = max - min;
	int32_t yValue = (((double)currentValue/(double)range) * GRAPH_HEIGHT) + GRAPH_OFFSET;
	if (yValue < GRAPH_OFFSET)
		yValue = 0;
	buffer[screenCursor] = yValue;
	if (yValue >= GRAPH_OFFSET)
		ST7735_DrawPixel(screenCursor, yValue, ST7735_WHITE);
	screenCursor = (screenCursor + 1) % ST7735_TFTWIDTH;
}

void Timer1_Init(uint32_t reloadValue)
{
    volatile uint32_t delay;
    SYSCTL_RCGCTIMER_R |= 0x02; // 0) activate TIMER1
    delay = SYSCTL_RCGCTIMER_R; // allow time to finish activating
    TIMER1_CTL_R = 0x00000000; // 1) disable TIMER1A during setup
    TIMER1_CFG_R = 0x00000000; // 2) configure for 32-bit mode
    TIMER1_TAMR_R = 0x00000002; // 3) configure for periodic mode, down-count
    TIMER1_TAILR_R = reloadValue; // 4) reload value
    TIMER1_TAPR_R = 0; // 5) bus clock resolution
    TIMER1_CTL_R = 0x00000001; // 10) enable TIMER1A
}
