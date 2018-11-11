// ******** ADC.c **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 11/07/2018
// Description: File for ADC functionality
// Lab Number: Lab09
// TA: Zee Lv
// Date of last revision: 11/07/2018
// Hardware Configuration: N/A

#include "ADC.h"
#include "tm4c123gh6pm.h"

int32_t result;

void ADC_Init(void)
{
    SYSCTL_RCGCADC_R |= 0x0001; // 7) activate ADC0
        // 1) activate clock for Port E
    //SYSCTL_RCGCGPIO_R |= 0x10;
    //while ((SYSCTL_PRGPIO_R & 0x10) != 0x10) {};
    GPIO_PORTE_DIR_R &= ~0x10; // 2) make PE4 input
    GPIO_PORTE_AFSEL_R |= 0x10; // 3) enable alternate function on PE4
    GPIO_PORTE_DEN_R &= ~0x10; // 4) disable digital I/O on PE4
    GPIO_PORTE_AMSEL_R |= 0x10; // 5) enable analog functionality on PE4

    //  while((SYSCTL_PRADC_R&0x0001) != 0x0001){};    // good code, but not yet
    //  implemented in simulator

    ADC0_PC_R &= ~0xF; // 7) clear max sample rate field
    ADC0_PC_R |= 0x1; //    configure for 125K samples/sec
    ADC0_SSPRI_R = 0x0123; // 8) Sequencer 3 is highest priority
    ADC0_ACTSS_R &= ~0x0008; // 9) disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000; // 10) seq3 is software trigger
    ADC0_SSMUX3_R &= ~0x000F; // 11) clear SS3 field
    ADC0_SSMUX3_R += 9; //    set channel
    ADC0_SSCTL3_R = 0x0006; // 12) no TS0 D0, yes IE0 END0
    ADC0_IM_R &= ~0x0008; // 13) disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0008; // 14) enable sample sequencer 3
    //ADC0_SAC_R = ADC_SAC_AVG_64X; //how much sampling for the ADC
}

//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
int32_t ADC0_InSeq3(void)
{
    ADC0_PSSI_R = 0x0008; // 1) initiate SS3
    while ((ADC0_RIS_R & 0x08) == 0) {} // 2) wait for conversion done
    // if you have an A0-A3 revision number, you need to add an 8 usec wait here
    result = ADC0_SSFIFO3_R & 0xFFF; // 3) read result
    ADC0_ISC_R = 0x0008; // 4) acknowledge completion

    return result;
}

int32_t ADC0_CurrentValue(void)
{
	return result;
}
