
/*
dtoverlay=maxtherm,spi0-0,max6675,type_k
dtoverlay=i2c-sensor,mpu6050,addr=0x68,int_pin=GPIO
*/
// Cargamos el driver ADS1115 en /boot/firmware/config.txt
// dtoverlay=ads1115
// sudo reboot
// Si esta conectado, debería de aparecer en ls /sys/bus/iio/devices/


#include <stdio.h>
#include <stdlib.h>
#define DEV0_TEMP_RAW "/sys/bus/iio/devices/iio:device0/in_temp_raw"
int main() {
	FILE *f = fopen(DEV0_TEMP_RAW, "r");
	if (!f) {
		perror("Failed to open Max6675");
		return 1;
	}

	int value;
	if (fscanf(f, "%d", &value) != 1) {
		perror("Failed to read value");
		fclose(f);
		return 1;
	}

	fclose(f);

	printf("ADC raw value: %d\n", value);
	return 0;
}
