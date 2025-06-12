#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>

/* I defined these to match my FPGA’s GPIO layout */
#define LED_BASE_ADDR   0x10010000U
#define LED_PIN         0U    /* bit index for my LED pin */

/* I set up the LED pin so I can turn it on or off */
void led_init(void);

/* I drive the pin high to light the LED */
void led_on(void);

/* I drive the pin low to turn the LED off */
void led_off(void);

/* I toggle the LED between on and off */
void led_toggle(void);

#endif /* LED_H */
