#ifndef __ERROR_H
#define __ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

void handle_error(void);
void assert_failed(uint8_t *file, uint32_t line);

#ifdef __cplusplus
}
#endif

#endif /* __ERROR_H */
