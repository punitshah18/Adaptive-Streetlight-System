#define IRSensor1

#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

#include "ece198.h"

int main(void)
{
    HAL_Init();

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A 
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B 
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C 

    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED + LED for IR Sensor 1
    InitializePin(GPIOB, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);
    InitializePin(GPIOA, GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); //LED for IR Sensor 2

    InitializePin(GPIOC, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_NOPULL, 0); 
    InitializePin(GPIOA, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_NOPULL, 0); 

    SerialSetup(9600);

//sensor code
#ifdef IRSensor1

char buff[100];

while(1)
{

    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == 1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, false);
    }
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == 0)
    {
        
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, true);

        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, true);
        uint16_t period = 200, prescale = 16;

        __TIM2_CLK_ENABLE();  
        TIM_HandleTypeDef pwmTimerInstance;  
        InitializePWMTimer(&pwmTimerInstance, TIM2, period, prescale);   
        InitializePWMChannel(&pwmTimerInstance, TIM_CHANNEL_1);          

        InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF1_TIM2); 
        
            // fade the LED in by slowly increasing the duty cycle,
            for (uint32_t i = 10; i != period; ++i)
            {
                //lighting up the LED
                SetPWMDutyCycle(&pwmTimerInstance, TIM_CHANNEL_1, 0.87*i);
                HAL_Delay(8);

                //when it's at the highest level, delay it at the top for 2.5 seconds
                if (i == period)
                {
                    HAL_Delay(2500);
                }
            }

            //start decreasing the brightness from the top to the default, which is 10
            for (uint32_t i = period; i >= 10; --i)
            {
                SetPWMDutyCycle(&pwmTimerInstance, TIM_CHANNEL_1, 1.33*i);
                HAL_Delay(30);
            }
    
    }

}

#endif

return 0;
}

// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}