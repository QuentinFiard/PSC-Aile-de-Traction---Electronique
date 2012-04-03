//
//  Capteurs.c
//  PSC
//
//  Created by Quentin Fiard on 26/02/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include <spi.h>
#include "./Capteurs.h"
#include <delays.h>
#include <math.h>
#include <timers.h>
#include "CommunicationProtocol.h"
#include "CommunicationUSB.h"

/*
#define OCF ((byte2 & 0x08) == 0)
#define COF ((byte2 & 0x04) != 0)
#define LIN ((byte2 & 0x02) != 0)
#define MAGINC ((byte2 & 0x01) != 0)
#define MAGDEC ((byte3 & 0x80) != 0)
#define PARITY ((byte3 & 0x40) != 0)
*/

#define OCF ((byte3 & 0x20) == 0)
#define COF ((byte3 & 0x10) != 0)
#define LIN ((byte3 & 0x08) != 0)
#define MAGINC ((byte3 & 0x04) != 0)
#define MAGDEC ((byte3 & 0x02) != 0)
#define PARITY ((byte3 & 0x01) != 0)

#define SENSOR_PORT0 LATAbits.LATA0
#define SENSOR_PORT1 LATAbits.LATA1
#define SENSOR_PORT2 LATAbits.LATA2
#define SENSOR_PORT3 LATAbits.LATA3
#define SENSOR_PORT4 LATAbits.LATA4
#define SENSOR_PORT5 LATAbits.LATA5

#define CLOCK_DIRECTION TRISBbits.RB1
#define CLOCK_PORT LATBbits.LATB1
#define DATA_DIRECTION TRISBbits.RB0
#define DATA_IN PORTBbits.RB0;

#define SPI_TIME_OFFSET Delay10TCYx(2);
#define SPI_TIME_OFFSET_UP Delay10TCYx(7);

extern BOOL shouldUpdateSensors;

#pragma idata

static BOOL isSensorReadReady = FALSE;

static INT8 nextStatusIndex[6] = {-1,-1,-1,-1,-1,-1};
static INT8 nextTimeIndex[6] = {-1,-1,-1,-1,-1,-1};

#pragma udata

static SensorStatus lastStatus[6][NUM_READ];
static UINT16 timeOffset[6][NUM_TIME];
static UINT16 lastDate[6];

static volatile UINT16 readTime;

void prepareForSensorRead(void)
{
    //OpenSPI(SPI_FOSC_64, MODE_00, SMPEND);

    //Initialisation des ports de sélection des capteurs
    LATA |= 0x3F; // Waiting state is high
    ADCON1 |= 0x0F;
    TRISA &= 0xC0; // Ports en sortie

    DATA_DIRECTION = 1; // Data in
    CLOCK_PORT = 1;
    CLOCK_DIRECTION = 0; // RB1 en sortie

    OpenTimer1(     TIMER_INT_OFF
                &   T1_16BIT_RW
                &   T1_SOURCE_INT
                &   T1_PS_1_2
                &   T1_OSC1EN_OFF
                &   T1_SYNC_EXT_OFF); // Used to measure time between measurements

    IPR1bits.TMR2IP = 0;

    OpenTimer2(     TIMER_INT_ON
                &   T2_PS_1_16
                &   T2_POST_1_16);   // Used to specify time between measurements

    isSensorReadReady = TRUE;

    //shouldUpdateSensors = TRUE;
}

static void selectSensor(Sensor sensor)
{
    LATA |= 0x3F; // Waiting state is high

    LATA -= (0x01 << sensor);
}

UINT16 timeOffsetBetweenDate(UINT16 date1, UINT16 date2)
{
    if(date2>= date1)
    {
        return date2-date1;
    }
    else
    {
        return 0x10000 - (date1-date2);
    }
}

void updateSensorStatus(SensorStatus status, Sensor sensor)
{
    if(nextStatusIndex[sensor]==-1)
    {
        lastStatus[sensor][0] = status;
        nextStatusIndex[sensor] = 1;

        nextTimeIndex[sensor] = 0;
        lastDate[sensor] = readTime;
    }
    else
    {
        lastStatus[sensor][nextStatusIndex[sensor]] = status;

        timeOffset[sensor][nextTimeIndex[sensor]] = timeOffsetBetweenDate(lastDate[sensor],readTime);

        lastDate[sensor] = readTime;

        nextTimeIndex[sensor] = (nextTimeIndex[sensor]+1)%NUM_TIME;
        nextStatusIndex[sensor] = (nextStatusIndex[sensor] + 1)%NUM_READ;
    }
}

