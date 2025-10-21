#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/gpio.h>		// Linux Kernel GPIO Module

#define DEVICE_NAME "ledbutton"

// See /sys/kernel/debug/gpio to get GPIO pin numbers
#define LED_GPIO 	517 // GPIO_5
#define BUTTON_GPIO 518 // GPIO_6

// IOCTL
#define LED_ON  _IO('a',1)
#define LED_OFF _IO('a',2)
#define BUTTON_READ _IOR('a',3,int32_t*)


static int major;
static struct cdev c_dev;

static int dev_open(struct inode *inode, struct file *file) { return 0; }
static int dev_release(struct inode *inode, struct file *file) { return 0; }
static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int val;
    switch(cmd){
        case LED_ON:
            gpio_set_value(LED_GPIO, 1);
            break;
        case LED_OFF:
            gpio_set_value(LED_GPIO, 0);
            break;
        case BUTTON_READ:
            val = gpio_get_value(BUTTON_GPIO);
            if (copy_to_user((int32_t*)arg, &val, sizeof(val)))
            {
                printk(KERN_WARNING "ledbutton: Error reading BUTTON\n");
                return -EACCES;
            }
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

// ioctl structure
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .unlocked_ioctl = dev_ioctl,
};

static int __init ledbutton_init(void)
{
    int err;
    if ( (err = gpio_request_one(BUTTON_GPIO, GPIOF_IN | GPIOF_PULL_DOWN, "BUTTON")) != 0)
    {
        printk(KERN_ERR "ledbutton: No se pudo configurar GPIO %d, error %d\n", BUTTON_GPIO, err);
        return -1;
    }
    
    if ( (err = gpio_request(LED_GPIO, "LED")) != 0 )
    {
        printk(KERN_ERR "ledbutton: No se pudo configurar GPIO %d, error %d\n", LED_GPIO, err);
        return -1;
    }
    
    gpio_direction_output(LED_GPIO, 0);
    gpio_direction_input(BUTTON_GPIO);

    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0)
    {
        printk(KERN_ERR "ledbutton: No se pudo configurar Device %s\n", DEVICE_NAME);
        return major;
    }

    printk(KERN_INFO "ledbutton: Driver ioctl cargado\n");
    
    return 0;
}

static void __exit ledbutton_exit(void)
{
    gpio_set_value(LED_GPIO, 0);
    gpio_direction_input(LED_GPIO);
    gpio_free(LED_GPIO);
    gpio_free(BUTTON_GPIO);
    
    unregister_chrdev(major, DEVICE_NAME);
    gpio_free(LED_GPIO);
    gpio_free(BUTTON_GPIO);
    printk(KERN_INFO "ledbutton: Driver ioctl descargado\n");
}

module_init(ledbutton_init);
module_exit(ledbutton_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Universidad Panamericana");
MODULE_DESCRIPTION("Driver LED y Boton con IOCTL");
