#pragma once

#include <stdint.h>
#include <driver/gpio.h>
#include <esp_timer.h>
#include <stdbool.h>
#include <esp_rom_sys.h>

#define MOTOR_NPINS  2

class StepperMotorNema {
public:
    // Constructor
    StepperMotorNema(gpio_num_t p0, gpio_num_t p1, uint32_t delayUs);

    void begin(); // Configures the GPIO pins as outputs
    void update(); // Non-blocking update method. Call this inside your main loop.
    bool stepsUpdate();  //Function to make it move to the beginning.
    void moveSteps(bool direction, int steps); 
    void setDelay(uint32_t delayUs); // Allows changing the speed on the fly
    void setDirection(bool direction);

private:
    gpio_num_t pins[MOTOR_NPINS];
    uint32_t stepDelayUs;
    uint64_t lastTime;
};