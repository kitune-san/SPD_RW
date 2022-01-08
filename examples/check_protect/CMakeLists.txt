cmake_minimum_required(VERSION 3.13)

add_executable(check_protect
    check_protect.cpp
    ${PROJECT_SOURCE_DIR}/spd_rw/spd_rw.cpp
    )

pico_enable_stdio_usb(check_protect 1)
pico_enable_stdio_uart(check_protect 0)

pico_add_extra_outputs(check_protect)

target_include_directories(check_protect
    PRIVATE ${PROJECT_SOURCE_DIR}
    )

target_link_libraries(check_protect pico_stdlib hardware_i2c)
