#ifndef SERIAL_FUNCT
#define SERIAL_FUNCT

#include "esp_system.h"
#include <stdio.h>
#include <string.h>

#define SEPARATOR ' '
#define EOL '\0'

#define DECODE_OK 0
#define DECODE_ERR -1


struct DecodedCommand
{
    /* Struct for saving the incoming command data*/
    char command;
    uint8_t  nParams;
    char * params[4];
};

/**
 * @brief Function for decoding a command from an input string.
 * @param inString pointer to the string
 * @param lenInString uint16_t string length
 * @param command Reference to DecodedCommand struct
 * 
 */
uint8_t decodeCommand(char * inString,uint16_t lenInString, struct DecodedCommand *command);

/**
 * @brief Function for cleaning the memory of the params in the heap.
 * @param command Reference to DecodedCommand struct
 */
uint8_t freeCommand(struct DecodedCommand *command);

#endif