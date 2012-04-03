//
//  ControleMoteur.c
//  PSC
//
//  Created by Quentin Fiard on 27/02/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "ControleMoteur.h"
#include "Capteurs.h"
#include <timers.h>
#include "Utilities.h"
#include <math.h>

#define MOTEUR_SELECT0  LATBbits.LATB4
#define MOTEUR_SELECT1  LATBbits.LATB2
#define MOTEUR_SIGNAL   PORTBbits.RB3

#define MOTEUR_SELECT0_DIRECTION    TRISBbits.TRISB4
#define MOTEUR_SELECT1_DIRECTION    TRISBbits.TRISB2
#define MOTEUR_SIGNAL_DIRECTION     TRISBbits.TRISB3

#define OUTPUT 0

#define TICKS_PER_SECONDS 12000000

#define SIGNAL_TIMER_INTERRUPT_ENABLE INTCONbits.TMR0IE
#define CONTROL_PERIOD_INTERRUPT_ENABLE PIE2bits.TMR3IE

#define WAITOFFSET 20000
//#define WAITOFFSET 5536
//#define WAITOFFSET 10000

//---------------------------------------------------------------------
 // Variables
 //---------------------------------------------------------------------
#pragma romdata eedata_scn=0xf00000

rom float minSignalDurationSaved = 0.0002;
rom float neutralSignalDurationSaved = 0.00108;
rom float maxSignalDurationSaved = 0.00185;

rom float PIDPeriodSaved = -1;

rom PID_Coeffs PID_speed_saved = {-1,-1,-1};
rom PID_Coeffs PID_position_saved = {-1,-1,-1};

rom ChoixAsservissement choixAsservissement_saved = {0,-1,-1};

#pragma idata

static volatile BOOL isControllingMotor[4] = {FALSE,FALSE,FALSE,FALSE};

static float controlForMotor[4] = {0,0,0,0};

static float speed_goal = HUGE_VAL;
static UINT16 position_goal = MAX_SENSOR_ANGLE;

 #pragma idata access accessram

static near volatile BOOL isMotorTimerActive = FALSE;

static near volatile Moteur currentMotor = 0;

static near volatile float PIDPeriod = 0;

static near BOOL firstStart = TRUE;

static near float integralTerm = 0;
static near float lastValue = HUGE_VAL;

#pragma udata

static PID_Coeffs PID_speed;
static PID_Coeffs PID_position;
static ChoixAsservissement choixAsservissement;
static float minSignalDuration;
static float neutralSignalDuration;
static float maxSignalDuration;

#pragma code

static void resetTimer20ms(void)
{
    WriteTimer3(WAITOFFSET);
}

