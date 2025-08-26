#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define LED_PIN 16

float core_read_temperature(void)
{
  FILE *fp;
  char buf[128];
  float temp = 0.0F;

  // Open vcgencmd to read the temperature
  fp = popen("vcgencmd measure_temp", "r");
  if (fp == NULL) {
    return -1.0F;
  }

  // Get the result
  if (fgets(buf, sizeof(buf), fp) != NULL) {
    // convert to float
    sscanf(buf, "temp=%f", &temp);
    printf("Temperatura CPU: %.2f °C\n", temp);
  }
  
  pclose(fp);

  return temp;
}

int main(void)
{
	if( gpioInitialise() < 0)
	{
		printf("Failed to initialize piGPIO\n");
		return -1;
	}

	
	
	gpioTerminate();
	
	return 0;
}
