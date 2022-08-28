#include "as5600_encoder.h"
#include <string.h>
#include <stdio.h>

static const uint8_t I2cAddr = 0x36;
static const uint32_t I2cFreqHz = 400000;
static const uint8_t I2c0SclPin = 5;
static const uint8_t I2c0SdaPin = 4;
static const uint8_t I2c1SclPin = 7;
static const uint8_t I2c1SdaPin = 6;

static const uint8_t RegConfig = 0x07;

typedef union Configuration
{
  struct
  {
    uint8_t SlowFilter : 2;
    uint8_t FastFilter : 3;
    uint8_t Watchdog : 1;
    uint8_t : 2;
    uint8_t PowerMode : 2;
    uint8_t Hysteresis: 2;
    uint8_t OutputStage : 2;
    uint8_t PwmFrequency: 2;
  };

  uint8_t RawData[2];

} SConfiguration;

_Static_assert(sizeof(SConfiguration) == 2);

void configureI2c(i2c_inst_t *i2cInst, uint8_t sdaPin, uint8_t sclPin)
{
  i2c_init(i2cInst, I2cFreqHz);
  gpio_set_function(sdaPin, GPIO_FUNC_I2C);
  gpio_set_function(sclPin, GPIO_FUNC_I2C);
  gpio_pull_up(sdaPin);
  gpio_pull_up(sclPin);
}

void as5600_set_configuration(i2c_inst_t *i2cInst, 
                              const uint8_t* slowFilter, 
                              const uint8_t* fastFilter, 
                              const uint8_t* hysteresis)
{
  // Read the current configuration
  uint8_t readConfig[2];
  i2c_write_blocking(i2cInst, I2cAddr, &RegConfig, 1, true);
  i2c_read_blocking(i2cInst, I2cAddr, readConfig, 2, false);

  // Change the configuration settings that are not NULL
  SConfiguration config;
  memcpy(&config, readConfig, 2);

  if(slowFilter != NULL) config.SlowFilter = *slowFilter;
  if(fastFilter != NULL) config.FastFilter = *fastFilter;
  if(hysteresis != NULL) config.Hysteresis = *hysteresis;
  
  memcpy(readConfig, &config, 2);

  // Write the new status back
  uint8_t writeBuffer[3] = {RegConfig, readConfig[0], readConfig[1]};
  i2c_write_blocking(i2cInst, I2cAddr, writeBuffer, 3, true);
}

void as5600_print_configuration(i2c_inst_t *i2cInst)
{
  uint8_t readConfig[2];
  i2c_write_blocking(i2cInst, I2cAddr, &RegConfig, 1, true);
  i2c_read_blocking(i2cInst, I2cAddr, readConfig, 2, false);

  SConfiguration config;
  memcpy(&config, readConfig, 2);

  printf("Config: SlowFilter = %d, FastFilter = %d, Hysteresis = %d\n", config.SlowFilter, config.FastFilter, config.Hysteresis);
}

void as5600_init()
{
    configureI2c(I2c0Inst, I2c0SdaPin, I2c0SclPin);
    configureI2c(I2c1Inst, I2c1SdaPin, I2c1SclPin);

    const uint8_t slowFilter = 0b11;
    const uint8_t fastFilter = 0b001;
    const uint8_t hysteresis = 0b00;

    as5600_set_configuration(I2c0Inst, &slowFilter, &fastFilter, &hysteresis);
    as5600_set_configuration(I2c1Inst, &slowFilter, &fastFilter, &hysteresis);
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