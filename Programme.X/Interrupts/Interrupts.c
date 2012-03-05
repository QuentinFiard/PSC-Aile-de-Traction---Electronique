//
//  Interrupts.c
//  PSC
//
//  Created by Quentin Fiard on 27/02/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Interrupts.h"

#include "ControleMoteur.h"

#define MOTOR_SIGNAL_TIMER_FLAG INTCONbits.TMR0IF
#define MOTOR_CONTROL_PERIOD_TIMER_FLAG PIR2bits.TMR3IF

#pragma code

//These are your actual interrupt handling routines.
#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode()
{
    if(MOTOR_SIGNAL_TIMER_FLAG)
    {
        stopMotorSignal();
        MOTOR_SIGNAL_TIMER_FLAG = 0;
    }
    else if(MOTOR_CONTROL_PERIOD_TIMER_FLAG)
    {
        startNextMotorControlSequence();

        MOTOR_CONTROL_PERIOD_TIMER_FLAG = 0;
    }
    else
    {
        USBDeviceTasks();
    }
}	//This return will be a "retfie fast", since this is in a #pragma interrupt section
#pragma interruptlow YourLowPriorityISRCode
void YourLowPriorityISRCode()
{
    

}	//This return will be a "retfie", since this is in a #pragma interruptlow section
