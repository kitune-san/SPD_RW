#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "spd_rw/spd_rw.hpp"

const uint LED_PIN = 25;

void memory_dump(const SPD_RW &spd)
{
  for (size_t i = 0; spd.memory_size > i; i+=16) {
    printf("0x%04x : ", i);
    for (size_t j = 0; 16 > j; j++) {
      printf("%02x ", spd.memory.at(i+j));
    }
    printf("\n");
  }
  printf("\n");
  return;
}

int main() {
  bi_decl(bi_program_description("Read SPD."));

  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, 1);
  bi_decl(bi_1pin_with_name(LED_PIN, "On-bord LED"));

  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  //gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  //gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

  SPD_RW spd(i2c_default, 0x00);

  while (1) {
    // Read original data
    if (!spd.read()) {
      printf("SPD Access is denied.\n");
      sleep_ms(1000);
      return 0;
    }
    printf("\nRead OK.\n");
    memory_dump(spd);

    sleep_ms(1000);

    do {
      printf("\nPress Enter.\n");
    } while ('\r' != getchar());

    printf("\nStart write test.\n");
    std::array<uint8_t, spd.memory_size> backup = spd.memory;

    for (size_t i = 0; spd.memory_size> i; i++) {
      spd.memory.at(i) = 1 + i;
    }
    if (!spd.write()) {
      printf("SPD Access is denied.\n");
      sleep_ms(1000);
      return 0;
    }
    printf("\nWrite OK.\n");

    for (size_t i = 0; spd.memory_size> i; i++) {
      spd.memory.at(i) = 0;
    }
    if (!spd.read()) {
      printf("SPD Access is denied.\n");
      sleep_ms(1000);
      return 0;
    }
    printf("\nRead OK.\n");
    memory_dump(spd);

    printf("\nRestoration.\n");
    spd.memory = backup;

    if (!spd.write()) {
      printf("SPD Access is denied.\n");
      sleep_ms(1000);
      return 0;
    }
    printf("\nWrite OK.\n");

    for (size_t i = 0; spd.memory_size> i; i++) {
      spd.memory.at(i) = 0;
    }
  }

  return 0;
}
