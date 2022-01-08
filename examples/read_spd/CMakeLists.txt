cmake_minimum_required(VERSION 3.13)

add_executable(read_spd
    read_spd.cpp
    ${PROJECT_SOURCE_DIR}/spd_rw/spd_rw.cpp
    )

pico_enable_stdio_usb(read_spd 1)
pico_enable_stdio_uart(read_spd 0)

pico_add_extra_outputs(read_spd)

target_include_directories(read_spd
    PRIVATE ${PROJECT_SOURCE_DIR}
    )

target_link_libraries(read_spd pico_stdlib hardware_i2c)
