/*****************************************************************************
* Filename:          C:\Users\superman\Documents\HinckWhite\SpaceInvaderXPS_HW2/drivers/dma_controller_v1_00_a/src/dma_controller.h
* Version:           1.00.a
* Description:       dma_controller Driver Header File
* Date:              Tue Dec 01 13:12:03 2015 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef DMA_CONTROLLER_H
#define DMA_CONTROLLER_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/
#define DMA_CONTROLLER_INTIATE_TRANSACTION 0xCC

// User Logic Slave Space Offsets
#define DMA_CONTROLLER_SLV_REG0_OFFSET 0x00000000
#define DMA_CONTROLLER_SLV_REG1_OFFSET 0x00000004
#define DMA_CONTROLLER_SLV_REG2_OFFSET 0x00000008
#define DMA_CONTROLLER_SLV_REG3_OFFSET 0x0000000C
#define DMA_CONTROLLER_SLV_REG4_OFFSET 0x00000010
#define DMA_CONTROLLER_SLV_REG5_OFFSET 0x00000014
#define DMA_CONTROLLER_SLV_REG6_OFFSET 0x00000018
#define DMA_CONTROLLER_SLV_REG7_OFFSET 0x0000001C

// Write/Read 32 bit value to/from DMA_CONTROLLER user logic slave registers.
#define DMA_CONTROLLER_mWriteSlaveReg0(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (DMA_CONTROLLER_SLV_REG0_OFFSET), (Xuint32)(Value))
#define DMA_CONTROLLER_mWriteSlaveReg1(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (DMA_CONTROLLER_SLV_REG1_OFFSET), (Xuint32)(Value))
#define DMA_CONTROLLER_mWriteSlaveReg2(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (DMA_CONTROLLER_SLV_REG2_OFFSET), (Xuint32)(Value))
#define DMA_CONTROLLER_mWriteSlaveReg3(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (DMA_CONTROLLER_SLV_REG3_OFFSET), (Xuint32)(Value))
#define DMA_CONTROLLER_mWriteSlaveReg4(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (DMA_CONTROLLER_SLV_REG4_OFFSET), (Xuint32)(Value))
#define DMA_CONTROLLER_mWriteSlaveReg5(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (DMA_CONTROLLER_SLV_REG5_OFFSET), (Xuint32)(Value))
#define DMA_CONTROLLER_mWriteSlaveReg6(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (DMA_CONTROLLER_SLV_REG6_OFFSET), (Xuint32)(Value))
#define DMA_CONTROLLER_mWriteSlaveReg7(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (DMA_CONTROLLER_SLV_REG7_OFFSET), (Xuint32)(Value))

#define DMA_CONTROLLER_mReadSlaveReg0(BaseAddress) \
 	Xil_In32((BaseAddress) + (DMA_CONTROLLER_SLV_REG0_OFFSET))
#define DMA_CONTROLLER_mReadSlaveReg1(BaseAddress) \
 	Xil_In32((BaseAddress) + (DMA_CONTROLLER_SLV_REG1_OFFSET))
#define DMA_CONTROLLER_mReadSlaveReg2(BaseAddress) \
 	Xil_In32((BaseAddress) + (DMA_CONTROLLER_SLV_REG2_OFFSET))
#define DMA_CONTROLLER_mReadSlaveReg3(BaseAddress) \
 	Xil_In32((BaseAddress) + (DMA_CONTROLLER_SLV_REG3_OFFSET))
#define DMA_CONTROLLER_mReadSlaveReg4(BaseAddress) \
 	Xil_In32((BaseAddress) + (DMA_CONTROLLER_SLV_REG4_OFFSET))
#define DMA_CONTROLLER_mReadSlaveReg5(BaseAddress) \
 	Xil_In32((BaseAddress) + (DMA_CONTROLLER_SLV_REG5_OFFSET))
#define DMA_CONTROLLER_mReadSlaveReg6(BaseAddress) \
 	Xil_In32((BaseAddress) + (DMA_CONTROLLER_SLV_REG6_OFFSET))
#define DMA_CONTROLLER_mReadSlaveReg7(BaseAddress) \
 	Xil_In32((BaseAddress) + (DMA_CONTROLLER_SLV_REG7_OFFSET))

void DMA_CONTROLLER_InitiateTransfer(Xuint32 BaseAddress, Xuint32 SrcAddress, Xuint32 DstAddress, Xuint32 transferLength);

#endif /** DMA_CONTROLLER_H */
