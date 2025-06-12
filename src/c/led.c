#include "led.h"

/* I don’t need a direction register on my board,
   but if you do, you can configure it here.
   For now, led_init() is just a placeholder. */

static inline volatile uint32_t *LED_REG(void) {
    return (volatile uint32_t *)LED_BASE_ADDR;
}

void led_init(void) {
    /* If my hardware required it, I would
       set the GPIO pin as output here. */
}

void led_on(void) {
    *LED_REG() |= (1U << LED_PIN);
}

void led_off(void) {
    *LED_REG() &= ~(1U << LED_PIN);
}

void led_toggle(void) {
    *LED_REG() ^= (1U << LED_PIN);
}
