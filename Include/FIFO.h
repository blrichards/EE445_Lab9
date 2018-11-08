// ******** FIFO.h **************
// Authors: Ryan Mitchell and Ben Richards
// Initial Creation Date: 11/07/2018
// Description: Header file for FIFO.c
// Lab Number: Lab09
// TA: Zee Lv
// Date of last revision: 11/07/2018
// Hardware Configuration: N/A

#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stdbool.h>

#define FIFO_SIZE 64
extern uint16_t currentCursor;
extern uint16_t lastFifoValue;
extern bool fifoFull;

int32_t FIFO_Push(int32_t value);
int32_t FIFO_Pop(void);

#endif // ADC_H_
