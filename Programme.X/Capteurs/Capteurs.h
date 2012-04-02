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

#include "CommunicationProtocol.h"

SensorDataPacket getDataPacketForSensor(Sensor sensor);

BOOL checkParity(UINT16 position, UINT8 status);

void prepareForSensorRead(void);

void setReadTime(UINT16 time);

void updateSensors(void);

#endif