float signalObjectiveForMotor(Moteur moteur)
{
    return controlForMotor[moteur];
}
void setSignalObjectiveForMotor(Moteur moteur, float objective)
{
    BOOL oldIsActive = isMotorTimerActive;

    controlForMotor[moteur] = objective;

    isMotorTimerActive = TRUE;

    isControllingMotor[moteur] = TRUE;

    if(!oldIsActive)
    {
        prepareMotorControl();
        startNextMotorControlSequence();
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

void prepareMotorControl(void)
{
    resetTimer20ms();

    MOTEUR_SELECT0_DIRECTION = OUTPUT;
    MOTEUR_SELECT1_DIRECTION = OUTPUT;
    MOTEUR_SIGNAL_DIRECTION = OUTPUT;

    if(firstStart)
    {
        IPR2bits.TMR3IP = 0;

        firstStart = FALSE;
        eeprom_read_block((UINT8)&maxSignalDurationSaved, &maxSignalDuration, sizeof(float));
        eeprom_read_block((UINT8)&neutralSignalDurationSaved, &neutralSignalDuration, sizeof(float));
        eeprom_read_block((UINT8)&minSignalDurationSaved, &minSignalDuration, sizeof(float));

        eeprom_read_block((UINT8)&PIDPeriodSaved, &PIDPeriod, sizeof(float));

        eeprom_read_block((UINT8)&PID_speed_saved, &PID_speed, sizeof(PID_Coeffs));
        eeprom_read_block((UINT8)&PID_position_saved, &PID_position, sizeof(PID_Coeffs));

        eeprom_read_block((UINT8)&choixAsservissement_saved, &choixAsservissement, sizeof(ChoixAsservissement));
    }
}

static void prepareControlForCurrentMotor(void)
{
    double ratio;
    UINT16 signalDuration;

    if(choixAsservissement.type != ASSERVISSEMENT_SANS && currentMotor==choixAsservissement.motor)
    {
        updateControl();
    }

    openMotor(currentMotor);

    ratio = signalObjectiveForMotor(currentMotor);
    if(ratio>=0)
    {
        signalDuration = TICKS_PER_SECONDS*(ratio*maxSignalDuration + (1-ratio)*neutralSignalDuration);
    }
    else
    {
        signalDuration = TICKS_PER_SECONDS*(-ratio*minSignalDuration + (1+ratio)*neutralSignalDuration);
    }

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
    if(duration<0.005 && duration>0.0001)
    {
        maxSignalDuration = duration;
        eeprom_write_block(&maxSignalDuration, (UINT8)&maxSignalDurationSaved, sizeof(float));
    }
    if(maxSignalDuration<minSignalDuration || neutralSignalDuration>maxSignalDuration || neutralSignalDuration>minSignalDuration)
    {
        Motor motor;
        for(motor=0 ; motor<4 ; motor++)
        {
            isControllingMotor[motor] = FALSE;
        }
    }
}

void setNeutralSignalDuration(float duration)
{
    if(duration<0.005 && duration>0.0001)
    {
        neutralSignalDuration = duration;
        eeprom_write_block(&neutralSignalDuration, (UINT8)&neutralSignalDurationSaved, sizeof(float));
    }
    if(maxSignalDuration<minSignalDuration || neutralSignalDuration>maxSignalDuration || neutralSignalDuration>minSignalDuration)
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
    if(duration<0.005 && duration>0.0001)
    {
        minSignalDuration = duration;
        eeprom_write_block(&minSignalDuration, (UINT8)&minSignalDurationSaved, sizeof(float));
    }
    if(maxSignalDuration<minSignalDuration || neutralSignalDuration>maxSignalDuration || neutralSignalDuration>minSignalDuration)
    {
        Motor motor;
        for(motor=0 ; motor<4 ; motor++)
        {
            isControllingMotor[motor] = FALSE;
        }
    }
}

float readMaxSignalDuration(void)
{
    return maxSignalDuration;
}

float readNeutralSignalDuration(void)
{
    return neutralSignalDuration;
}


float readMinSignalDuration(void)
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

ChoixAsservissement readChoixAsservissement(void)
{
    return choixAsservissement;
}

void setChoixAsservissement(ChoixAsservissement newChoix)
{
    choixAsservissement = newChoix;
    eeprom_write_block(&choixAsservissement, (UINT8)&choixAsservissement_saved, sizeof(ChoixAsservissement));

    integralTerm = 0;
    lastValue = HUGE_VAL;
}

void setPIDPeriod(float period)
{
    if(period<0.02 && period>400e-6)
    {
        PIDPeriod = period;
        eeprom_write_block(&PIDPeriod, (UINT8)&PIDPeriodSaved, sizeof(float));
    }
}

float readPIDPeriod(void)
{
    return PIDPeriod;
}

void updateControl(void)
{
    if(choixAsservissement.type == ASSERVISSEMENT_POSITION && position_goal < MAX_SENSOR_ANGLE)
    {
        float newValue,error,differentialTerm;

        UINT16 currentPosition = getPositionAtSensor(choixAsservissement.sensor);

        newValue = currentPosition;
        newValue /= MAX_SENSOR_ANGLE;

        error = offsetBetweenAngle(position_goal,currentPosition);
        error /= MAX_SENSOR_ANGLE;

        integralTerm += PID_position.gainIntegral * error;
        if(integralTerm > 1)
        {
            integralTerm = 1;
        }
        else if(integralTerm < -1)
        {
            integralTerm = -1;
        }

        if(lastValue != HUGE_VAL)
        {
            differentialTerm = newValue - lastValue;
        }
        else
        {
            differentialTerm = 0;
        }

        lastValue = newValue;

        controlForMotor[choixAsservissement.motor] =    PID_position.gainProportionnel * error
                                                    +   integralTerm
                                                    -   PID_position.gainDifferentiel * differentialTerm;
        if(controlForMotor[choixAsservissement.motor] > 1)
        {
            controlForMotor[choixAsservissement.motor] = 1;
        }
        else if(controlForMotor[choixAsservissement.motor] < -1)
        {
            controlForMotor[choixAsservissement.motor] = -1;
        }
    }
    else if(choixAsservissement.type == ASSERVISSEMENT_VITESSE && position_goal != HUGE_VAL)
    {
        
    }
}

void setPositionObjective(UINT16 objective)
{
    position_goal = objective;

    if(choixAsservissement.type == ASSERVISSEMENT_POSITION)
    {
        BOOL oldIsActive = isMotorTimerActive;

        isMotorTimerActive = TRUE;

        isControllingMotor[choixAsservissement.motor] = TRUE;

        if(!oldIsActive)
        {
            prepareMotorControl();
            startNextMotorControlSequence();
        }
    }
}

void setSpeedObjective(float speed)
{
    speed_goal = speed;

    if(choixAsservissement.type == ASSERVISSEMENT_VITESSE)
    {
        BOOL oldIsActive = isMotorTimerActive;

        isMotorTimerActive = TRUE;

        isControllingMotor[choixAsservissement.motor] = TRUE;

        if(!oldIsActive)
        {
            prepareMotorControl();
            startNextMotorControlSequence();
        }
    }
}