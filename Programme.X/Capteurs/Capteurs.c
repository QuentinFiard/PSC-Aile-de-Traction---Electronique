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

#define OCF ((byte2 & 0x08) == 0)
#define COF ((byte2 & 0x04) != 0)
#define LIN ((byte2 & 0x02) != 0)
#define MAGINC ((byte2 & 0x01) != 0)
#define MAGDEC ((byte3 & 0x80) != 0)
#define PARITY ((byte3 & 0x40) != 0)

#define SENSOR_PORT0 LATAbits.LATA0
#define SENSOR_PORT1 LATAbits.LATA1
#define SENSOR_PORT2 LATAbits.LATA2
#define SENSOR_PORT3 LATAbits.LATA3
#define SENSOR_PORT4 LATAbits.LATA4
#define SENSOR_PORT5 LATAbits.LATA5

static volatile BOOL isSensorReadReady = FALSE;

void prepareForSensorRead(void)
{
    OpenSPI(SPI_FOSC_64, MODE_10, SMPEND);

    //Initialisation des ports de sélection des capteurs
    LATA |= 0x3F; // Waiting state is high
    ADCON1 |= 0x0F;
    TRISA &= 0xC0; // Ports en sortie

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
    SensorStatus res;

    if(!isSensorReadReady)
    {
        prepareForSensorRead();
    }

    selectSensor(sensor);

    // Waiting 500 ns for sensor to be ready (Period = 83 ns => about 7 cycles)

    Delay10TCYx(2);

    byte1 = ReadSPI();
    byte2 = ReadSPI();
    byte3 = ReadSPI();

    /*byte1 <<= 1;
    byte1 += (byte2 >> 7);
    byte2 <<= 1;
    byte2 += (byte3 >> 7);
    byte3 <<= 1;*/

    LATA |= 0x3F; // Return to waiting state

    byte1 <<= 1;
    byte1 += (byte2>>7);
    byte2 <<= 1;
    byte2 += (byte3>>7);
    byte3 <<= 1;

    res.position = 0;

    res.position += byte1;
    res.position <<= 4;
    res.position += (byte2 >> 4);

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

    if(!checkParity(byte1,byte2,byte3))
    {
        res.error += SENSOR_ERROR_PARITY;
    }

    return res;
}

BOOL checkParity(UINT8 byte1, UINT8 byte2, UINT8 byte3)
{
    UINT8 sum = 0;
    BOOL parity;

    while(byte1!=0)
    {
        sum += (byte1 & 0x01);
        byte1 >>= 1;
    }

    while(byte2!=0)
    {
        sum += (byte2 & 0x01);
        byte2 >>= 1;
    }

    byte3 >>= 6;

    parity = byte3 & 0x01;

    byte3 >>= 1;

    sum += (byte3 & 0x01);

    return parity == (sum & 0x01);
}
