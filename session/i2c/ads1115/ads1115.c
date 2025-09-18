#include "ads1115.h"
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h> // usleep

// Convert PGA enum to voltage
static float ADS1115_PGA_to_VFSR(ADS1115_PGA pga) {
    switch(pga) {
        case ADS1115_PGA_6_144V: return 6.144;
        case ADS1115_PGA_4_096V: return 4.096;
        case ADS1115_PGA_2_048V: return 2.048;
        case ADS1115_PGA_1_024V: return 1.024;
        case ADS1115_PGA_0_512V: return 0.512;
        case ADS1115_PGA_0_256V: return 0.256;
        default: return 2.048;
    }
}

int ADS1115_Init(ADS1115 *dev, int bus, uint8_t addr, ADS1115_PGA pga, ADS1115_Mode mode) {
    dev->i2cHandle = i2cOpen(bus, addr, 0);
    if (dev->i2cHandle < 0) return -1;
    
	dev->vfsr = ADS1115_PGA_to_VFSR(pga);
    
	return 0;
}

void ADS1115_Close(ADS1115 *dev) {
    i2cClose(dev->i2cHandle);
    gpioTerminate();
}

int16_t ADS1115_ReadRaw(ADS1115 *dev, uint8_t channel) {
    // Configuración single-shot, AINx, ±FS, 128SPS
    uint16_t config = 0x8000; // OS = 1 (start single)
    config |= (0x04 + channel) << 12; // MUX bits
    config |= 0x0183; // PGA ±4.096V, MODE single, DR=128SPS

    // Ajustar endianess
    uint16_t config_swapped = (config >> 8) | (config << 8);

    i2cWriteWordData(dev->i2cHandle, ADS1115_REG_CONFIG, config_swapped);

    // Esperar la conversión (~8ms a 128SPS)
    usleep(10000); // 10ms

    uint16_t result = i2cReadWordData(dev->i2cHandle, ADS1115_REG_CONVERSION);
    result = (result >> 8) | (result << 8); // ajustar endianess
    return (int16_t)result;
}

double ADS1115_ReadVoltage(ADS1115 *dev, uint8_t channel) {
    int16_t raw = ADS1115_ReadRaw(dev, channel);
    return (double)raw * dev->vfsr / 32768.0;
}
