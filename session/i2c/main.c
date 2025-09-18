#include <stdio.h>
#include <pigpio.h>

// Dirección I2C del ADS1115 (según el pin ADDR)
#define ADS1115_ADDR 0x48

// Registros ADS1115
#define ADS1115_REG_CONVERT   0x00
#define ADS1115_REG_CONFIG    0x01

// Configuración básica (lectura single-ended canal 0, 4.096V rango, 128 SPS)
#define CONFIG_OS_SINGLE   0x8000
#define CONFIG_MUX_AIN0    0x4000
#define CONFIG_PGA_4_096V  0x0200
#define CONFIG_MODE_SINGLE 0x0100
#define CONFIG_DR_128SPS   0x0080
#define CONFIG_COMP_DISABLE 0x0003

int main() {
    if (gpioInitialise() < 0) {
        printf("Error iniciando pigpio\n");
        return 1;
    }

    // Abrir bus I2C (1 = /dev/i2c-1)
    int handle = i2cOpen(1, ADS1115_ADDR, 0);
    if (handle < 0) {
        printf("Error abriendo I2C\n");
        gpioTerminate();
        return 1;
    }

    // Construir configuración
    int16_t config = CONFIG_OS_SINGLE | CONFIG_MUX_AIN0 | CONFIG_PGA_4_096V |
                     CONFIG_MODE_SINGLE | CONFIG_DR_128SPS | CONFIG_COMP_DISABLE;

    // Enviar configuración
    char buf[3];
    buf[0] = ADS1115_REG_CONFIG;
    buf[1] = (config >> 8) & 0xFF;
    buf[2] = config & 0xFF;
    i2cWriteDevice(handle, buf, 3);

    // Esperar conversión (~8 ms para 128 SPS)
    gpioDelay(10000);

    // Leer resultado
    i2cWriteByte(handle, ADS1115_REG_CONVERT);
    char data[2];
    i2cReadDevice(handle, data, 2);

    int16_t raw = (data[0] << 8) | data[1];
    double voltage = (raw * 4.096) / 32768.0; // en Voltios

    printf("Lectura canal 0: %d (%.6f V)\n", raw, voltage);

    i2cClose(handle);
    gpioTerminate();
    return 0;
}
