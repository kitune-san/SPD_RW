#pragma once
#include <array>
#include <stdint.h>
#include "hardware/i2c.h"

class SPD_RW
{
public:
  const static size_t memory_size = 512;
  const static size_t read_page_size = 256;
  const static size_t write_page_size = 16;
  const static uint8_t device_type = 0x0A;
  const static uint8_t register_device_type = 0x06;
  const static uint16_t twr_ms = 6;

  std::array<uint8_t, memory_size> memory;

  SPD_RW()
    : i2c(i2c_default), device_select(0x00) {};
  SPD_RW(i2c_inst_t *init_i2c, uint8_t init_device_select)
    : i2c(init_i2c), device_select(init_device_select) {};
  virtual ~SPD_RW() {};

  bool read();
  bool write();
  bool is_protected();
  bool set_protect();
  bool clear_protect();
  uint16_t crc16(size_t start, int count);

private:
  i2c_inst_t * const i2c;
  const uint8_t device_select;

  int command(uint8_t);
  int read_status(uint8_t);

  SPD_RW(const SPD_RW &) = delete;
  SPD_RW &operator=(const SPD_RW &) = delete;
};

