#ifndef __INIT_H
#define __INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

void system_clock_init(void);
void gpio_init(void);

#ifdef __cplusplus
}
#endif

#endif // __INIT_H
