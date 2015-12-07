/*****************************************************************************
* Filename:          C:\Users\superman\Documents\HinckWhite\SpaceInvaderXPS_HW2/drivers/dma_controller_v1_00_a/src/dma_controller.c
* Version:           1.00.a
* Description:       dma_controller Driver Source File
* Date:              Tue Dec 01 13:12:03 2015 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "dma_controller.h"

/************************** Function Definitions ***************************/

void DMA_CONTROLLER_InitiateTransfer(Xuint32 BaseAddress, void* SrcAddress, void* DstAddress, Xuint32 transferLength)
{
	Xil_Out32(BaseAddress + DMA_CONTROLLER_SLV_REG0_OFFSET, (Xuint32)SrcAddress);// SLV0 is src address
	Xil_Out32(BaseAddress + DMA_CONTROLLER_SLV_REG1_OFFSET, (Xuint32)DstAddress);// SLV1 is dst address
	Xil_Out32(BaseAddress + DMA_CONTROLLER_SLV_REG2_OFFSET, transferLength);// SLV2 is transfer length
	Xil_Out32(BaseAddress + DMA_CONTROLLER_SLV_REG3_OFFSET, DMA_CONTROLLER_INTIATE_TRANSACTION);// SLV3 is go
}

