#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "spd_rw/spd_rw.hpp"

const uint LED_PIN = 25;

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
    if (spd.is_protected()) {
      printf("SPD is protected.\n");
    } else {
      printf("SPD is not protected.\n");
    }
    printf("\n");
    sleep_ms(1000);
  }

  return 0;
}
