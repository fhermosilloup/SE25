/******************************************************
 * Ejemplo: Comunicación kernel con user space via /proc files
 * Autor: Universidad Panamericana
 * Descripción: Crea /proc/lkm_info para intercambio simple
 ******************************************************/

/* ===================== BIBLIOTECAS ===================== */
#include <linux/init.h>		// Inicialización y salida de módulos
#include <linux/module.h>	// Módulo del kernel
#include <linux/kernel.h>	// Funciones del kernel (printk, pr_info, etc.)
#include <linux/proc_fs.h>	// API de /proc
#include <linux/uaccess.h>	// Transferencia de datos entre kernel y user space


/* ===================== DEFINES Y VARIABLES ===================== */
#define PROC_NAME "lkm_info"   // Nombre del archivo en /proc
#define MAX_LEN 64             // Tamaño máximo del mensaje

static char message[MAX_LEN] = "Hola desde el kernel!\n"; // Mensaje inicial


/* ===================== FUNCIÓN DE LECTURA ===================== */
// Se ejecuta cuando el usuario lee /proc/lkm_info: 
// @example cat /proc/lkm_info
static ssize_t lkm_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    /* Copia datos del kernel hacia el user space */
    return simple_read_from_buffer(buf, count, pos, message, strlen(message));
}


/* ===================== FUNCIÓN DE ESCRITURA ===================== */
// Se ejecuta cuando el usuario escribe en /proc/lkm_info,
// @example	echo "Hola" > /proc/lkm_info
static ssize_t lkm_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
    if (count > MAX_LEN - 1)
        return -EINVAL; // Longitud inválida

    /* Copiar desde user space hacia buffer del kernel */
    if (copy_from_user(message, buf, count))
        return -EFAULT; // Error de copia

    message[count] = '\0'; // Terminar cadena
    pr_info("Nuevo mensaje desde user space: %s\n", message);
    return count;
}


/* ================ ESTRUCTURA DE OPERACIONES WRITE/READ ================ */
static const struct proc_ops proc_fops = {
    .proc_read  = lkm_read,
    .proc_write = lkm_write,
};


/* ===================== INICIALIZACIÓN DEL MÓDULO ===================== */
static int __init proc_module_init(void)
{
    pr_info("Creando archivo /proc/%s\n", PROC_NAME);

    /* Crear el archivo en /proc */
    if (!proc_create(PROC_NAME, 0666, NULL, &proc_fops)) {
        pr_err("Error al crear /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    pr_info("Archivo /proc/%s creado correctamente\n", PROC_NAME);
    return 0;
}


/* ===================== SALIDA DEL MÓDULO ===================== */
static void __exit proc_module_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("Archivo /proc/%s eliminado\n", PROC_NAME);
}


/* ===================== METADATOS DEL MÓDULO ===================== */
module_init(proc_module_init);
module_exit(proc_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Universidad Panamericana");
MODULE_DESCRIPTION("LKM ejemplo comunicación con user space mediante /proc");
