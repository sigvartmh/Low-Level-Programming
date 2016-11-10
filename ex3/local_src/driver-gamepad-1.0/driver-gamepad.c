/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/interrupt.h>

#include "efm32gg.h"

#define DRIVER_NAME "Gamepad"
#define GPIO_IRQEVEN 	17
#define GPIO_IRQODD		18
#define GPIO_IF			((volatile uint32_t *)(GPIO_PA_BASE + 0x114))

static dev_t			devnr;
struct cdev				my_cdev;
struct fasync_struct 	*async_queue;
struct class			*cl;


static int my_open(struct inode *inode, struct file *filp) {
	return 0;
}

static int my_release(struct inode *inode, struct file *filp) {
	return 0;
}

static ssize_t my_read(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	uint32_t data = ioread32(GPIO_PC_DIN);
	copy_to_user(buff, &data, 4);
	return 4;
}

static ssize_t my_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
	printk("Cannot write!");
	return 0;
}

static int my_fasync(int fd, struct file *flip, int mode) {
	return fasync_helper(fd, flip, mode , &async_queue);
}

static struct file_operations my_fops = {
	.owner		= THIS_MODULE,
	.read		= my_read,
	.write		= my_write,
	.open		= my_open,
	.release	= my_release,
	.fasync		= my_fasync
};

irqreturn_t gpio_interrupt_handler(int irq, void *devID, struct pt_regs *regs) {
	iowrite32(ioread32(GPIO_IF), GPIO_IFC);
	if(async_queue) {
		kill_fasync(&async_queue, SIGIO, POLL_IN);
	}
	return IRQ_HANDLED;
}


/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init template_init(void) {
	printk("Loading gamepad module...\n");
	
	int allocationStatus = alloc_chrdev_region(&devnr, 0, 1, DRIVER_NAME);
	if(allocationStatus < 0) {
		printk("Failed to allocate device numbers\n");
		return -1;
	}
	
	if(request_mem_region(GPIO_PC_MODEL, 1, DRIVER_NAME) == NULL) {
		printk("Failed to map memory region GPIO_PC_MODEL\n");
		return -1;
	}
	
	if(request_mem_region(GPIO_PC_DOUT, 1, DRIVER_NAME) == NULL) {
		printk("Failed to map memory region GPIO_PC_DOUT\n");
		return -1;
	}
	
	if(request_mem_region(GPIO_PC_DIN, 1, DRIVER_NAME) == NULL) {
		printk("Failed to map memory region GPIO_PC_DIN\n");
		return -1;
	}
	
	iowrite32(0x33333333, GPIO_PC_MODEL);
	iowrite32(0xFF, GPIO_PC_DOUT);
	
	iowrite32(0x22222222, GPIO_EXTIPSELL);
	iowrite32(0xFF, GPIO_EXTIRISE);
	iowrite32(0xFF, GPIO_EXTIFALL);
	iowrite32(0xFF, GPIO_IEN);
	iowrite32(0x802, ISER0);
	
	request_irq(GPIO_IRQODD, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &devnr);
	request_irq(GPIO_IRQEVEN, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &devnr);
	
	cdev_init(&my_cdev, &my_fops);
	my_cdev.owner = THIS_MODULE;
	int err = cdev_add(&my_cdev, devnr, 1);
	
	if(err < 0) {
		printk("Failed to add cdev");
		return -1;
	}
	
	cl = class_create(THIS_MODULE, DRIVER_NAME);
	device_create(cl, NULL, devnr, NULL, DRIVER_NAME);
	
	printk("Successfully loaded\n");
	
	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void) {
	 printk("Short life for a small module...\n");
	 
	 release_mem_region(GPIO_PC_MODEL, 1);
	 release_mem_region(GPIO_PC_DOUT, 1);
	 release_mem_region(GPIO_PC_DIN, 1);
	 
	 device_destroy(cl, devnr);
	 class_destroy(cl);
	 cdev_del(&my_cdev);
	 
	 unregister_chrdev_region(devnr, 1);
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

