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

SensorStatus getStatusOfSensor(Sensor sensor);

BOOL checkParity(UINT8 byte1, UINT8 byte2, UINT8 byte3);

void prepareForSensorRead(void);

#endif