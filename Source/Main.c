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
#include "FIFO.h"
#include "Graphics.h"
#include "PLL.h"
#include "ST7735.h"
#include "Timer.h"
#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

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
	while((SYSCTL_PRGPIO_R&0x32) != 0x32){}
	
    // configure PF2 as GPIO
    Debug_Init();
    Output_Init();
	Timer0A_Init(79999999);
	ADC_Init();
    EnableInterrupts();
	Graphics_ClearDisplay();
	ST7735_SetCursor(0,0); 
	ST7735_OutString("EE445L Lab9");
	
    while (true) {
		ST7735_PlotClear(1000,4000);  // Could make a better clear function
		ST7735_SetCursor(0,1); 
		ST7735_OutString("N=");		//Current ADC Value
		ST7735_SetCursor(0,2); 
		ST7735_OutString("T=");		//TODO Converted Temperature
		ST7735_OutUDec(2500);		//Output temp maybe in float (use lab 3 or something)

		
		if(fifoFull){
			for(int i = 0; i < FIFO_SIZE; i++){
				//TODO Plot
			}
		}
    }
}

