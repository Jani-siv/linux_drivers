#include <linux/module.h>
#include <linux/fs.h>
//add header files to support character devices
#include <linux/cdev.h>
#include <linux/device.h>

//define names
#define DEVICE_NAME "mydev"
#define CLASS_NAME "hello_class"

static struct class* helloClass;
//creating cdev structure
static struct cdev my_dev;

dev_t dev;
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
	dev_t dev_no;
	int Major;

	struct device* helloDevice;
	pr_info("Hello world init\n");

	//allocate device numbers dynamically
	ret = alloc_chrdev_region(&dev_no,0, 1,DEVICE_NAME);
	if (ret < 0) {
		pr_info("unable to allocate mayor number\n");
		return ret;
	}
	Major = MAJOR(dev_no);
	dev = MKDEV(Major,0);

	pr_info("Allocated correctly with major number %d\n", Major);


	cdev_init(&my_dev, &my_dev_fops);
	ret = cdev_add(&my_dev,dev,1);
	if (ret < 0) {
		unregister_chrdev_region(dev,1);
		pr_info("unable to add cdev\n");
		return ret;
	}

	//register the device class
	helloClass = class_create(THIS_MODULE,CLASS_NAME);
	if (IS_ERR(helloClass)){
		cdev_del(&my_dev);
		unregister_chrdev_region(dev,1);
		pr_info("Failed to create the device\n");
		return PTR_ERR(helloClass);
	}
	pr_info("the class is created correctly");

	helloDevice = device_create(helloClass,NULL, dev, NULL,DEVICE_NAME);
	if (IS_ERR(helloDevice)) {
		class_destroy(helloClass);
		cdev_del(&my_dev);
		unregister_chrdev_region(dev,1);
		pr_info("Failed to create the device\n");
		return PTR_ERR(helloDevice);
	}
	pr_info("the device is created correctly");


	return 0;
}

static void __exit hello_exit(void)
{
	device_destroy(helloClass,dev);
	class_destroy(helloClass);
	cdev_del(&my_dev);
	unregister_chrdev_region(dev,1);
	pr_info("bye bye cruel world");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Liberal");
MODULE_DESCRIPTION("this module interacts with the ioctl system call");

