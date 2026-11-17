
#include "definitions.h"

extern "C" void app_main()
{
    StepperMotorNema stepper(DIR_1, PUL_1, STEP_DELAY);
    stepper.begin();

    while (1)
    {
        // RECEPCION UART
        if (uart.available() > 0)
        {
            int len = uart.read(temporal, sizeof(temporal));

            for (int i = 0; i < len; i++)
            {
                if (temporal[i] == '\n')
                {
                    linea[posicion] = '\0';

                    if (current_state == EstadoStepper::ESPERA)
                    {
                        if (strcmp(linea, "STEP1") == 0)
                            current_state = EstadoStepper::STEP1;

                        else if (strcmp(linea, "STEP2") == 0)
                            current_state = EstadoStepper::STEP2;
                    }

                    posicion = 0;
                }
                else if (posicion < (int)sizeof(linea) - 1)
                {
                    linea[posicion++] = temporal[i];
                }
                else
                {
                    posicion = 0;
                }
            }
        }

        // DETECCION DE ESTADOS
        if (current_state != last_state)
        {
            last_state = current_state;
            is_new_state = true;
        }
        else
        {
            is_new_state = false;
        }

        // STATE MACHINE
        switch (current_state)
        {
        case EstadoStepper::ESPERA:
            if (is_new_state)
                ESP_LOGI("ESP_B", "Esperando comando");
            break;

        case EstadoStepper::STEP1:
            if (is_new_state)
            {
                ESP_LOGI("ESP_B", "Iniciando STEP1");
                stepper.setDirection(true);
                esp_rom_delay_us(20);
                pasos = 0;
            }

            if (pasos < NUM_STEPS && stepper.stepsUpdate())
                pasos++;

            if (pasos >= NUM_STEPS)
            {
                uart.write("DONE1\n", 6);
                ESP_LOGI("ESP_B", "STEP1 terminado");
                current_state = EstadoStepper::ESPERA;
            }
            break;

        case EstadoStepper::STEP2:
            if (is_new_state)
            {
                ESP_LOGI("ESP_B", "Iniciando STEP2");
                stepper.setDirection(false);
                esp_rom_delay_us(20);
                pasos = 0;
            }

            if (pasos < NUM_STEPS && stepper.stepsUpdate())
                pasos++;

            if (pasos >= NUM_STEPS)
            {
                uart.write("DONE2\n", 6);
                ESP_LOGI("ESP_B", "STEP2 terminado");
                current_state = EstadoStepper::ESPERA;
            }
            break;

        default:
            break;
        }
    }
}
