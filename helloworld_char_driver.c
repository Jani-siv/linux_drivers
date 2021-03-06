#include <linux/module.h>

//add header files to support character devices
#include <linux/cdev.h>
#include <linux/fs.h>

//define major number
//
#define MY_MAJOR_NUM	202
//creating cdev structure
static struct cdev my_dev;
//https://tldp.org/LDP/lkmpg/2.4/html/c577.htm
//callback functions that are defined in the struct file_operations structure:
//you see (struct inode *inode, struct file *file) from
//int (*open) (struct inode *, struct file *);
static int my_dev_open(struct inode *inode, struct file *file)
{
	pr_info("my_dev_open() is called.\n");
	return 0;
}
static int my_dev_close(struct inode *inode, struct file *file)
{
	pr_info("my_dev_close() is called.\n");
	return 0;
}
static long my_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	pr_info("my_dev_ioctl() is called. cmd = %d, arg = %ld\n", cmd, arg);
	return 0;
}

//declare a file operations structure

static const struct file_operations my_dev_fops = {
	.owner = THIS_MODULE,
	.open = my_dev_open,
	.release = my_dev_close,
	.unlocked_ioctl = my_dev_ioctl,
};

static int __init hello_init(void)
{
	int ret;
	//MKDEV — Creates a value that can be compared to a kernel device number
	//dev_t MKDEV(long major, long minor)
	dev_t dev = MKDEV(MY_MAJOR_NUM, 0);
	
	pr_info("hello world init\n");

	//allocate device numbers
	//int register_chrdev_region (	dev_t from,unsigned count,const char * name);
	//range of devices what needed in this practice we need one
	ret = register_chrdev_region(dev, 1,"my_char_device");
	if (ret < 0) {
		pr_info("unable to allocate mayor number %d\n", MY_MAJOR_NUM);
		return ret;
	}

	//init the cdev structure and add it to the kernel space
	//void cdev_init (	struct cdev * cdev,const struct file_operations * fops);
	//fops is file_operations structure
	cdev_init(&my_dev, &my_dev_fops);
	//cdev_add — add a char device to the system
	//int cdev_add (	struct cdev * p,dev_t dev,unsigned count);

	ret = cdev_add(&my_dev,dev,1);
	if (ret < 0) {
		unregister_chrdev_region(dev,1);
		pr_info("unable to add cdev\n");
		return ret;
	}
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("bye bye cruel world");
	cdev_del(&my_dev);
	unregister_chrdev_region(MKDEV(MY_MAJOR_NUM,0),1);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Liberal");
MODULE_DESCRIPTION("this module interacts with the ioctl system call");

