#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

static const uint8_t I2cAddr = 0x36;

static i2c_inst_t *I2c0Inst = i2c0;
static const uint8_t I2c0SclPin = 5;
static const uint8_t I2c0SdaPin = 4;

static i2c_inst_t *I2c1Inst = i2c1;
static const uint8_t I2c1SclPin = 7;
static const uint8_t I2c1SdaPin = 6;

static uint8_t as5600_get_status()
{
    uint8_t buffer;
    uint8_t reg = 0x0B;
    i2c_write_blocking(I2c0Inst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(I2c0Inst, I2cAddr, &buffer, 1, false);
    return buffer;
}

static uint16_t as5600_read_raw()
{
    uint8_t buffer[2];
    uint8_t reg = 0x0C;
    i2c_write_blocking(I2c0Inst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(I2c0Inst, I2cAddr, buffer, 2, false);
    return (((buffer[0] << 8) & 0x0F00) | buffer[1]);
}

static uint16_t as5600_read()
{
    uint8_t buffer[2];
    uint8_t reg = 0x0E;
    i2c_write_blocking(I2c0Inst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(I2c0Inst, I2cAddr, buffer, 2, false);
    return (((buffer[0] << 8) & 0x0F00) | buffer[1]);
}

int main()
{
    stdio_init_all();

    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(I2c0Inst, 400 * 1000);
    gpio_set_function(I2c0SdaPin, GPIO_FUNC_I2C);
    gpio_set_function(I2c0SclPin, GPIO_FUNC_I2C);
    gpio_pull_up(I2c0SdaPin);
    gpio_pull_up(I2c0SclPin);

    // Make the I2C pins available to picotool
    //bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    uint8_t status = 0;
    uint16_t angle = 0;

    while (1) 
    {
        status = as5600_get_status();
        if (status & (1 << 4)) printf("Status: Magnet too weak\n");
        if (status & (1 << 3)) printf("Status: Magnet too strong\n");
        if (status & (1 << 5)) printf("Status: Magnet detected\n");

        angle = as5600_read_raw();
        printf("Angle value = %d\n", angle);

        sleep_ms(100);
    }

    return 0;
}
