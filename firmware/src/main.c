#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "init.h"

int main(void)
{
	system_clock_init();
	gpio_init();

	while (1)
	{

	}
}

void handle_error(void)
{
	
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif /* USE_FULL_ASSERT */
