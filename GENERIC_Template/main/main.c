#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "string.h"

#include "main.h"
#include "config.h"
#include "task_1.h"

#define APP_NAME "DEMO_APP"


//FREE RTOS DEFINITIONS
xQueueHandle task1_queue = NULL;

void delay( int msec )
{
    vTaskDelay( msec / portTICK_PERIOD_MS);
}

/*
 *  INIT BOARD FUNCTIONS
 ****************************************************************************************
 */

bool getConfigPin()
{
    gpio_config_t io_conf;
    gpio_num_t pin = (gpio_num_t) SENDER_RECEIVER_PIN;

    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pin_bit_mask = (1ULL << pin );
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;

    gpio_config(&io_conf);

    return gpio_get_level( pin ) == 0;
}



/*
 *  MAIN TASK 
 ****************************************************************************************
 * Main task area. 
 */

//void app_main()
void main_task( void* param )
{

	for ( ;; )
	{
		BaseType_t status;
		char message[LEN_MESSAGE] = "Hello world";

		//Send the lora message
		status = xQueueSend(task1_queue, message ,10/portTICK_PERIOD_MS);

		if(status == pdPASS){
			ESP_LOGI(APP_NAME, "Message send correctly \n");
		}else{
			ESP_LOGI(APP_NAME, "ERROR SENDING MESSAGE \n");
		}

		//Task Delay 50 ms
		vTaskDelay(100 / portTICK_PERIOD_MS);

	}

}

/*
 *  MAIN
 ****************************************************************************************
 * Use to initial configuration of the system and starting the tasks.
 */

void app_main()
{

	/*** Init the FREERTOS queques ***/
	
	// Create the queque for sending Lora Messages
    task1_queue = xQueueCreate(5, LEN_MESSAGE * sizeof(char));

    if( task1_queue == NULL )
    {
        // There was not enough heap memory space available to create the message buffer. 
        ESP_LOGE(APP_NAME, "Not enough memory to create the lora_sender_queue\n");
    }




	// Had to set the task size to 10k otherwise I would get various instabilities
	// Around 2k or less I would get the stack overflow warning but at 2048 it would
	// just crash in various random ways

	xTaskCreate(main_task, "main_task", 10000, NULL, 1, NULL);

	xTaskCreate(task_1, "task_1", 10000, NULL, 2, NULL);
}

