#ifndef AS5600_ENCODER
#define AS5600_ENCODER

#include "pico/stdlib.h"
#include "hardware/i2c.h"

static i2c_inst_t *I2c0Inst = i2c0;
static i2c_inst_t *I2c1Inst = i2c1;

typedef union Status
{
  struct
  {
    uint8_t : 3; // Reserved
    uint8_t IsTooStrong : 1;
    uint8_t IsTooWeak : 1;
    uint8_t IsDetected : 1;
    uint8_t : 2; // Reserved
  };

  uint8_t RawData;

} SStatus;

_Static_assert(sizeof(SStatus) == 1); // Make sure status uses only 1 byte

void as5600_init();
void as5600_print_configuration(i2c_inst_t *i2cInst);
SStatus as5600_get_status(i2c_inst_t *i2cInst);
uint16_t as5600_read_raw(i2c_inst_t *i2cInst);
uint16_t as5600_read(i2c_inst_t *i2cInst);

#endif