void updateStatusOfSensor(Sensor sensor)
{
    UINT8 byte3,test;
    UINT8 i;
    SensorStatus res;
    
    BIT interruptEnable;

    if(!isSensorReadReady)
    {
        prepareForSensorRead();
    }

    res.sensor = sensor;

    interruptEnable = INTCONbits.GIEH;
    INTCONbits.GIEH = 0; //disable interrupts

    if(sensor==0)
    {
        Nop();
    }

    CLOCK_PORT = 1;

    SPI_TIME_OFFSET
    //SPI_TIME_OFFSET
    //SPI_TIME_OFFSET

    readTime = ReadTimer1();

    selectSensor(sensor);

    // Waiting 500 ns for sensor to be ready (Period = 83 ns => about 7 cycles, waiting 20)

    SPI_TIME_OFFSET
    //SPI_TIME_OFFSET
    //SPI_TIME_OFFSET
    //SPI_TIME_OFFSET
    

    res.position = 0;

    for(i=0 ; i<13 ; i++)
    {
        res.position <<= 1;

        CLOCK_PORT = 1;

        SPI_TIME_OFFSET_UP

        //res.position += DATA_IN;

        CLOCK_PORT = 0;

        SPI_TIME_OFFSET

        res.position += DATA_IN;
    }

    byte3 = 0;

    for(i=0 ; i<6 ; i++)
    {
        byte3 <<= 1;

        CLOCK_PORT = 1;

        SPI_TIME_OFFSET_UP

        //byte3 += DATA_IN;

        CLOCK_PORT = 0;

        SPI_TIME_OFFSET

        byte3 += DATA_IN;


    }

    test = 0;

    for(i=0 ; i<8 ; i++)
    {
        test <<= 1;

        CLOCK_PORT = 1;

        SPI_TIME_OFFSET_UP

        //byte3 += DATA_IN;

        CLOCK_PORT = 0;

        SPI_TIME_OFFSET

        test += DATA_IN;


    }

    /*byte1 = ReadSPI();
    byte2 = ReadSPI();
    byte3 = ReadSPI();*/    

    
    
    SPI_TIME_OFFSET

    LATA |= 0x3F; // Return to waiting state

    SPI_TIME_OFFSET

    CLOCK_PORT = 1;

    INTCONbits.GIEH=interruptEnable;

    /*res.position = 0;

    res.position += byte1;
    res.position <<= 4;
    res.position += (byte2 >> 4);*/

    res.error = 0;

    if(OCF)
    {
        res.error += SENSOR_ERROR_OCF_NOT_FINISHED;
    }
    if(COF)
    {
        res.error += SENSOR_ERROR_OVERFLOW;
    }
    if(LIN)
    {
        res.error += SENSOR_ERROR_LINEARITY;
    }
    if(MAGDEC)
    {
        res.error += SENSOR_ERROR_MAG_DEC;
    }
    if(MAGINC)
    {
        res.error += SENSOR_ERROR_MAG_INC;
    }

    if(!checkParity(res.position,byte3))
    {
        res.error += SENSOR_ERROR_PARITY;
    }

    if(sensor==0)
    {
        Nop();
    }

    if(!OCF)
    {
        res.error = byte3;
        updateSensorStatus(res,sensor);
    }
}

SensorDataPacket getDataPacketForSensor(Sensor sensor)
{
    SensorDataPacket res;
    UINT8 i;
    UINT8 index;

    res.sensor = sensor;
    
    index = nextStatusIndex[sensor];

    for(i=0 ; i<NUM_READ ; i++)
    {
        res.lastStatus[i] = lastStatus[sensor][index];
        index = (index+1)%NUM_READ;
    }

    index = nextTimeIndex[sensor];

    for(i=0 ; i<NUM_TIME ; i++)
    {
        res.timeOffset[i] = timeOffset[sensor][index];
        index = (index+1)%NUM_READ;
    }

    return res;
}

BOOL checkParity(UINT16 position, UINT8 status)
{
    UINT8 sum = 0;
    
    BOOL parity = status & 0x01;
    
    status &= 0xFE;

    while(position!=0)
    {
        sum += position & 0x0001;
        position >>= 1;
    }

    while(status!=0)
    {
        sum += status & 0x0001;
        status >>= 1;
    }

    return parity == (sum & 0x01);
}

void setReadTime(UINT16 time)
{
    readTime = time;
}

void updateSensors(void)
{
    UINT8 i;
    shouldUpdateSensors = FALSE;

    WriteTimer2(0x0);

    for(i=0 ; i<6 ; i++)
    {
        updateStatusOfSensor(i);
    }
}
