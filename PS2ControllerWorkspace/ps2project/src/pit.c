/*****************************************************************************
* Filename:          drivers/pit_v1_00_a/src/pit.c
* Version:           1.00.a
* Description:       pit Driver Source File
* Date:              Tue Nov 03 14:10:03 2015
*****************************************************************************/


/***************************** Include Files *******************************/

#include "pit.h"

/************************** Function Definitions ***************************/

void PIT_startRecurringTimer(Xuint32 BaseAddress, Xuint32 TimerValue)
{
    PIT_mWriteDelayReg(BaseAddress, TimerValue);
    PIT_mWriteControlReg(BaseAddress, PIT_ENABLE_RELOAD | PIT_ENABLE_DECREMENT | PIT_ENABLE_INTERRUPTS);
}

void PIT_startSingleTimer(Xuint32 BaseAddress, Xuint32 TimerValue)
{
    PIT_mWriteDelayReg(BaseAddress, TimerValue);
    PIT_mWriteControlReg(BaseAddress, PIT_ENABLE_DECREMENT | PIT_ENABLE_INTERRUPTS);
}

void PIT_stopTimer(Xuint32 BaseAddress)
{
    PIT_mWriteControlReg(BaseAddress, 0);
}


