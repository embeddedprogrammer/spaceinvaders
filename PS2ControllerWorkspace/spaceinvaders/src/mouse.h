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
#define MOUSE_LEFT_BUTTON 0x1
#define MOUSE_RIGHT_BUTTON 0x2
#define MOUSE_MIDDLE_BUTTON 0x4
#define MOUSE_ALL_BUTTONS 0x7

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
void mouse_init();
void mouse_resetErrors();
void mouse_enableReporting();
void mouse_reset();
void mouse_stateMachine(unsigned char readVal);
unsigned char mouse_ps2ctrlReadValue();
int mouse_getXMovement();
int mouse_getYMovement();
int mouse_getMouseButtons();

#endif /** PS2CTRL_H */
