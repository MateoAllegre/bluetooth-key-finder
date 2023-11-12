#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <zephyr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int output_init(void);
void output_set_led(bool state);
void output_set_buzzer(int value);

#ifdef __cplusplus
}
#endif

#endif /* OUTPUT_H_ */