#ifndef FT232RL_H_
#define FT232RL_H_

void FT232_init(void);
void FT232_sendData(
uint8_t h, uint8_t m, uint8_t s, uint16_t ms, uint16_t micros,
uint16_t spm,
int16_t ax, int16_t ay, int16_t az,
int16_t gx, int16_t gy, int16_t gz,
uint16_t count
);

#endif /* FT232RL_H_ */