#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

#define DEVICE_NAME "mydevice"
#define CLASS_NAME  "myclass"

#define MY_IOCTL_MAGIC  'x'        // Identificador mágico
#define IOCTL_SET_VALUE _IOW(MY_IOCTL_MAGIC, 1, int)  // Escribir valor (user → kernel)
#define IOCTL_GET_VALUE _IOR(MY_IOCTL_MAGIC, 2, int)  // Leer valor (kernel → user)

static int value = 0;
static dev_t dev;
static struct cdev my_cdev;
static struct class *my_class;

/* --- open (IOCTL file is opened) --- */
static int my_open(struct inode *inode, struct file *file)
{
    pr_info("mydevice: abierto\n");
    return 0;
}

/* --- release (IOCTL file is released) --- */
static int my_release(struct inode *inode, struct file *file)
{
    pr_info("mydevice: cerrado\n");
    return 0;
}

/* --- ioctl (Command Processing function) --- */
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
    case IOCTL_SET_VALUE:
        if (copy_from_user(&value, (int __user *)arg, sizeof(value)))
            return -EFAULT;
        pr_info("mydevice: valor establecido a %d\n", value);
        break;

    case IOCTL_GET_VALUE:
        if (copy_to_user((int __user *)arg, &value, sizeof(value)))
            return -EFAULT;
        pr_info("mydevice: valor enviado: %d\n", value);
        break;

    default:
        return -EINVAL;
    }

    return 0;
}

/* --- file_operations --- */
static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = my_open,
    .release        = my_release,
    .unlocked_ioctl = my_ioctl,
};

/* --- init --- */
static int __init my_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
    if (ret < 0) return ret;

    cdev_init(&my_cdev, &fops);
    ret = cdev_add(&my_cdev, dev, 1);
    if (ret < 0) return ret;

    my_class = class_create(THIS_MODULE, CLASS_NAME);
    device_create(my_class, NULL, dev, NULL, DEVICE_NAME);

    pr_info("mydevice: módulo cargado correctamente\n");
    return 0;
}

/* --- exit --- */
static void __exit my_exit(void)
{
    device_destroy(my_class, dev);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);

    pr_info("mydevice: módulo descargado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tu Nombre");
MODULE_DESCRIPTION("Ejemplo simple de ioctl en kernel");
