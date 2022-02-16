#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"

#include "main.h"
#include "config.h"
#include "uart_reciever_task.h"
#include "libraries/serial_functions.h"

#define APP_NAME "DEMO_APP"

// FREE RTOS DEFINITIONS
xQueueHandle commands_queue = NULL;
QueueHandle_t uart_in_queue = NULL;

void delay(int msec)
{
	vTaskDelay(msec / portTICK_PERIOD_MS);
}

/*
 *  INIT BOARD FUNCTIONS
 ****************************************************************************************
 */

static void uart_init(void)
{
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_RTS,
		.rx_flow_ctrl_thresh = 122,
		.source_clk = UART_SCLK_APB,
	};

	// Install UART driver, and get the queue.
	uart_driver_install(UART_NUM_0, 4096, 8192, 10, &uart_in_queue, 0);
	// Set UART parameters
	uart_param_config(UART_NUM_0, &uart_config);
	// Set UART pins
	uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

/*
 *  MAIN TASK
 ****************************************************************************************
 * Main task area.
 */

// void app_main()
void main_task(void *param)
{

	for (;;)
	{
		// Structure to save the incoming command data
		struct DecodedCommand command;

		// Wait to recieve a command from the queue
		if (xQueueReceive(commands_queue, &command, 10 / portTICK_PERIOD_MS))
		{

			// Process the command
			switch (command.command)
			{

			case 'A':
			{

				ESP_LOGI(APP_NAME, "Recieved Command A");

				break;
			}

			case 'B':
			{

				break;
			}

			case 'L':
			{	
				// TODO 1: Read the color of the led.
				int R, G, B;

				R = atoi(command.params[1]);

				// TODO 2: Send the color to the led.
				


				break;
			}

			default:
			{
				printf("%c:Unknown\n", command.command);
			}

				// Free the memory used by the command structure
				freeCommand(&command);
			}
			// Task Delay 50 ms
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
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
	commands_queue = xQueueCreate(5, LEN_MESSAGE * sizeof(char));

	if (commands_queue == NULL)
	{
		// There was not enough heap memory space available to create the message buffer.
		ESP_LOGE(APP_NAME, "Not enough memory to create the lora_sender_queue\n");
	}

	/*** Init the uart ***/

	uart_init(); // The uart queue is created here.

	/*** Create the Tasks ***/

	xTaskCreate(main_task, "main_task", 2048, NULL, 1, NULL);

	xTaskCreate(uart_reciever_task, "uart_reciever_task", 2048, NULL, 1, NULL);
}
