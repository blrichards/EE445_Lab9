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

#define TEMP_DATA_SIZE 64

extern int32_t TempData[TEMP_DATA_SIZE];
extern uint8_t TempDataLastIdx;
extern uint8_t TempDataSize;
extern int32_t TempDataMovingAverage;

void TempData_Process(int32_t value);
int32_t TempData_MovingAverage(void);

#endif // ADC_H_
