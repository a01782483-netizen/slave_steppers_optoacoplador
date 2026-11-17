#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <esp_timer.h>
#include <esp_log.h>
#include <string.h>

#include "SimpleUART.h"
#include "stepmotor.h"

#define UART_TX GPIO_NUM_17
#define UART_RX GPIO_NUM_16
#define BAUD_RATE 9600

#define DIR_1 GPIO_NUM_32
#define PUL_1 GPIO_NUM_33


#define NUM_STEPS 100
#define STEP_DELAY 3000

enum class EstadoStepper
{
    ESPERA,
    STEP1,
    STEP2,
    NONE
};

SimpleUART uart(BAUD_RATE, UART_NUM_2, UART_TX, UART_RX);

EstadoStepper current_state = EstadoStepper::ESPERA;
EstadoStepper last_state = EstadoStepper::NONE;
bool is_new_state = false;

char temporal[64];
char linea[64];
int posicion = 0;

int pasos = 0;

#endif // __DEFINITIONS_H__