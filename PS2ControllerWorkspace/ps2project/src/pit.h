/*****************************************************************************
* Filename:          drivers/pit_v1_00_a/src/pit.h
* Version:           1.00.a
* Description:       pit Driver Header File
* Date:              Tue Nov 03 14:10:03 2015
*****************************************************************************/

#ifndef PIT_H
#define PIT_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/
#define PIT_CONTROL_REG_OFFSET 0x00000000
#define PIT_DELAY_REG_OFFSET 0x00000004
#define PIT_ENABLE_DECREMENT 0x1
#define PIT_ENABLE_INTERRUPTS 0x2
#define PIT_ENABLE_RELOAD 0x4

/***************** Macros (Inline Functions) Definitions *******************/
#define PIT_mWriteControlReg(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (PIT_CONTROL_REG_OFFSET), (Xuint32)(Value))
#define PIT_mWriteDelayReg(BaseAddress, Value) \
 	Xil_Out32((BaseAddress) + (PIT_DELAY_REG_OFFSET), (Xuint32)(Value))

#define PIT_mReadControlReg(BaseAddress) \
 	Xil_In32((BaseAddress) + (PIT_CONTROL_REG_OFFSET))
#define PIT_mReadDelayReg(BaseAddress) \
 	Xil_In32((BaseAddress) + (PIT_DELAY_REG_OFFSET))

/************************** Function Prototypes ****************************/

void PIT_startRecurringTimer(Xuint32 BaseAddress, Xuint32 TimerValue);
void PIT_startSingleTimer(Xuint32 BaseAddress, Xuint32 TimerValue);
void PIT_stopTimer(Xuint32 BaseAddress);

#endif /** PIT_H */
