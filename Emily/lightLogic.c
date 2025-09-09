#include <stdio.h>
#include <stdbool.h>

// ADC conversion and threshold definitions
#define ADC_MAX_VALUE 1023          // 10-bit ADC maximum
#define DARK_THRESHOLD 200          // Below this value = dark
#define MEDIUM_THRESHOLD 600        // Between dark and this = medium light
                                    // Above this = bright light

// Light level enumeration
typedef enum {
    LIGHT_DARK = 0,
    LIGHT_MEDIUM = 1,
    LIGHT_BRIGHT = 2
} light_level_t;

// Function prototypes
int read_ldr_value(void);
light_level_t determine_light_level(int ldr_value);
void handle_dark_mode(void);
void handle_medium_light_mode(void);
void handle_bright_light_mode(void);

/**
 * Read ADC value from light-dependent resistor
 * Returns: ADC value (0-1023 for 10-bit ADC)
 */
int read_ldr_value(void) {

    return 0; 
}

/**
 * Determine light level based on LDR reading
 * @param ldr_value: Raw ADC value from LDR
 * @return: Light level enumeration
 */
light_level_t determine_light_level(int ldr_value) {
    if (ldr_value < DARK_THRESHOLD) {
        return LIGHT_DARK;
    } else if (ldr_value < MEDIUM_THRESHOLD) {
        return LIGHT_MEDIUM;
    } else {
        return LIGHT_BRIGHT;
    }
}

/**
 * Handle behavior in full darkness
 * - Calm, no movement
 * - Minimal power consumption
 */
void handle_dark_mode(void) {
  // calm mode
  // happy noises
  // minimal movement

}

/**
 * Handle behavior in medium light
 * - Some sound, small movements
 * - Moderate activity level
 */
void handle_medium_light_mode(void) {

  // unhappy noises
  // some movement
}

/**
 * Handle behavior in bright light
 * - Lots of sound, large movements
 * - High activity level
 */
void handle_bright_light_mode(void) {
  // angry noises
  // lots of movement - thrashing around
    
}

/**
 * Main light logic processing function
 * Call this function regularly in your main loop
 */
void process_light_logic(void) {
    static light_level_t previous_level = LIGHT_DARK;
    
    // Read current light level
    int ldr_reading = read_ldr_value();
    light_level_t current_level = determine_light_level(ldr_reading);
    
    // Only change behavior if light level has changed
    if (current_level != previous_level) {

        
        switch (current_level) {
            case LIGHT_DARK:
                handle_dark_mode();
                break;
            case LIGHT_MEDIUM:
                handle_medium_light_mode();
                break;
            case LIGHT_BRIGHT:
                handle_bright_light_mode();
                break;
            default:
                printf("Unknown light level\n");
                break;
        }
        
        previous_level = current_level;
    }
}

/**
 * Example main function for testing
 */
int main(void) {
    
    while (1) {
        process_light_logic();
        
    }
    
    return 0;
} 
