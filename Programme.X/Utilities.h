//
//  Utilities.h
//  PSC
//
//  Created by Quentin Fiard on 17/03/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "GenericTypeDefs.h"

void eeprom_write_block(void *ptr, UINT8 addr, UINT8 len);

void eeprom_read_block(UINT8 addr, void *res, UINT8 len);
