#ifndef CONFIG

#define CONFIG
#include "freertos/FreeRTOS.h"

/*** Board Configuration ***/


#define PIN_NUM_MISO 	19
#define PIN_NUM_MOSI 	27
#define PIN_NUM_CLK  	5
#define PIN_NUM_CS   	18
#define PIN_NUM_DIO		26
#define RESET_PIN  		23

#define SENDER_RECEIVER_PIN	12
#define	FLASH_PIN			25

/*** System Config ***/
#define LEN_MESSAGES_LORA 20 

#endif