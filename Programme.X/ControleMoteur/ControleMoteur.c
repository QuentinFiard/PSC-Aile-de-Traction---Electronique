//
//  ControleMoteur.c
//  PSC
//
//  Created by Quentin Fiard on 27/02/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "ControleMoteur.h"
#include <timers.h>
#include "Utilities.h"

#define MOTEUR_SELECT0  LATBbits.LATB4
#define MOTEUR_SELECT1  LATBbits.LATB2
#define MOTEUR_SIGNAL   PORTBbits.RB3

#define MOTEUR_SELECT0_DIRECTION    TRISBbits.TRISB4
#define MOTEUR_SELECT1_DIRECTION    TRISBbits.TRISB2
#define MOTEUR_SIGNAL_DIRECTION     TRISBbits.TRISB3

#define OUTPUT 0

#define MIN_SIGNAL_DURATION 1.250 //Value in ms
#define MAX_SIGNAL_DURATION 1.850 //Value in ms

#define TICKS_PER_MILLISECONDS 12000

#define SIGNAL_TIMER_INTERRUPT_ENABLE INTCONbits.TMR0IE
#define CONTROL_PERIOD_INTERRUPT_ENABLE PIE2bits.TMR3IE

#define WAITOFFSET 5536
//#define WAITOFFSET 10000

//---------------------------------------------------------------------
 // Variables
 //---------------------------------------------------------------------
#pragma romdata

rom float minSignalDurationSaved = 0;
rom float maxSignalDurationSaved = 0;

rom PID_Coeffs PID_speed_saved;
rom PID_Coeffs PID_position_saved;


 #pragma idata access accessram

static near volatile BOOL isControllingMotor[4] = {FALSE,FALSE,FALSE,FALSE};

static near volatile BOOL isMotorTimerActive = FALSE;

static near volatile double positionForMotor0 = 0;
static near volatile double positionForMotor1 = 0;
static near volatile double positionForMotor2 = 0;
static near volatile double positionForMotor3 = 0;

static near volatile Moteur currentMotor = 0;

static near volatile float minSignalDuration = 0;
static near volatile float maxSignalDuration = 0;

#pragma udata

static volatile PID_Coeffs PID_speed;
static volatile PID_Coeffs PID_position;

#pragma code

static void resetTimer20ms()
{
    WriteTimer3(WAITOFFSET);
}

double positionObjectiveForMotor(Moteur moteur)
{
    if(moteur==0)
    {
        return positionForMotor0;
    }
    if(moteur==1)
    {
        return positionForMotor1;
    }
    if(moteur==2)
    {
        return positionForMotor2;
    }
    if(moteur==3)
    {
        return positionForMotor3;
    }
}
void setPositionObjectiveForMotor(Moteur moteur,double objective)
{
    if(moteur==0)
    {
        positionForMotor0 = objective;
    }
    if(moteur==1)
    {
        positionForMotor1 = objective;
    }
    if(moteur==2)
    {
        positionForMotor2 = objective;
    }
    if(moteur==3)
    {
        positionForMotor3 = objective;
    }
}

void openMotor(Moteur moteur)
{
    if(moteur==0)
    {
        MOTEUR_SELECT0 = 1;
        MOTEUR_SELECT1 = 1;
    }
    else if(moteur==1)
    {
        MOTEUR_SELECT0 = 1;
        MOTEUR_SELECT1 = 0;
    }
    else if(moteur==2)
    {
        MOTEUR_SELECT0 = 0;
        MOTEUR_SELECT1 = 1;
    }
    else if(moteur==3)
    {
        MOTEUR_SELECT0 = 0;
        MOTEUR_SELECT1 = 0;
    }
}

void prepareMotorControl()
{
    resetTimer20ms();

    MOTEUR_SELECT0_DIRECTION = OUTPUT;
    MOTEUR_SELECT1_DIRECTION = OUTPUT;
    MOTEUR_SIGNAL_DIRECTION = OUTPUT;

    eeprom_read_block((UINT8)&maxSignalDurationSaved, &maxSignalDuration, sizeof(float));
    eeprom_read_block((UINT8)&minSignalDurationSaved, &minSignalDuration, sizeof(float));

    eeprom_read_block((UINT8)&PID_speed_saved, &PID_speed, sizeof(PID_Coeffs));
    eeprom_read_block((UINT8)&PID_position_saved, &PID_position, sizeof(PID_Coeffs));
}

