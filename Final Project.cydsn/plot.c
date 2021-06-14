/* ========================================
 * Robert Wong-Sing
 * rwongsin@ucsc.edu
 *
 * This file contains all of the plotting
 * functions.
 * ========================================
*/

#include "project.h"
#include "Functions.h"
#include "GUI.h"

/* Scales the x values */
void scaleValsX(int *xArray) {
    if(Ps > 1) {
        for(int i=0; i < XSIZE; i++) {
            xArray[i] = (int)(Ps*i);
        }
    } else if(Ps == 1) {
        for(int i=0; i < XSIZE; i++) {
            xArray[i] = i;    
        }
    } else {
        for(int i=0; i < XSIZE; i++) {
            xArray[i] = i;    
        }
    }
}

/* Scales the y values */
void scaleValsY(scopeParams spPtr, int *points, int startIndex, int *yArray, int channel) {
    float Py = YSIZE / YDIVISIONS;
    int yArrPtr = 0;
    for(int i=startIndex; i < XSIZE; i++) {
        yArray[yArrPtr] = (Py / (float)spPtr.yScale) * 3300 * ((float)points[i] / 4096);
        yArrPtr++;
    }
    while(yArrPtr != XSIZE) {
        if(channel == 1) {
            collectData = 0;
            if(plotArrayFull) {
                for(int i=0; i < XSIZE; i++) {
                    yArray[yArrPtr] = (Py / (float)spPtr.yScale) * 3300 * ((float)points[i] / 4096);
                    yArrPtr++;
                    if(yArrPtr == XSIZE) {
                        break;
                    }
                }
            }
            collectData = 1;
        } else if(channel == 2) {
            collectData2 = 0;
            if(plotArrayFull2) {
                for(int i=0; i < XSIZE; i++) {
                    yArray[yArrPtr] = (Py / (float)spPtr.yScale) * 3300 * ((float)points[i] / 4096);
                    yArrPtr++;
                    if(yArrPtr == XSIZE) {
                        break;
                    }
                }
            }
            collectData2 = 1;  
        }
    }
    if(channel == 1) {
        collectData = 0;    
    } else if(channel == 2) {
        collectData2 = 0;    
    }
}

/* Returns the index of the starting point based on trigger settings */
int triggerDetect(scopeParams spPtr, int *points) {
    int triggerLevel = ((float)spPtr.triggerLevel / 3300) * 4096;

    for(int i=1; i < BUFFERSIZE; i++) {
        if(points[i-1] > triggerLevel) { // Rising Edge
            if(points[i] <= triggerLevel) {
                if(spPtr.triggerSlope == 1) {
                    return i;
                }
            }
        } else if(points[i-1] < triggerLevel) { // Falling Edge
            if(points[i] >= triggerLevel) {
                if(spPtr.triggerSlope == 0) {
                    return i;
                }
            } 
        }
    }
    
    return -1;
}

/* Returns the offset based on potentiometer */
int getOffset(scopeParams spPtr, int channel) {
    int offset = 0;
    int potOffset = 0;

    potOffset = (int16_t)(SAR->CHAN_RESULT[2]) * ((float)YSIZE / 2048) - (YSIZE / 2);
    
    if(spPtr.potChannel == channel) {
        offset += potOffset;    
    }
    if(channel == 1) {
        offset += 60;
        prevOffset = offset;
    } else if(channel == 2) {
        offset += 130;
        prevOffset2 = offset;
    }

    return offset;
}

/* Draws plots when scope is off. Allows for potentiometer usage */
void stoppedDraw(scopeParams spPtr, int *xArray, int *yArray, int channel) {
    if(channel == 1) {
        erasePlot(xArray, yArray, prevOffset);
    } else if(channel == 2) {
        erasePlot(xArray, yArray, prevOffset2);
    }
    int yOffset = getOffset(spPtr, channel);

    GUI_SetPenSize(2);
    GUI_SetLineStyle(GUI_LS_SOLID);
    if(channel == 1) {
        GUI_SetColor(GUI_RED);
    } else if(channel == 2) {
        GUI_SetColor(GUI_BLACK);
    }
    
    for(int i=0; i < XSIZE-1; i++) {
        GUI_DrawLine(xArray[i], yArray[i]+yOffset, xArray[i+1], yArray[i+1]+yOffset);    
    }
}

