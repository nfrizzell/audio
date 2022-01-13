#include <linux/module.h>

static int __init telegraph_init(void)
{
	return 0;
}

static void __exit telegraph_exit(void)
{
	
}

module_init(telegraph_init);
module_exit(telegraph_exit);

MODULE_AUTHOR("nfrizzell");
MODULE_LICENSE("GPL");
