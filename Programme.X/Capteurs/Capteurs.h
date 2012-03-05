//
//  Capteurs.h
//  PSC
//
//  Created by Quentin Fiard on 26/02/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#ifndef __Capteurs__
#define __Capteurs__

#include "Compiler.h"
#include "GenericTypeDefs.h"

typedef UINT8 Sensor;

typedef enum
{
	SENSOR_ERROR_PARITY = 0x01,
	SENSOR_ERROR_LINEARITY = 0x02,
	SENSOR_ERROR_OCF_NOT_FINISHED = 0x04,
	SENSOR_ERROR_OVERFLOW = 0x08,
        SENSOR_ERROR_MAG_INC = 0x10,
        SENSOR_ERROR_MAG_DEC = 0x20
} SensorError;

typedef struct
{
    Sensor sensor;
    SensorError error;
    UINT16 position;
}  SensorStatus;

SensorStatus getStatusOfSensor(Sensor sensor);

BOOL checkParity(SensorStatus status, BIT parity);

void prepareForSensorRead(void);

#endif