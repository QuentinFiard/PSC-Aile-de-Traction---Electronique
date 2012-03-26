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

#define CLOCK_PORT LATBbits.LATB1
#define DATA_IN PORTBbits.RB0;

#define SPI_TIME_OFFSET 5

static volatile BOOL isSensorReadReady = FALSE;

void prepareForSensorRead(void)
{
    //OpenSPI(SPI_FOSC_64, MODE_00, SMPEND);

    //Initialisation des ports de sélection des capteurs
    LATA |= 0x3F; // Waiting state is high
    ADCON1 |= 0x0F;
    TRISA &= 0xC0; // Ports en sortie

    TRISBbits.RB0 = 1; // RB0 en entrée
    CLOCK_PORT = 1;
    TRISBbits.RB1 = 0; // RB1 en sortie


    isSensorReadReady = TRUE;
}

static void selectSensor(Sensor sensor)
{
    LATA |= 0x3F; // Waiting state is high

    LATA -= (0x01 << sensor);
}

SensorStatus getStatusOfSensor(Sensor sensor)
{
    UINT8 byte1,byte2,byte3,test;
    UINT8 i;
    SensorStatus res;

    if(!isSensorReadReady)
    {
        prepareForSensorRead();
    }

    selectSensor(sensor);

    // Waiting 500 ns for sensor to be ready (Period = 83 ns => about 7 cycles, waiting 100)

    Delay10TCYx(100);

    res.position = 0;

    for(i=0 ; i<12 ; i++)
    {
        res.position <<= 1;

        CLOCK_PORT = 0;

        Delay10TCYx(SPI_TIME_OFFSET);

        CLOCK_PORT = 1;

        Delay10TCYx(SPI_TIME_OFFSET);

        res.position += DATA_IN;
    }

    for(i=0 ; i<6 ; i++)
    {
        byte3 <<= 1;

        CLOCK_PORT = 0;

        Delay10TCYx(SPI_TIME_OFFSET);

        CLOCK_PORT = 1;

        Delay10TCYx(SPI_TIME_OFFSET);

        byte3 += DATA_IN;
    }

    /*byte1 = ReadSPI();
    byte2 = ReadSPI();
    byte3 = ReadSPI();*/

    LATA |= 0x3F; // Return to waiting state

    /*res.position = 0;

    res.position += byte1;
    res.position <<= 4;
    res.position += (byte2 >> 4);*/

    res.sensor = sensor;

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

    if(!checkParity(res,PARITY))
    {
        res.error += SENSOR_ERROR_PARITY;
    }

    //res.position = byte1;
    //res.position *= 0x100;
    //res.position += byte2;
    //res.error = byte3;

    return res;
}

BOOL checkParity(SensorStatus status, BOOL parity)
{
    UINT8 sum = 0;
    UINT8 tmp = status.position >> 8;
    UINT8 currentBit;

    sum += tmp & 0x01;
    sum += (tmp >> 1) & 0x01;

    tmp = status.position;

    for(currentBit=0 ; currentBit<8 ; currentBit++)
    {
        sum += (tmp >> currentBit) & 0x01;
    }

    tmp = status.error;

    if(tmp & SENSOR_ERROR_OCF_NOT_FINISHED == 0)
    {
        sum++;
    }
    if(tmp & SENSOR_ERROR_OVERFLOW != 0)
    {
        sum++;
    }
    if(tmp & SENSOR_ERROR_LINEARITY != 0)
    {
        sum++;
    }
    if(tmp & SENSOR_ERROR_MAG_DEC != 0)
    {
        sum++;
    }
    if(tmp & SENSOR_ERROR_MAG_INC != 0)
    {
        sum++;
    }

    if(parity && ((sum & 0x01)!=0))
    {
        return FALSE;
    }
    if(!parity && ((sum & 0x01)==0))
    {
        return FALSE;
    }

    return TRUE;
}
