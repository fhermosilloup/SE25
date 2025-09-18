#ifndef ADS1115_H
#define ADS1115_H

#include <stdint.h>

// ADS1115 I2C default address
#define ADS1115_ADDR 0x48

// Registro
#define ADS1115_REG_CONVERSION 0x00
#define ADS1115_REG_CONFIG     0x01

// PGA Full Scale Range
typedef enum {
    ADS1115_PGA_6_144V = 0,
    ADS1115_PGA_4_096V,
    ADS1115_PGA_2_048V,
    ADS1115_PGA_1_024V,
    ADS1115_PGA_0_512V,
    ADS1115_PGA_0_256V
} ADS1115_PGA;

// Modo de conversión
typedef enum {
    ADS1115_MODE_CONTINUOUS = 0,
    ADS1115_MODE_SINGLESHOT
} ADS1115_Mode;

// Handle del dispositivo
typedef struct {
    int i2cHandle;
    float vfsr; // full scale voltage
} ADS1115;

int ADS1115_Init(ADS1115 *dev, int bus, uint8_t addr, ADS1115_PGA pga, ADS1115_Mode mode);
void ADS1115_Close(ADS1115 *dev);
int16_t ADS1115_ReadRaw(ADS1115 *dev, uint8_t channel);
double ADS1115_ReadVoltage(ADS1115 *dev, uint8_t channel);

#endif
