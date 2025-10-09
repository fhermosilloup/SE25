#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MY_IOCTL_MAGIC  'x'
#define IOCTL_SET_VALUE _IOW(MY_IOCTL_MAGIC, 1, int)
#define IOCTL_GET_VALUE _IOR(MY_IOCTL_MAGIC, 2, int)

int main(void)
{
    int fd;
    int value;

    // Abrir el dispositivo
    fd = open("/dev/mydevice", O_RDWR);
    if (fd < 0) {
        perror("Error al abrir /dev/mydevice");
        return -1;
    }

    // --- Enviar valor al kernel ---
    value = 42;
    if (ioctl(fd, IOCTL_SET_VALUE, &value) == -1) {
        perror("Error en IOCTL_SET_VALUE");
    } else {
        printf("Valor %d enviado al kernel\n", value);
    }

    // --- Leer valor desde el kernel ---
    value = 0;
    if (ioctl(fd, IOCTL_GET_VALUE, &value) == -1) {
        perror("Error en IOCTL_GET_VALUE");
    } else {
        printf("Valor recibido desde kernel: %d\n", value);
    }

    // Cerrar el dispositivo
    close(fd);
    return 0;
}
