cmake_minimum_required(VERSION 3.13)

add_executable(write_test
    write_test.cpp
    ${PROJECT_SOURCE_DIR}/spd_rw/spd_rw.cpp
    )

pico_enable_stdio_usb(write_test 1)
pico_enable_stdio_uart(write_test 0)

pico_add_extra_outputs(write_test)

target_include_directories(write_test
    PRIVATE ${PROJECT_SOURCE_DIR}
    )

target_link_libraries(write_test pico_stdlib hardware_i2c)
