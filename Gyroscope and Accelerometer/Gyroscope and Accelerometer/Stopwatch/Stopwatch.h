#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <stdint.h>

void Stopwatch_Init(void);
void Stopwatch_Start(void);
void Stopwatch_Stop(void);
void Stopwatch_Reset(void);
void Stopwatch_GetTime(uint8_t* h, uint8_t* m, uint8_t* s, uint16_t* ms, uint16_t* us);

#endif /* STOPWATCH_H_ */