static void prepareControlForCurrentMotor()
{
    double ratio;
    UINT16 signalDuration;

    openMotor(currentMotor);

    ratio = positionObjectiveForMotor(currentMotor);
    signalDuration = (TICKS_PER_MILLISECONDS*((ratio+1)*MAX_SIGNAL_DURATION + (1-ratio)*MIN_SIGNAL_DURATION) + 1)/2;
    WriteTimer0(~signalDuration);
    MOTEUR_SIGNAL = 1;
}

void stopMotorSignal()
{
    MOTEUR_SIGNAL = 0;
    MOTEUR_SELECT1 = 0;

    currentMotor++;

    while(currentMotor<4 && !isControllingMotor[currentMotor])
    {
        currentMotor++;
    }

    if(currentMotor<4)
    {
        prepareControlForCurrentMotor();
    }
    else
    {
        CloseTimer0();
    }
}

void setRelativePositionObjectiveForMotor(double ratio, Moteur moteur)
{
    BOOL oldIsActive = isMotorTimerActive;
    double check;

    isMotorTimerActive = TRUE;

    isControllingMotor[moteur] = TRUE;
    setPositionObjectiveForMotor(moteur,ratio);
    currentMotor = moteur;
    check = positionObjectiveForMotor(moteur);

    if(!oldIsActive)
    {
        prepareMotorControl();
        startNextMotorControlSequence();
    }
}

void startNextMotorControlSequence()
{
    if(maxSignalDuration!=0 && minSignalDuration != 0 && minSignalDuration < maxSignalDuration)
    {
        // Timer0 is used to send the impulsions to control the motor
        OpenTimer0(     TIMER_INT_ON
                    &   T0_16BIT
                    &   T0_SOURCE_INT
                    &   T0_PS_1_1); // No prescaler allows for a 5ms signal duration, well enough.

        OpenTimer3(     TIMER_INT_ON
                    &   T3_16BIT_RW
                    &   T3_SOURCE_INT
                    &   T3_PS_1_4   // Prescaler of 4 allows for a 20ms period to be measured.
                    &   T3_SYNC_EXT_OFF
                    );

        resetTimer20ms();

        currentMotor = 0;

        while(currentMotor<4 && !isControllingMotor[currentMotor])
        {
            currentMotor++;
        }

        if(currentMotor<4)
        {
            prepareControlForCurrentMotor();
        }
        else
        {
            CloseTimer0();
            CloseTimer3();
        }
    }
}

void stopControllingMotor(Moteur moteur)
{
    BOOL test;
    Moteur tmp;
    
    isControllingMotor[moteur] = FALSE;

    test = TRUE;

    tmp = 0;

    while(tmp<4 && !isControllingMotor[tmp])
    {
        tmp++;
    }

    if(tmp==4)
    {
        isMotorTimerActive = FALSE;

        CloseTimer0();
        CloseTimer3();
    }
}

void setMaxSignalDuration(float duration)
{
    if(duration<5000 && duration>100)
    {
        maxSignalDuration = duration;
        eeprom_write_block(&maxSignalDuration, (UINT8)&maxSignalDurationSaved, sizeof(float));
    }
    if(maxSignalDuration<minSignalDuration)
    {
        Motor motor;
        for(motor=0 ; motor<4 ; motor++)
        {
            isControllingMotor[motor] = FALSE;
        }
    }
}

void setMinSignalDuration(float duration)
{
    if(duration<5000 && duration>100)
    {
        minSignalDuration = duration;
        eeprom_write_block(&minSignalDuration, (UINT8)&minSignalDurationSaved, sizeof(float));
    }
    if(maxSignalDuration<minSignalDuration)
    {
        Motor motor;
        for(motor=0 ; motor<4 ; motor++)
        {
            isControllingMotor[motor] = FALSE;
        }
    }
}

float readMaxSignalDuration()
{
    return maxSignalDuration;
}

float readMinSignalDuration()
{
    return minSignalDuration;
}

PID_Coeffs* readSpeedPIDCoeffs(void)
{
    return &PID_speed;
}

PID_Coeffs* readPositionPIDCoeffs(void)
{
    return &PID_position;
}

void setSpeedPIDCoeffs(PID_Coeffs* coeffs)
{
    PID_speed = *coeffs;
    eeprom_write_block(&PID_speed, (UINT8)&PID_speed_saved, sizeof(PID_Coeffs));
}

void setPositionPIDCoeffs(PID_Coeffs* coeffs)
{
    PID_position = *coeffs;
    eeprom_write_block(&PID_position, (UINT8)&PID_position_saved, sizeof(PID_Coeffs));
}
