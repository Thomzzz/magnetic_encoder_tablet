#include "as5600_encoder.h"

static const uint8_t I2cAddr = 0x36;
static const uint32_t I2cFreqHz = 400000;


static const uint8_t I2c0SclPin = 5;
static const uint8_t I2c0SdaPin = 4;

static const uint8_t I2c1SclPin = 7;
static const uint8_t I2c1SdaPin = 6;

void configureI2c(i2c_inst_t *i2cInst, uint8_t sdaPin, uint8_t sclPin)
{
  i2c_init(i2cInst, I2cFreqHz);
  gpio_set_function(sdaPin, GPIO_FUNC_I2C);
  gpio_set_function(sclPin, GPIO_FUNC_I2C);
  gpio_pull_up(sdaPin);
  gpio_pull_up(sclPin);
}

void as5600_init()
{
    configureI2c(I2c0Inst, I2c0SdaPin, I2c0SclPin);
    configureI2c(I2c1Inst, I2c1SdaPin, I2c1SclPin);

    // TODO: Configure settings
}

SStatus as5600_get_status(i2c_inst_t *i2cInst)
{
    uint8_t buffer;
    uint8_t reg = 0x0B;
    i2c_write_blocking(i2cInst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(i2cInst, I2cAddr, &buffer, 1, false);
    return (SStatus)buffer;
}

uint16_t as5600_read_raw(i2c_inst_t *i2cInst)
{
    uint8_t buffer[2];
    uint8_t reg = 0x0C;
    i2c_write_blocking(i2cInst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(i2cInst, I2cAddr, buffer, 2, false);
    return (((buffer[0] << 8) & 0x0F00) | buffer[1]);
}

uint16_t as5600_read(i2c_inst_t *i2cInst)
{
    uint8_t buffer[2];
    uint8_t reg = 0x0E;
    i2c_write_blocking(i2cInst, I2cAddr, &reg, 1, true);
    i2c_read_blocking(i2cInst, I2cAddr, buffer, 2, false);
    return (((buffer[0] << 8) & 0x0F00) | buffer[1]);
}