/* Draws plot using potentiometer offset and scaled values */
void drawPlot(scopeParams spPtr, int *xArray, int *yArray, int *plotArray, int channel) {
    if(channel == 1) {
        collectData = 0;
        erasePlot(xArray, yArray, prevOffset);

    } else if(channel == 2) {
        collectData2 = 0;
        erasePlot(xArray, yArray, prevOffset2);

    }
    int yOffset = getOffset(spPtr, channel);

    int startIndex = 0;
    if((spPtr.mode == 1) && (spPtr.triggerChannel == channel)) { // Trigger-Level
        startIndex = triggerDetect(spPtr, plotArray);
        
        while(startIndex < 0) {
            if(channel == 1) {
                collectData = 1;    
            } else if(channel == 2) {
                collectData2 = 1;
            }
            if(plotArrayFull && (channel == 1)) {
                collectData = 0;
                startIndex = triggerDetect(spPtr, plotArray);
            } else if(plotArrayFull2 && channel == 2) {
                collectData2 = 0;
                startIndex = triggerDetect(spPtr, plotArray);
            }
        }
    }

    scaleValsX(xArray);
    scaleValsY(spPtr, plotArray, startIndex, yArray, channel);

    GUI_SetPenSize(2);
    GUI_SetLineStyle(GUI_LS_SOLID);
    if(channel == 1) {
        GUI_SetColor(GUI_RED);
    } else if(channel == 2) {
        GUI_SetColor(GUI_BLACK);
    }
    
    for(int i=0; i < XSIZE-1; i++) {
        GUI_DrawLine(xArray[i], yArray[i]+yOffset, xArray[i+1], yArray[i+1]+yOffset);    
    }
    
    calculateFreqs(&spPtr, yArray, channel);
    if(channel == 1) {
        collectData = 1;
    } else if(channel == 2) {
        collectData2 = 1;    
    }
}

/* Erases plot using background color */
void erasePlot(int *xArray, int *yArray, int yOffset) {
    GUI_SetPenSize(2);
    GUI_SetColor(GUI_DARKCYAN);
    GUI_SetLineStyle(GUI_LS_SOLID);
    for(int i=0; i < XSIZE-1; i++) {
        GUI_DrawLine(xArray[i], yArray[i]+yOffset, xArray[i+1], yArray[i+1]+yOffset);    
    }
}

/* Finds transition from positive slope to negative slope */
int findPeak(int *arr, int n, int startIndex) {
    uint8_t prevSlope = 2; // 0 is negative, 1 is positive
    int peak = -1;

    for(int i=startIndex; i < n-1; i++) {
        if((prevSlope == 1) && (arr[i] >= arr[i+1])) {
            peak = i;
            break;
        }
        if(arr[i] < arr[i+1]) {
            prevSlope = 1;
            continue;
        } else {
            prevSlope = 0;
        }
    }

    // check that it's not a false peak
    int j = 1;
    if((peak >= 3) && (peak <= n - 3)) {
        if((arr[peak-1] <= arr[peak]) && (arr[peak-2] <= arr[peak])) {
            if((arr[peak+1] <= arr[peak]) && (arr[peak+2] <= arr[peak])) {
                while(arr[peak] == arr[peak+j]) {
                    j++;
                    peak++;
                }
                return peak;
            }
        }
    }
    
    return -1;
}

/* Calculates frequencies. Sets frequency variable */
void calculateFreqs(scopeParams *spPtr, int *yArray, int channel) {
    float Px = XSIZE / XDIVISIONS;
    int p1, p2;

    p1 = findPeak(yArray, XSIZE, 0);
    if(p1 == -1) {
        return;
    }

    p2 = findPeak(yArray, XSIZE, p1);
    if(p2 == -1) {
        return;    
    }

    if(channel == 1) {
        if((freqCount1 % 5) == 0) {
            freq1 = 1000000 / (((float)spPtr->xScale / Px) * (p2 - p1));
        }
        freqCount1++;
    } else if(channel == 2) {
        if((freqCount2 % 5) == 0) {
            freq2 = 1000000 / (((float)spPtr->xScale / Px) * (p2 - p1));
        }
        freqCount2++;
    }
}

/* [] END OF FILE */
