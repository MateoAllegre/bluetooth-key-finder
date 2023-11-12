#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <zephyr/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int system_init(void);
void system_set_debug(const char *debug_message);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_H_ */
