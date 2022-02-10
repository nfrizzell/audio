#include "error.h"

void handle_error(void)
{
	
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif /* USE_FULL_ASSERT */
