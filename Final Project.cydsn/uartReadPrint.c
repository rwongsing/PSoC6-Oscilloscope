/* ========================================
 * Robert Wong-Sing
 * rwongsin@ucsc.edu
 *
 * This file contains all of the UART
 * functions.
 * ========================================
*/

#include "project.h"
#include "Functions.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

char data;
char command[1024];
int rxCount = 0;
bool inData = false;

/* Initializes UART */
void init_uart_printf(void){
 Cy_SCB_UART_Init(UART_printf_HW, &UART_printf_config, &UART_printf_context);
 Cy_SCB_UART_Enable(UART_printf_HW);
}

/* Prints string to terminal */
void uart_printf(char *print_string){
  Cy_SCB_UART_PutString(UART_printf_HW, print_string);
}

/* Prints array to terminal */
void printData(int *arr, int n) {
    char print_string[10];
    uart_printf("\n\rData:\n\r");
    for(int i=0; i < n; i++) {
        sprintf(print_string, "%d ", arr[i]);
        uart_printf(print_string);
    }
}

/* Reads input from terminal through UART */
void readTerminal() {
    uint32_t rxStatus = Cy_SCB_UART_GetRxFifoStatus(UART_printf_HW);
    
    if(rxStatus & CY_SCB_UART_RX_NOT_EMPTY) {
        data = Cy_SCB_UART_Get(UART_printf_HW);
        command[rxCount] = data;
        rxCount++;

        if(data == '\r') {
            UART_printf_HW->INTR_RX_MASK = 0;
            inData = true;
        }
        
        Cy_SCB_UART_ClearRxFifoStatus(UART_printf_HW, CY_SCB_UART_RX_NOT_EMPTY);
    }    
}

/* Parses terminal input and sets values accordingly */
void updateParams(scopeParams *spPtr) {
    char *phrase;
    char *val;
    char response[100];
    int numVal;
    bzero(&response, sizeof(response));
    readTerminal();
    
    if(inData) {
        inData = false;
        strcat(command, "\n");
        uart_printf(command);
        
        phrase = strtok(command, " ");
        
        if(!strncmp(phrase, "start", 5)) {
            uart_printf("Starting TinyScope\n\r");
            spPtr->on = 1;
        } else if(!strncmp(phrase, "stop", 4)) {
            uart_printf("Stopping TinyScope\n\r");
            spPtr->on = 0;          
        } else if(!strncmp(phrase, "set", 3)) {
            phrase = strtok(NULL, " ");
            
            if(phrase == NULL) {
                sprintf(response, "Usage Error: set <mode/trigger_level/trigger_slope/trigger_channel/xscale/yscale> <value>");
            } else {
                val = strtok(NULL, " ");
                if(val == NULL) {
                    sprintf(response, "Usage Error: set <mode/trigger_level/trigger_slope/trigger_channel/xscale/yscale> <value>");         
                } else {
                    if(!strncmp(phrase, "mode", 4)) {
                        if(!strncmp(val, "free", 4)) {
                            sprintf(response, "Mode set to free-running");
                            spPtr->mode = 0;
                        } else if(!strncmp(val, "trigger", 7)) {
                            sprintf(response, "Mode set to trigger");
                            spPtr->mode = 1;
                        } else {
                            sprintf(response, "Usage Error: set mode <free/trigger>");    
                        }
                    } else if(!strncmp(phrase, "trigger_level", 13)) {
                        numVal = strtol(val, (char**)NULL, 10);
                        if(numVal >= 0 && numVal <= 3000) {
                            sprintf(response, "Trigger level set to %d mV", numVal);
                            spPtr->triggerLevel = numVal;
                        } else {
                            sprintf(response, "Usage Error: set trigger_level <0-3000 mV>");    
                        }
                    } else if(!strncmp(phrase, "trigger_slope", 13)) {
                        if(!strncmp(val, "negative", 8)) {
                            sprintf(response, "Trigger slope set to negative");
                            spPtr->triggerSlope = 0;
                        } else if(!strncmp(val, "positive", 8)) {
                            sprintf(response, "Trigger slope set to positive");
                            spPtr->triggerSlope = 1;
                        } else {
                            sprintf(response, "Usage Error: set trigger_slope <negative/positive>");    
                        }
                    } else if(!strncmp(phrase, "trigger_channel", 15)) {
                        if(!strncmp(val, "1", 1)) {
                            sprintf(response, "Trigger channel set to 1");
                            spPtr->triggerChannel = 1;
                        } else if(!strncmp(val, "2", 1)) {
                            sprintf(response, "Trigger channel set to 2");
                            spPtr->triggerChannel = 2;
                        } else {
                            sprintf(response, "Usage Error: set trigger_channel <1/2>");    
                        }    
                    } else if(!strncmp(phrase, "xscale", 6)) {
                        numVal = strtol(val, (char**)NULL, 10);
                        if((numVal >= 500) && (numVal <= 2000) && (numVal % 500 == 0)) {
                            sprintf(response, "Xscale set to %d", numVal);
                            spPtr->xScale = numVal;
                        } else {
                            sprintf(response, "Usage Error: set xscale <500/1000/1500/2000>");    
                        }
                    } else if(!strncmp(phrase, "yscale", 6)) {
                        numVal = strtol(val, (char**)NULL, 10);
                        if((numVal >= 500) && (numVal <= 2000) && (numVal % 500 == 0)) {
                            sprintf(response, "Yscale set to %d", numVal);
                            spPtr->yScale = numVal;
                        } else {
                            sprintf(response, "Usage Error: set yscale <500/1000/1500/2000>");    
                        }
                    } else {
                        sprintf(response, "Usage Error: set <mode/trigger_level/trigger_slope/trigger_channel/xscale/yscale> <value>");    
                    }
                }
            }
        } else {
            sprintf(response, "invalid command");
        }
        strcat(response, "\n\r");
        uart_printf(response);
        
        bzero(&command, sizeof(command));
        bzero(&phrase, sizeof(phrase));
        bzero(&val, sizeof(val));
        bzero(&response, sizeof(response));
        numVal = 0;
        rxCount = 0;
    }
}


/* [] END OF FILE */
