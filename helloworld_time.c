#include <linux/module.h>
//#include <linux/time.h>

static int num = 10;
//setting variable time64_t type
static time64_t start_time, end_time;

static void say_hello(void)
{
	int i;
	for (i = 1; i <= num; i++)
		pr_info("[%d%d] Hello!\n",i,num);
}
//init module and set start time
static int __init first_init(void)
{
	start_time = ktime_get_seconds();
	pr_info("loading first\n");
	say_hello();
	return 0;
}
//exit module and set end time
static void __exit first_exit(void)
{
	
	end_time = ktime_get_seconds();
	pr_info("unloading module after %lld seconds\n",end_time - start_time);
	say_hello();
}
module_init(first_init);
module_exit(first_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("alberto liberal <aliberal@arroweurope.com>");
MODULE_DESCRIPTION("this is a print out hello world module");

