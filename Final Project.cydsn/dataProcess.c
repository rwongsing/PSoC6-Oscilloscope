/* ========================================
 * Robert Wong-Sing
 * rwongsin@ucsc.edu
 *
 * This file contains all of the data processing
 * functions.
 * ========================================
*/

#include "project.h"
#include "Functions.h"
#define ARRSIZE 256 // DMA transfers 256 bytes

int16_t arrA[ARRSIZE];
int16_t arrB[ARRSIZE];
int16_t arrC[ARRSIZE];
int16_t arrD[ARRSIZE];

uint8_t desc = 1;
uint8_t desc2 = 1;

/* DMA ISR fills plot array for Channel 1 */
void dma_ISR() {
    Cy_DMA_Channel_ClearInterrupt(DMA_1_HW, DMA_1_DW_CHANNEL);

    if(collectData) {
        for(int i=0; i < 256; i++) {
            if(plotPtr == BUFFERSIZE) {
                plotArrayFull = 1;
                plotPtr = 0;
                collectData = 0;
                break;
            }
            if(Ps >= 1) {
                if((desc % 2) == 1) {
                    arrPlot[plotPtr] = arrA[i]; 
                } else {
                    arrPlot[plotPtr] = arrB[i];
                }
                plotPtr++;
            } else if(Ps < 1) {
                if(ithSample == 0) {
                    if((desc % 2) == 1) {
                        arrPlot[plotPtr] = arrA[i];
                    } else {
                        arrPlot[plotPtr] = arrB[i];    
                    }
                    ithSample =  xVal;
                    plotPtr++;
                }
                ithSample--;
            }
        }
    }
    desc++;
}

/* DMA ISR fills plot array for Channel 2 */
void dma2_ISR() {
    Cy_DMA_Channel_ClearInterrupt(DMA_2_HW, DMA_2_DW_CHANNEL);

    if(collectData2) {
        for(int i=0; i < 256; i++) {
            if(plotPtr2 == BUFFERSIZE) {
                plotArrayFull2 = 1;
                plotPtr2 = 0;
                collectData2 = 0;
                break;
            }
            if(Ps >= 1) {
                if((desc2 % 2) == 1) {
                    arrPlot2[plotPtr2] = arrC[i]; 
                } else {
                    arrPlot2[plotPtr2] = arrD[i];
                }
                plotPtr2++;
            } else if(Ps < 1) {
                if(ithSample2 == 0) {
                    if((desc2 % 2) == 1) {
                        arrPlot2[plotPtr2] = arrC[i];
                    } else {
                        arrPlot2[plotPtr2] = arrD[i];    
                    }
                    ithSample2 = xVal;
                    plotPtr2++;
                }
                ithSample2--;
            }
        }
    }
    desc2++;
}

/* Initializes ADC, DMA, Channels */
void dataInit() {
    /* Initialize ADC */
    ADC_1_Start();
    ADC_1_StartConvert();
    
    /* Set up DMA (Interrupts, Channel, Descriptors) */
    Cy_SysInt_Init(&dma_int_cfg, dma_ISR);
    NVIC_EnableIRQ(dma_int_cfg.intrSrc);
    Cy_DMA_Channel_SetInterruptMask(DMA_1_HW, DMA_1_DW_CHANNEL, CY_DMA_INTR_MASK);
    
    cy_stc_dma_channel_config_t channelConfig;
    channelConfig.descriptor = &DMA_1_Descriptor_1;
    channelConfig.preemptable = DMA_1_PREEMPTABLE;
    channelConfig.priority = DMA_1_PRIORITY;
    channelConfig.enable = false;
    channelConfig.bufferable = DMA_1_BUFFERABLE;
    
    Cy_DMA_Descriptor_Init(&DMA_1_Descriptor_1, &DMA_1_Descriptor_1_config);
    Cy_DMA_Descriptor_Init(&DMA_1_Descriptor_2, &DMA_1_Descriptor_2_config);
    
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_1_Descriptor_1, (uint16_t *) &(SAR->CHAN_RESULT[0]));
    Cy_DMA_Descriptor_SetDstAddress(&DMA_1_Descriptor_1, arrA);
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_1_Descriptor_2, (uint16_t *) &(SAR->CHAN_RESULT[0]));
    Cy_DMA_Descriptor_SetDstAddress(&DMA_1_Descriptor_2, arrB);
    
    Cy_DMA_Channel_Init(DMA_1_HW, DMA_1_DW_CHANNEL, &channelConfig);
    Cy_DMA_Channel_Enable(DMA_1_HW, DMA_1_DW_CHANNEL);
    Cy_DMA_Enable(DMA_1_HW);
    
    // Set up DMA 2
    Cy_SysInt_Init(&dma_int_2_cfg, dma2_ISR);
    NVIC_EnableIRQ(dma_int_2_cfg.intrSrc);
    Cy_DMA_Channel_SetInterruptMask(DMA_2_HW, DMA_2_DW_CHANNEL, CY_DMA_INTR_MASK);
    
    cy_stc_dma_channel_config_t channelConfig2;
    channelConfig2.descriptor = &DMA_2_Descriptor_1;
    channelConfig2.preemptable = DMA_2_PREEMPTABLE;
    channelConfig2.priority = DMA_2_PRIORITY;
    channelConfig2.enable = false;
    channelConfig2.bufferable = DMA_2_BUFFERABLE;
    
    Cy_DMA_Descriptor_Init(&DMA_2_Descriptor_1, &DMA_2_Descriptor_1_config);
    Cy_DMA_Descriptor_Init(&DMA_2_Descriptor_2, &DMA_2_Descriptor_2_config);
    
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_2_Descriptor_1, (int16_t *) &(SAR->CHAN_RESULT[1]));
    Cy_DMA_Descriptor_SetDstAddress(&DMA_2_Descriptor_1, arrC);
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_2_Descriptor_2, (int16_t *) &(SAR->CHAN_RESULT[1]));
    Cy_DMA_Descriptor_SetDstAddress(&DMA_2_Descriptor_2, arrD);
    
    Cy_DMA_Channel_Init(DMA_2_HW, DMA_2_DW_CHANNEL, &channelConfig2);
    Cy_DMA_Channel_Enable(DMA_2_HW, DMA_2_DW_CHANNEL);
    Cy_DMA_Enable(DMA_2_HW);
}

/* [] END OF FILE */
