#include <linux/module.h>

static int __init hello_init(void)
{
	pr_info("Hello world init\n");
	return 0;
}
static void __exit hello_exit(void)
{
	pr_info("bye cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alberto liberal <aliberal@arroweurope.com>");
MODULE_DESCRIPTION("this is a print out hello world module");
