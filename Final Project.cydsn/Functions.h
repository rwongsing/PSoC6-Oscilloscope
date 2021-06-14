/* ========================================
 * Robert Wong-Sing
 * rwongsin@ucsc.edu
 *
 * This file contains all of the function declarations
 * ========================================
*/
#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "stdio.h"

#define XSIZE 320 // Width of LCD screen in pixels
#define YSIZE 240 // Height of LCF screen in pixels
#define XDIVISIONS 10 // Number of x-axis divisions for grid
#define YDIVISIONS 8 // Number of y-axis divisions for grid
#define XMARGIN 5 // Margin around screen on x-axis
#define YMARGIN 4 // Margin around screen on y-axis
#define MAXPOINTS XSIZE-2*XMARGIN // Maximum number of points in wave for plotting
#define BUFFERSIZE 512
    
// Global Variables
int arrPlot[BUFFERSIZE];
int plotPtr;
int arrPlot2[BUFFERSIZE];
int plotPtr2;

uint8_t plotArrayFull;
uint8_t plotArrayFull2;
uint8_t collectData;
uint8_t collectData2;

int xArr[XSIZE];
int yArr[XSIZE];
int xArr2[XSIZE];
int yArr2[XSIZE];

int prevOffset;
int prevOffset2;

int freq1; // Measured in Hz
int freq2; // Measured in Hz
int freqCount1;
int freqCount2;

float Ps;
int xVal;
int ithSample;
int ithSample2;
    
struct scopeParams {
    int on; // 0 is Off, 1 is On 
    int mode; // 0 is Free-running, 1 is Trigger
    int triggerLevel; // measured in milliVolts
    int triggerSlope; // 0 is Negative, 1 is Positive
    int triggerChannel; // 1 is Channel 1, 2 is Channel 2
    int xScale; // Measured in microseconds per division
    int yScale; // Measured in microseconds per division
    
    int potChannel; // Channel being controlled by potentiometer
};
typedef struct scopeParams scopeParams;
    
// Display.c
void drawBackground(int w, int h, int xmargin, int ymargin);
void drawGrid(int w, int h, int xdiv, int ydiv, int xmargin, int ymargin);
void printScaleSettings(int xscale, int yscale, int xposition, int yposition);
void printFrequencies(int freq1, int freq2, int xposition, int yposition);

// uartReadPrint.c
void init_uart_printf(void);
void uart_printf(char *print_string);
void printData(int *arr, int n);
void updateParams(scopeParams *spPtr);

// dataProcess.c
void dataInit();

// plot.c
void stoppedDraw(scopeParams spPtr, int *xArray, int *yArray, int channel);
void drawPlot(scopeParams spPtr, int *xArray, int *yArray, int *plotArray, int channel);
void erasePlot(int *xArr, int *yArr, int yOffset);
void calculateFreqs(scopeParams *spPtr, int *yArray, int channel);

#endif
/* [] END OF FILE */
