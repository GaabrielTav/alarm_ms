#include "pico/stdlib.h"
#include <stdio.h>

 #define LED_BLUE_PIN 11    
#define LED_RED_PIN 12     
#define LED_GREEN_PIN 13   
#define BUTTON_PIN 5       

 typedef enum {
    STATE_OFF,      
    STATE_ALL_ON,   
    STATE_TWO_ON,   
    STATE_ONE_ON,   
} LedState;

 LedState current_state = STATE_OFF;  
bool button_pressed = false;         
bool timer_active = false;           

 void initialize_components() {

     gpio_init(LED_BLUE_PIN);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);

     gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

     gpio_put(LED_BLUE_PIN, 0);
    gpio_put(LED_RED_PIN, 0);
    gpio_put(LED_GREEN_PIN, 0);
}

 int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    // Altera o estado dos LEDs
    switch (current_state) {
        case STATE_ALL_ON:
            gpio_put(LED_BLUE_PIN, 0); 
            current_state = STATE_TWO_ON;
             add_alarm_in_ms(3000, turn_off_callback, NULL, false);
            break;
        case STATE_TWO_ON:
            gpio_put(LED_RED_PIN, 0);  
            current_state = STATE_ONE_ON;
             add_alarm_in_ms(3000, turn_off_callback, NULL, false);
            break;
        case STATE_ONE_ON:
            gpio_put(LED_GREEN_PIN, 0);  
            current_state = STATE_OFF;
            timer_active = false;  
            break;
        default:
            break;
    }

     return 0;
}

 int main() {
     stdio_init_all();

     initialize_components();

     while (true) {
         if (!gpio_get(BUTTON_PIN) && !button_pressed && !timer_active) {
            button_pressed = true;  

             gpio_put(LED_BLUE_PIN, 1);
            gpio_put(LED_RED_PIN, 1);
            gpio_put(LED_GREEN_PIN, 1);
            current_state = STATE_ALL_ON;

             add_alarm_in_ms(3000, turn_off_callback, NULL, false);
            timer_active = true;  
        }

         if (gpio_get(BUTTON_PIN) && button_pressed) {
            button_pressed = false;  
        }

         sleep_ms(10);
    }

    return 0;
}