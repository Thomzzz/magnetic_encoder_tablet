#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "as5600_encoder.h"

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
