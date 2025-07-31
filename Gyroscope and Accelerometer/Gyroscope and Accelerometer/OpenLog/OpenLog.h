#ifndef OPENLOG_H_
#define OPENLOG_H_
#include "../REGISTER/REGISTER.h"
void OpenLog_init(void);
void OpenLog_appendFile(const char* filename);
void OpenLog_writeLine(const char* data);
void OpenLog_write(const char* data);
void OpenLog_LogFullLine(uint8_t h, uint8_t m, uint8_t s, uint16_t ms, uint16_t micros, uint16_t spm, int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz, uint16_t count, float p, float r);

#endif /* OPENLOG_H_ */