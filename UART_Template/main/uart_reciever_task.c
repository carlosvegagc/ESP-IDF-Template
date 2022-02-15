#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "string.h"

#include "main.h"
#include "config.h"
#include "uart_reciever_task.h"
#include "libraries/serial_functions.h"

#define MOD_NAME "UART_RECIEVER"

extern xQueueHandle commands_queue;
extern QueueHandle_t uart_in_queue;

void uart_reciever_task(void *pvParameters)
{
    uart_event_t event;

    for (;;)
    {
        // Waiting for UART event.
        if (xQueueReceive(uart_in_queue, (void *)&event, (portTickType)portMAX_DELAY))
        {
            switch (event.type)
            {
            // Event of UART receving data
            case UART_DATA:
            {

                BaseType_t status;
                uint8_t *temp = NULL;

                // Asign the memory space for the data in the heap
                temp = (uint8_t *)malloc(sizeof(uint8_t) * event.size);
                if (temp == NULL)
                {
                    ESP_LOGE(MOD_NAME, "%s malloc.1 failed\n", __func__);
                    break;
                }

                // Sets the memory space to 0x00
                memset(temp, 0x0, event.size);

                // Reads the uart and saves the values in the heap.
                uart_read_bytes(UART_NUM_0, temp, event.size, portMAX_DELAY);

                // Structure to save the incoming command data
                struct DecodedCommand command;

                // Decode the string into the command structure.
                decodeCommand((char *)temp, LEN_MESSAGE, &command);

                // Add the command structure to the queue.
                status = xQueueSend(commands_queue, &command, 10 / portTICK_PERIOD_MS);

                // Check if it has been added correctly
                if (status != pdPASS)
                {
                    ESP_LOGI(MOD_NAME, "ERROR SENDING MESSAGE \n");
                }

                // Free on memory the received string.
                free(temp);

                break;
            }
            default:
                break;
            }
        }
    }
    vTaskDelete(NULL);
}
