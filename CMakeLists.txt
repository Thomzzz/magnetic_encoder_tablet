cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

# Set name and standards
project(blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


#set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_executable(magnetic_encoder_tablet
        magnetic_encoder_tablet.c
        as5600_encoder.c
        )

#target_sources(magnetic_encoder_tablet PRIVATE magnetic_encoder_tablet.c)

# pull in common dependencies and additional i2c hardware support
target_link_libraries(magnetic_encoder_tablet 
                        pico_stdlib 
                        hardware_i2c
                        )

# create map/bin/hex file etc.
pico_add_extra_outputs(magnetic_encoder_tablet)

pico_enable_stdio_usb(magnetic_encoder_tablet 1)
pico_enable_stdio_uart(magnetic_encoder_tablet 0)
