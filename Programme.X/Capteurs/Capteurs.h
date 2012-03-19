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

BOOL checkParity(SensorStatus status, BIT parity);

void prepareForSensorRead(void);

#endif