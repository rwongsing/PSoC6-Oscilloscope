/* ========================================
 * Robert Wong-Sing
 * rwongsin@ucsc.edu
 * Final Project
 * 
 * Note: Some code is borrowed from Professor
 *       Varma's Final Project Demo
 * ========================================
*/
#include "project.h"
#include "GUI.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "Functions.h"

/* Default: Off, Free-running, 1V trigger, Positive trigger, 1000 xScale, 1000 yScale, Channel 1 */
scopeParams sp = {0, 0, 1000, 1, 1, 1000, 1000, 1};
scopeParams *spPtr = &sp;

void initGlobals() {
    plotArrayFull = 0;
    plotArrayFull2 = 0;
    plotPtr = 0;
    plotPtr2 = 0;
    collectData = 0;
    collectData2 = 0;
    prevOffset = 0;
    prevOffset2 = 0;
    freq1 = 0;
    freq2 = 0;
    freqCount1 = 0;
    freqCount2 = 0;
    
    Ps = 1;
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Initialize EmWin Graphics */
    GUI_Init();
    
    /* Display Background and Grid */
    drawBackground(XSIZE, YSIZE, XMARGIN, YMARGIN);
    drawGrid(XSIZE, YSIZE, XDIVISIONS, YDIVISIONS, XMARGIN, YMARGIN);
    
    /* Start ADC and DMA */
    dataInit();
    
    init_uart_printf();
    
    initGlobals();

    for(;;)
    {
        if(!Cy_GPIO_Read(Pot_Switch_0_PORT, Pot_Switch_0_NUM)) {
            if(sp.potChannel == 1) {
                sp.potChannel = 2;    
            } else if(sp.potChannel == 2) {
                sp.potChannel = 1;    
            }
        }
        updateParams(spPtr);
        if(sp.on) {
            Ps = 4 * ((XSIZE/XDIVISIONS) / (float)sp.xScale);
            xVal = 1 / Ps;
            ithSample = xVal;
            ithSample2 = xVal;
            
            collectData = 1;
            collectData2 = 1;
            if(plotArrayFull) {
                //printData(arrPlot, BUFFERSIZE);
                drawPlot(sp, xArr, yArr, arrPlot, 1);
                
                plotArrayFull = 0;
            }
            if(plotArrayFull2) {
                //printData(arrPlot2, BUFFERSIZE);
                drawPlot(sp, xArr2, yArr2, arrPlot2, 2);

                plotArrayFull2 = 0;
            }
            
        } else {
            collectData = 0;
            collectData2 = 0;
            stoppedDraw(sp, xArr, yArr, 1);
            stoppedDraw(sp, xArr2, yArr2, 2);
        }
        drawGrid(XSIZE, YSIZE, XDIVISIONS, YDIVISIONS, XMARGIN, YMARGIN);
        /* Display scale settings */
        printScaleSettings(sp.xScale, sp.yScale, 200, 15);
        printFrequencies(freq1, freq2, 10, 15);
    }
}

/* [] END OF FILE */
