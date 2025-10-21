#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define LED_ON  _IO('a',1)
#define LED_OFF _IO('a',2)
#define BUTTON_READ _IOR('a',3,int)

int main() {
    int fd = open("/dev/ledbutton", O_RDWR);
    int val;

    ioctl(fd, LED_ON);
    sleep(1);
    ioctl(fd, LED_OFF);

    ioctl(fd, BUTTON_READ, &val);
    printf("Button value: %d\n", val);

    close(fd);
    return 0;
}
