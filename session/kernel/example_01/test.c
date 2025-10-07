// Bibliotecas básicas
#include <linux/kernel.h>  //
#include <linux/init.h> //
#include <linux/module.h> //

/* Función que se ejecuta cuando se instala el driver con insmod */
static int __init my_driver_init(void)
{
        printk(KERN_INFO "Hello world from kernel space!\n");
        return 0;
}

/* Función que se ejecuta cuando se desinstala el driver con rmmod */
static void __exit my_driver_exit(void)
{
        printk(KERN_INFO "Godbye from kernel space\n");
}

/* Registro de ambas funciones */
module_init(my_driver_init);
module_exit(my_driver_exit);

/* Descripción del driver */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Universidad Panamericana");
MODULE_DESCRIPTION("The hello world of a Linux kernel module");
MODULE_VERSION("1.0");

