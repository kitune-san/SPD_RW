#include "spd_rw.hpp"

uint16_t SPD_RW::crc16(size_t start, int count)
{
  uint16_t crc;
  size_t ptr;

  crc = 0;
  ptr = start;

  while (--count >= 0) {
    crc = crc ^ memory.at(ptr) << 8;
    ++ptr;
    for (int i = 0; i < 8; ++i) {
      if (crc & 0x8000) {
        crc = crc << 1 ^ 0x1021;
      } else {
        crc = crc <<1;
      }
    }
  }
  return (crc & 0xFFFF);
}

bool SPD_RW::read()
{
  for (size_t addr = 0; memory_size > addr; addr += read_page_size) {
    // Sets page address.
    if (command(0b110 | ((addr >> 8) & 0x01)) < 0) {
      //throw std::runtime_error("SPD Access is denied.");
      return false;
    }

    // Sets word address.
    uint8_t device_addr = (device_type << 3) | (device_select & 0x07);
    uint8_t send_data = 0x00;
    if (i2c_write_blocking(i2c, device_addr, &send_data, 1, true) < 0) {
      //throw std::runtime_error("SPD Access is denied.");
      return false;
    }

    // Starts sequential read.
    size_t size = memory.size();
    if (size <= addr) {
      break;
    } else if ((size - addr) < read_page_size) {
      size = size - addr;
    } else {
      size = read_page_size;
    }
    uint8_t *read_ptr = &memory.data()[addr];
    if (i2c_read_blocking(i2c, device_addr, read_ptr, size, false) < 0) {
      //throw std::runtime_error("SPD Access is denied.");
      return false;
    }
  }

  return true;
}

bool SPD_RW::write()
{
  // Starts to write data.
  for (size_t addr = 0; memory_size > addr; addr += write_page_size) {
    // Sets page address.
    if (command(0b110 | ((addr >> 8) & 0x01)) < 0) {
      //throw std::runtime_error("SPD Access is denied.");
      return false;
    }

    // Starts page write.
    uint8_t device_addr = (device_type << 3) | (device_select & 0x07);
    uint8_t send_data[1 + write_page_size] = {0};
    send_data[0] = addr;
    for (size_t i = 0; write_page_size > i; i++) {
      send_data[1 + i] = memory.at(addr + i);
    }
    if (i2c_write_blocking(i2c, device_addr, send_data, sizeof(send_data), false) < 0) {
      //throw std::runtime_error("SPD Access is denied.");
      return false;
    }
    // write wait.
    sleep_ms(twr_ms);
  }

  return true;
}

bool SPD_RW::is_protected()
{
  if (read_status(0b001) < 0) {
    // is protected.
    return true;
  }
  if (read_status(0b100) < 0) {
    // is protected.
    return true;
  }
  if (read_status(0b101) < 0) {
    // is protected.
    return true;
  }
  if (read_status(0b000) < 0) {
    // is protected.
    return true;
  }
  // is not protected.
  return false;
}

bool SPD_RW::clear_protect()
{
  // Clears all write protect.
  if (command(0b011) < 0) {
    //throw std::runtime_error("SPD Access is denied.");
    return false;
  }
  return true;
}

bool SPD_RW::set_protect()
{
  // Sets all write protects.
  // SWP0
  if (command(0b001) < 0) {
    //throw std::runtime_error("SPD Access is denied.");
    return false;
  }
  // SWP1
  if (command(0b100) < 0) {
    //throw std::runtime_error("SPD Access is denied.");
    return false;
  }
  // SWP2
  if (command(0b101) < 0) {
    //throw std::runtime_error("SPD Access is denied.");
    return false;
  }
  // SWP3
  if (command(0b000) < 0) {
    //throw std::runtime_error("SPD Access is denied.");
    return false;
  }
  return true;
}

int SPD_RW::command(uint8_t cmd)
{
    uint8_t device_addr = (register_device_type << 3) | (cmd & 0x07);
    uint8_t send_data[2] = {0};

    return i2c_write_blocking(i2c, device_addr, send_data, sizeof(send_data), false);
}

int SPD_RW::read_status(uint8_t cmd)
{
    uint8_t device_addr = (register_device_type << 3) | (cmd & 0x07);
    uint8_t recieved_data[2] = {0};

    return i2c_read_blocking(i2c, device_addr, recieved_data, sizeof(recieved_data), false);
}

