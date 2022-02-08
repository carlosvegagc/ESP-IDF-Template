
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"

#include "libraries/LoRa.h"
#include "config.h"
#include "main.h"

#define MODULE_NAME "LORA_MOD"

extern xQueueHandle lora_sender_queue;

void writeMessage( char * message)
{

    ESP_LOGI(MODULE_NAME, "Sending message Lora: %s", message);

	loraBeginPacket(false);

	//lora->setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
	loraWrite( (uint8_t*) message, (size_t) strlen(message) );
	loraEndPacket(false);

}


void lora_sender_task(void *pvParameters)
{

    char * in_message ;

    // Inits the Lora module
    loraInit( PIN_NUM_MOSI, PIN_NUM_MISO, PIN_NUM_CLK, PIN_NUM_CS, RESET_PIN, PIN_NUM_DIO, 10 );

    for (;;) {
        //Waiting for UART event.
        if (xQueueReceive(lora_sender_queue, (void * )&in_message, (portTickType)portMAX_DELAY)) {

            writeMessage(in_message);

        }

        // Task delay
		vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

