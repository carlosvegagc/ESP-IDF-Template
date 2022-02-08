#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"

#include "string.h"

#include "main.h"
#include "config.h"
#include "task_1.h"

#define MODULE_NAME "LORA_MOD"

extern xQueueHandle task1_queue;

void task_1(void *pvParameters)
{

    char in_message [LEN_MESSAGE];

    for (;;) {
        //Waiting for UART event.
        if (xQueueReceive(task1_queue, (void * )&in_message, (portTickType)portMAX_DELAY)) {

            ESP_LOGI(MODULE_NAME, "Recieved message: %s\n", in_message);
            
        }
    }

    vTaskDelete(NULL);
}