
#include "serial_functions.h"

/**
 * @brief Function for decoding a command from an input string.
 * @param inString pointer to the string
 * @param lenInString uint16_t string length
 * @param command Referece to DecodedCommand struct
 * 
 */
uint8_t decodeCommand(char * inString,uint16_t lenInString, struct DecodedCommand *command) {
    //Read the incomming command
    command->command = inString[0];

    //Divide the parameters int separated strings
    uint8_t index = 2;
    uint8_t indLastSep = 2;
    command->nParams = 0;

    do{
        index++;
        if((inString[index]==SEPARATOR)||(inString[index]==EOL)){
            size_t lenParam = index-indLastSep;

            //Assign the memory space for the current param
            command->params[command->nParams] = (char *) malloc(lenParam+1);

            if(command->params[command->nParams] == NULL)
                return DECODE_ERR;
            
            //Copy the chars of the current param.
            memcpy((void *)command->params[command->nParams],(const void *)(inString+(char)indLastSep),lenParam);

            //Assign EOL to the last pos of the string
            command->params[command->nParams][lenParam] = '\0';

            indLastSep = index + 1;
            command->nParams++;

        }
    }while((inString[index] != EOL)&&(index<lenInString));


    return DECODE_OK;
}


/**
 * @brief Function for cleaning the memory of the params in the heap.
 * @param command Referece to DecodedCommand struct
 */
uint8_t freeCommand(struct DecodedCommand *command){
    for(int i = 0;i < command->nParams;i++){
        free(command->params[i]);
    }
    command->nParams = 0;

    return DECODE_OK;
}