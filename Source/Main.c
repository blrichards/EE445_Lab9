// ******** Lab3_Main.c **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 9/18/18
// Description: Main file for Lab 3
// Lab Number: Lab03
// TA: Zee Lv
// Date of last revision: 9/19/2018
// Hardware Configuration: N/A

#include "ADC.h"
#include "Debug.h"
#include "Graphics.h"
#include "PLL.h"
#include "ST7735.h"
#include "Timer.h"
#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

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

#define PF2 (*((volatile uint32_t*)0x40025010))
#define PF1 (*((volatile uint32_t*)0x40025008))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void); // Enable interrupts
long StartCritical(void); // previous I bit, disable interrupts
void EndCritical(long sr); // restore I bit to previous value
void WaitForInterrupt(void); // low power mode


int main(void)
{
	PLL_Init(Bus80MHz); // 80 MHz
    SYSCTL_RCGCGPIO_R |= 0x32; // activate port F, E, B
	while ((SYSCTL_PRGPIO_R&0x32) != 0x32) {}
	
    // configure PF2 as GPIO
    Debug_Init();
    Output_Init();
	Timer0A_Init(7999999);
	ADC_Init();
    EnableInterrupts();
	Graphics_ClearDisplay();
	ST7735_SetCursor(0,0); 
	ST7735_OutString("EE445L Lab9");
	
	while (true) {
		ST7735_SetCursor(0,1); 
		ST7735_OutString("Current ADC Value = ");	//Current ADC Value
		int32_t ADCVal = ADC0_CurrentValue();
		ST7735_OutUDec(ADCVal);
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
		ST7735_OutUDec(temp/100);		//Output temp maybe in float (use lab 3 or something)
		ST7735_OutString(".");
		ST7735_OutUDec(temp%100);
	}
		
	/*
    while (true) {
		ST7735_PlotClear(1000,4000);  // TODO - make a better clear function
		ST7735_SetCursor(0,1); 
		ST7735_OutString("Current ADC Value = ");	//Current ADC Value
		ST7735_OutUDec(0);							//TODO - add current ADC Value
		ST7735_SetCursor(0,2); 
		ST7735_OutString("T=");		//TODO Converted Temperature
		ST7735_OutUDec(2500);		//Output temp maybe in float (use lab 3 or something)

		
		if (fifoFull) {
			for(int i = 0; i < FIFO_SIZE; i++){
				//TODO Plot
			}
		}
    }
	*/
}

