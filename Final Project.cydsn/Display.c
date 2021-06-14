/* ========================================
 * Robert Wong-Sing
 * rwongsin@ucsc.edu
 *
 * This file contains all of the Display functions
 * ========================================
*/

#include "GUI.h"
#include "Functions.h"
#include "stdio.h"

/*******************************************************************************
// Draw the background for the oscilloscope screen
*******************************************************************************/
void drawBackground(int w, int h, // width and height of screen
		    int xmargin, int ymargin){ // Margin around the image

   GUI_SetBkColor(GUI_BLACK);
   GUI_SetColor(GUI_DARKCYAN);
   GUI_FillRect(0, 0, w, h);
   GUI_SetPenSize(1);
   GUI_SetColor(GUI_LIGHTGRAY);
   GUI_DrawRect(xmargin, ymargin, w-xmargin, h-ymargin);
   GUI_SetLineStyle(GUI_LS_DOT);
}

/*******************************************************************************
// Draw the X/Y grid
*******************************************************************************/
void drawGrid(int w, int h, // width and height of screen
		    int xdiv, int ydiv,// Number of x and y divisions
		    int xmargin, int ymargin){ // Margin around the image

    int xstep = (w-xmargin*2)/xdiv;
    int ystep = (h-ymargin*2)/ydiv;
 
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_SetLineStyle(GUI_LS_DOT);
    for (int i=1; i<ydiv; i++){
        GUI_DrawLine(xmargin, ymargin+i*ystep, w-xmargin, ymargin+i*ystep);
    }
    for (int i=1; i<xdiv; i++){
      GUI_DrawLine(xmargin+i*xstep, ymargin, xmargin+i*xstep, h-ymargin);
    }
}
           
/*******************************************************************************
// Display x and y scale settings
*******************************************************************************/

void printScaleSettings(int xscale, int yscale, int xposition, int yposition) { 
    char str[20];
    
    GUI_SetBkColor(GUI_DARKCYAN); // Set background color
    GUI_SetFont(GUI_FONT_16B_1); // Set font
    GUI_SetColor(GUI_LIGHTGRAY); // Set foreground color
    if (xscale >= 1000)
       sprintf(str, "Xscale: %0d ms", xscale/1000);
    else
       sprintf(str, "Xscale: %0d us", xscale);
    GUI_DispStringAt(str, xposition, yposition);
    int yscaleVolts = yscale/1000;
    int yscalemVolts = yscale % 1000;
    if (yscale >= 1000)
      sprintf(str, "Yscale: %0d V", yscaleVolts);
    else
      sprintf(str, "Yscale: %0d.%0d V", yscaleVolts, yscalemVolts/100);
    GUI_DispStringAt(str, xposition, yposition + 20);
  
}

/*******************************************************************************
// Displays frequencies
*******************************************************************************/
void printFrequencies(int freq1, int freq2, int xposition, int yposition) {
    char str[20];
    
    GUI_SetBkColor(GUI_DARKCYAN);
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_SetColor(GUI_LIGHTGRAY);
    if(freq1 >= 1000) {
        sprintf(str, "Ch 1 Freq: %0d kHz", freq1/1000);    
    } else {
        sprintf(str, "Ch 1 Freq: %0d Hz", freq1);    
    }
    GUI_DispStringAt(str, xposition, yposition);
    
    if(freq2 >= 1000) {
        sprintf(str, "Ch 2 Freq: %0d kHz", freq2/1000);    
    } else {
        sprintf(str, "Ch 2 Freq: %0d Hz", freq2);
    }
    GUI_DispStringAt(str, xposition, yposition + 20);
}

/* [] END OF FILE */
