/*****************************************************************************
* Filename:          C:\Users\superman\Documents\HinckWhite\SpaceInvaderXPS_HW2/drivers/ps2ctrl_v1_00_a/src/ps2ctrl.h
* Version:           1.00.a
* Description:       ps2ctrl Driver Header File
* Date:              Thu Nov 19 08:30:54 2015 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef PS2CTRL_H
#define PS2CTRL_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/
#define PS2CTRL_SLV_REG0_OFFSET 0x00000000
#define PS2CTRL_SLV_REG1_OFFSET 0x00000004
#define PS2CTRL_SLV_REG2_OFFSET 0x00000008
#define PS2CTRL_SLV_REG3_OFFSET 0x0000000C
#define PS2CTRL_SLV_REG4_OFFSET 0x00000010

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/
#define PS2CTRL_mWriteSlaveReg0(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (PS2CTRL_SLV_REG0_OFFSET), (Xuint32)(Value))
#define PS2CTRL_mWriteSlaveReg2(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (PS2CTRL_SLV_REG2_OFFSET), (Xuint32)(Value))

#define PS2CTRL_mReadSlaveReg0(BaseAddress) \
 	Xil_In32((BaseAddress) + (PS2CTRL_SLV_REG0_OFFSET))
#define PS2CTRL_mReadSlaveReg1(BaseAddress) \
 	Xil_In32((BaseAddress) + (PS2CTRL_SLV_REG1_OFFSET))
#define PS2CTRL_mReadSlaveReg2(BaseAddress) \
 	Xil_In32((BaseAddress) + (PS2CTRL_SLV_REG2_OFFSET))
#define PS2CTRL_mReadSlaveReg3(BaseAddress) \
 	Xil_In32((BaseAddress) + (PS2CTRL_SLV_REG3_OFFSET))
#define PS2CTRL_mReadSlaveReg4(BaseAddress) \
 	Xil_In32((BaseAddress) + (PS2CTRL_SLV_REG4_OFFSET))


/************************** Function Prototypes ****************************/
void init();
void resetErrors();
void enableReporting();
void reset();
void mouseStateMachine(unsigned char readVal);
int getXMovement();
int getYMovement();
int getMouseButtons();

#endif /** PS2CTRL_H */
