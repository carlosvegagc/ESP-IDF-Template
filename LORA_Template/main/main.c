
// Libraries Includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "string.h"

// Project Includes
#include "main.h"
#include "config.h"
#include "OLED.h"
#include "tsk_lora.h"
#include "font.h"

#define APP_NAME "DEMO_APP"

//FREE RTOS DEFINITIONS
xQueueHandle lora_sender_queue = NULL;

int _counter = 0;
uint8_t _color = WHITE;

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
 *  MAIN TASK FUNCTIONS
 ****************************************************************************************
 */

//void app_main()
void main_task( void* param )
{

	//OLED Init
	initOLED( 128, 64, 21, 22, 16 );
	setFont(ArialMT_Plain_16 );
	clear();
	sendData();

	//OLED Send message screeen
	clear();
	drawString(0, 12, "Starting...", _color );
	sendDataBack();

	bool _write = getConfigPin();

	gpio_num_t fp = (gpio_num_t) FLASH_PIN;
	gpio_pad_select_gpio( fp );
	gpio_set_direction( fp , GPIO_MODE_OUTPUT);

	for ( ;; )
	{
		BaseType_t status;
		char message[LEN_MESSAGES_LORA] = "Hello world";

		//Send the lora message
		status = xQueueSend(task1_queue, message ,10/portTICK_PERIOD_MS);

		if(status == pdPASS){
			ESP_LOGI(APP_NAME, "Message send correctly \n");
		}else{
			ESP_LOGI(APP_NAME, "ERROR SENDING MESSAGE \n");
		}

		// Blink the led
		gpio_set_level( fp, 1);
		delay(100);

		gpio_set_level( fp, 0);
		delay(1000);
	

		// if ( !_write && lora.getDataReceived() )
		// {
		// 	for ( int i = 0 ; i < 3 ; i++)
		// 	{
		// 		gpio_set_level( fp, 1);
		// 		delay(50);
		// 		gpio_set_level( fp, 0);
		// 		delay(50);
		// 	}

		// 	char buf[200];
		// 	char msg[100];

		// 	int packetSize = lora.handleDataReceived( msg );
		// 	lora.setDataReceived( false );

		// 	sprintf( buf, "<%10s>\n(%d) RSSI: %d", msg, packetSize, lora.getPacketRssi() );

		// 	_oled->clear();
		// 	_oled->drawString(0, 12, buf, _color );
		// 	_oled->sendDataBack();
		// }

		vTaskDelay(50 / portTICK_PERIOD_MS);

	}

}

/*
 *  MAIN
 ****************************************************************************************
 */

void app_main()
{

	/*** Init the FREERTOS queques ***/
	
	// Create the queque for sending Lora Messages
    lora_sender_queue = xQueueCreate(5, LEN_MESSAGES_LORA);

    if( lora_sender_queue == NULL )
    {
        // There was not enough heap memory space available to create the message buffer. 
        ESP_LOGE(APP_NAME, "Not enough memory to create the lora_sender_queue\n");
    }




	// Had to set the task size to 10k otherwise I would get various instabilities
	// Around 2k or less I would get the stack overflow warning but at 2048 it would
	// just crash in various random ways

	xTaskCreate(main_task, "main_task", 10000, NULL, 1, NULL);

	xTaskCreate(lora_sender_task, "lora_sender_task", 10000, NULL, 1, NULL);
}

