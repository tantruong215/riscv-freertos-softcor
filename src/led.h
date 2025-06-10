#ifndef LED_H
#define LED_H

#include <stdint.h>

/**
 * @brief  Configure the LED GPIO pin as output.
 */
void led_init(void);

/**
 * @brief  Toggle the LED pin.
 */
void led_toggle(void);

#endif // LED_H
