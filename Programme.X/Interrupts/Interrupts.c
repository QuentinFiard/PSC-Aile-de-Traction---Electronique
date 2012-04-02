//
//  Interrupts.c
//  PSC
//
//  Created by Quentin Fiard on 27/02/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Interrupts.h"

#include "ControleMoteur.h"

#include "CommunicationUSB.h"

#define MOTOR_SIGNAL_TIMER_FLAG INTCONbits.TMR0IF
#define MOTOR_CONTROL_PERIOD_TIMER_FLAG PIR2bits.TMR3IF
#define SENSOR_UPDATE_TIMER_FLAG PIR1bits.TMR2IF

#define MOTOR_SIGNAL_TIMER_ENABLE INTCONbits.TMR0IE
#define MOTOR_CONTROL_PERIOD_TIMER_ENABLE PIE2bits.TMR3IE
#define SENSOR_UPDATE_TIMER_ENABLE PIE1bits.TMR2IE

extern BOOL shouldUpdateSensors;

#pragma code

//These are your actual interrupt handling routines.
#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode()
{
    if(MOTOR_SIGNAL_TIMER_ENABLE && MOTOR_SIGNAL_TIMER_FLAG)
    {
        stopMotorSignal();
        MOTOR_SIGNAL_TIMER_FLAG = 0;
    }
}	//This return will be a "retfie fast", since this is in a #pragma interrupt section

#pragma interruptlow YourLowPriorityISRCode
void YourLowPriorityISRCode()
{
    if(MOTOR_CONTROL_PERIOD_TIMER_ENABLE && MOTOR_CONTROL_PERIOD_TIMER_FLAG)
    {
        startNextMotorControlSequence();

        MOTOR_CONTROL_PERIOD_TIMER_FLAG = 0;
    }
    else if(SENSOR_UPDATE_TIMER_ENABLE && SENSOR_UPDATE_TIMER_FLAG)
    {
        shouldUpdateSensors = TRUE;

        SENSOR_UPDATE_TIMER_FLAG = 0;
    }
    else
    {
        USBDeviceTasks();

        ProcessUSBData();
    }
}	//This return will be a "retfie", since this is in a #pragma interruptlow section
