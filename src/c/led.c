#include "led.h"

/* 
 * Replace these with your actual memory-map defines.
 * For example, if your top.v maps a 32-bit LED register at 0x1001_0000:
 */
#define LED_REG   (*(volatile uint32_t*)0x10010000)

void led_init(void) {
    /* Configure LED pin as output. 
       If you have a direction register, set it here instead. */
    // e.g. GPIO_DIR |= (1 << LED_PIN);
}

void led_toggle(void) {
    /* Flip the LED bit */
    LED_REG ^= 1;
}
