#include "stepmotor.h"

StepperMotorNema::StepperMotorNema(gpio_num_t p0, gpio_num_t p1, uint32_t delayUs)  {
    pins[0] = p0;
    pins[1] = p1;

    stepDelayUs = delayUs;
    lastTime = 0;
}

void StepperMotorNema::begin() {
    for (int i = 0; i < MOTOR_NPINS; i++) {
        gpio_reset_pin(pins[i]);
        gpio_set_direction(pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(pins[i], 0);
    }

    //set Step to ON
    gpio_set_level(pins[1], 0);

    lastTime = esp_timer_get_time();
}

void StepperMotorNema::update() {
    uint64_t currentTime = esp_timer_get_time();
    
    if (currentTime - lastTime >= stepDelayUs) {

        // Just to send mi Pull/Step a 0 and 1 to advance steps this is bc I conented it to VCC so in rest 1. 
        gpio_set_level(pins[1], 1);
        esp_rom_delay_us(5);
        gpio_set_level(pins[1], 0);
        
        lastTime = currentTime;
    }
}

bool StepperMotorNema::stepsUpdate(){
    uint64_t currentTime = esp_timer_get_time();
    
    if (currentTime - lastTime >= stepDelayUs) {

        // Just to send mi Pull/Step a 0 and 1 to advance steps this is bc I conented it to VCC so in rest 1. 
        gpio_set_level(pins[1], 1);
        esp_rom_delay_us(5);
        gpio_set_level(pins[1], 0);
        
        lastTime = currentTime;
        return true;
    } else {
        return false;
        
    }
        
}

void StepperMotorNema::moveSteps(bool direction, int steps){
    setDirection(direction);
    int count = 0;
    while(count < steps){
        if(stepsUpdate()){
            count++;
        }
    }
}

void StepperMotorNema::setDelay(uint32_t delayUs) {
    stepDelayUs = delayUs;
}

void StepperMotorNema::setDirection(bool direction){
    // Direction of spin determined = [1, 0], where 1 is (clockwise) and 0 is (counterclockwise) 
    gpio_set_level(pins[0],direction ? 1 : 0); 
}