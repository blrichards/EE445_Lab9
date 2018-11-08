// ******** FIFO.c **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 11/07/2018
// Description: Code for basic FIFO functionality
// Lab Number: Lab09
// TA: Zee Lv
// Date of last revision: 11/07/2018
// Hardware Configuration: N/A

#include <stdint.h>
#include "FIFO.h"

int32_t FIFO[FIFO_SIZE] = {-1};
uint16_t currentCursor = 0;
uint16_t lastFifoValue = 0;
bool fifoFull = false;

int32_t FIFO_Push(int32_t value)
{
	uint16_t checkFull = (lastFifoValue + 1) % FIFO_SIZE;	//Look to see if the next iteration hits the cursor
	if(checkFull == currentCursor) return 0;				//If full return 0 = false
	lastFifoValue = checkFull;								//If good incrament the end value
	FIFO[lastFifoValue] = value;							//Set the new value
	return 1;												//return success
}

int32_t FIFO_Pop(void)
{
	if(currentCursor == lastFifoValue) return 0;	//if there is nothing in the Fifo then don't pop anything
	int32_t returnValue = FIFO[currentCursor];		//Get the current value
	currentCursor = (currentCursor + 1) % FIFO_SIZE;//Incrament the cursor
	return returnValue;								//return the value
}
