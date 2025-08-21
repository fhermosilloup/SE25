#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define LED_PIN 16

int main(void)
{
	if( gpioInitialise() < 0)
	{
		printf("Failed to initialize piGPIO\n");
		return -1;
	}

	gpioSetMode(LED_PIN, PI_OUTPUT);

	for(int i = 0; i < 10; i++)
	{
		gpioWrite(LED_PIN, 1);
		gpioDelay(500000);
		gpioWrite(LED_PIN, 0);
		gpioDelay(500000);
	}
	
	gpioTerminate();
	
	return 0;
}
