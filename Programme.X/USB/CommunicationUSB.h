//
//  CommunicationUSB.h
//  PSC
//
//  Created by Quentin Fiard on 29/02/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "GenericTypeDefs.h"
#include "Compiler.h"

void prepareUSB(void);

void ProcessUSBData(void);

void USBDeviceTasks(void);

void USBCBSendResume(void);
