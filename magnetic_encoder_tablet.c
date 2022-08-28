#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

static const uint8_t I2cAddr = 0x36;
static const uint32_t I2cFreqHz = 400000;

static i2c_inst_t *I2c0Inst = i2c0;
static const uint8_t I2c0SclPin = 5;
static const uint8_t I2c0SdaPin = 4;

static i2c_inst_t *I2c1Inst = i2c1;
static const uint8_t I2c1SclPin = 7;
static const uint8_t I2c1SdaPin = 6;

static void as5600_init()
{
    i2c_init(I2c0Inst, I2cFreqHz);
    gpio_set_function(I2c0SdaPin, GPIO_FUNC_I2C);
    gpio_set_function(I2c0SclPin, GPIO_FUNC_I2C);
    gpio_pull_up(I2c0SdaPin);
    gpio_pull_up(I2c0SclPin);

    i2c_init(I2c1Inst, I2cFreqHz);
    gpio_set_function(I2c1SdaPin, GPIO_FUNC_I2C);
    gpio_set_function(I2c1SclPin, GPIO_FUNC_I2C);
    gpio_pull_up(I2c1SdaPin);
    gpio_pull_up(I2c1SclPin);

    // TODO: Configure settings
}

static uint8_t as5600_get_status(i2c_inst_t *I2cInst)
{
    uint8_t buffer;
    uint8_t reg = 0x0B;
    i2c_write_blocking(I2cInst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(I2cInst, I2cAddr, &buffer, 1, false);
    return buffer;
}

static uint16_t as5600_read_raw(i2c_inst_t *I2cInst)
{
    uint8_t buffer[2];
    uint8_t reg = 0x0C;
    i2c_write_blocking(I2cInst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(I2cInst, I2cAddr, buffer, 2, false);
    return (((buffer[0] << 8) & 0x0F00) | buffer[1]);
}

static uint16_t as5600_read(i2c_inst_t *I2cInst)
{
    uint8_t buffer[2];
    uint8_t reg = 0x0E;
    i2c_write_blocking(I2cInst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(I2cInst, I2cAddr, buffer, 2, false);
    return (((buffer[0] << 8) & 0x0F00) | buffer[1]);
}

int main()
{
    stdio_init_all();
    as5600_init();

    uint8_t status0 = 0;
    uint8_t status1 = 0;
    uint16_t angle0 = 0;
    uint16_t angle1 = 0;

    while (1) 
    {
        status0 = as5600_get_status(I2c0Inst);
        if (status0 & (1 << 4)) printf("Status0: Magnet too weak\n");
        if (status0 & (1 << 3)) printf("Status0: Magnet too strong\n");
        if (status0 & (1 << 5)) printf("Status0: Magnet detected\n");

        angle0 = as5600_read_raw(I2c0Inst);
        printf("Angle0 value = %d\n", angle0);

        status1 = as5600_get_status(I2c1Inst);
        if (status1 & (1 << 4)) printf("Status1: Magnet too weak\n");
        if (status1 & (1 << 3)) printf("Status1: Magnet too strong\n");
        if (status1 & (1 << 5)) printf("Status1: Magnet detected\n");

        angle1 = as5600_read_raw(I2c1Inst);
        printf("Angle1 value = %d\n", angle1);
        printf("\n");

        sleep_ms(100);
    }

    return 0;
}
