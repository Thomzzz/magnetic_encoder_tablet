#ifndef AS5600_ENCODER
#define AS5600_ENCODER

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

static i2c_inst_t *I2c0Inst = i2c0;
static i2c_inst_t *I2c1Inst = i2c1;

void as5600_init();
uint8_t as5600_get_status(i2c_inst_t *I2cInst);
uint16_t as5600_read_raw(i2c_inst_t *I2cInst);
uint16_t as5600_read(i2c_inst_t *I2cInst);

#endif