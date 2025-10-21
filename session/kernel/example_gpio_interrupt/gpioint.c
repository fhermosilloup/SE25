#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/fs.h>

// See /sys/kernel/debug/gpio to get GPIO pin numbers
#define LED_GPIO 	517 // GPIO_5
#define BUTTON_GPIO 518 // GPIO_6

static unsigned int button_irq;
static int led_state = 0;

// Interrupcion: invocada al presionar el boton
static irqreturn_t button_irq_handler(int irq, void *dev_id)
{
    led_state = !led_state;
    gpio_set_value(LED_GPIO, led_state);
    printk(KERN_INFO "Button pressed! LED is now %d\n", led_state);
    return IRQ_HANDLED;
}

static int __init gpio_irq_init(void)
{
    int ret;

    // Configurar LED como salida
    ret = gpio_request_one(LED_GPIO, GPIOF_OUT_INIT_LOW, "LED");
    if (ret) {
        printk(KERN_ERR "Failed to request LED GPIO %d\n", LED_GPIO);
        return ret;
    }

    // Configurar boton como entrada
    ret = gpio_request_one(BUTTON_GPIO, GPIOF_IN, "BUTTON");
    if (ret) {
        printk(KERN_ERR "Failed to request BUTTON GPIO %d\n", BUTTON_GPIO);
        gpio_free(LED_GPIO);
        return ret;
    }

    // Obtener numero de IRQ asociado al GPIO
    button_irq = gpio_to_irq(BUTTON_GPIO);
    if (button_irq < 0) {
        printk(KERN_ERR "Failed to get IRQ for BUTTON GPIO\n");
        gpio_free(LED_GPIO);
        gpio_free(BUTTON_GPIO);
        return button_irq;
    }

    // Solicitar interrupcion: trigger en flanco descendente (ej. boton activo bajo)
    ret = request_irq(button_irq,
                      button_irq_handler,
                      IRQF_TRIGGER_FALLING | IRQF_SHARED,
                      "gpio_button_irq",
                      (void *)(button_irq));
    if (ret) {
        printk(KERN_ERR "Failed to request IRQ %d\n", button_irq);
        gpio_free(LED_GPIO);
        gpio_free(BUTTON_GPIO);
        return ret;
    }

    printk(KERN_INFO "GPIO IRQ driver loaded\n");
    return 0;
}

static void __exit gpio_irq_exit(void)
{
    free_irq(button_irq, (void *)(button_irq));
    gpio_set_value(LED_GPIO, 0);
    gpio_free(LED_GPIO);
    gpio_free(BUTTON_GPIO);
    printk(KERN_INFO "GPIO IRQ driver unloaded\n");
}

module_init(gpio_irq_init);
module_exit(gpio_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Universidad Panamericana");
MODULE_DESCRIPTION("Ejemplo de GPIO con interrupcion");
