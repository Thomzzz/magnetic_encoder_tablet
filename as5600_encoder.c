#include "as5600_encoder.h"

static const uint8_t I2cAddr = 0x36;
static const uint32_t I2cFreqHz = 400000;


static const uint8_t I2c0SclPin = 5;
static const uint8_t I2c0SdaPin = 4;

static const uint8_t I2c1SclPin = 7;
static const uint8_t I2c1SdaPin = 6;

void as5600_init()
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

uint8_t as5600_get_status(i2c_inst_t *I2cInst)
{
    uint8_t buffer;
    uint8_t reg = 0x0B;
    i2c_write_blocking(I2cInst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(I2cInst, I2cAddr, &buffer, 1, false);
    return buffer;
}

uint16_t as5600_read_raw(i2c_inst_t *I2cInst)
{
    uint8_t buffer[2];
    uint8_t reg = 0x0C;
    i2c_write_blocking(I2cInst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(I2cInst, I2cAddr, buffer, 2, false);
    return (((buffer[0] << 8) & 0x0F00) | buffer[1]);
}

uint16_t as5600_read(i2c_inst_t *I2cInst)
{
    uint8_t buffer[2];
    uint8_t reg = 0x0E;
    i2c_write_blocking(I2cInst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(I2cInst, I2cAddr, buffer, 2, false);
    return (((buffer[0] << 8) & 0x0F00) | buffer[1]);
}