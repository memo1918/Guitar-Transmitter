#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
  const uint LED_PIN = 17;
  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  while (true)
  {
    gpio_put(LED_PIN, 1);
    sleep_ms(500);
    gpio_put(LED_PIN, 0);
    sleep_ms(500);
    printf("Hello World");
  }
}