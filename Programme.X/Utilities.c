//
//  Utilities.c
//  PSC
//
//  Created by Quentin Fiard on 17/03/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Utilities.h"
#include "Compiler.h"
#include "CommunicationProtocol.h"

static void eeprom_write(UINT8 addr, UINT8 byte)
{
    UINT8 i=INTCONbits.GIEH;
    INTCONbits.GIEH = 0; //disable interrupts
    EECON1bits.EEPGD=0; //Write to EEPROM
    EECON1bits.CFGS=0; //EEPROM not config bits
    EECON1bits.WREN=1; //Allows write

    EEADR=addr;
    EEDATA=byte;
    EECON2=0x55;
    EECON2=0xaa;
    EECON1bits.WR=1;
    while(EECON1bits.WR); //Wait until written
    //while(!PIR2bits.EEIF);
    //PIR2bits.EEIF=0;
    EECON1bits.WREN=0; //No more write
    INTCONbits.GIEH = i; //restore interrupts
}



static UINT8 eeprom_read(UINT8 addr)
{
    EECON1bits.CFGS=0; //EEPROM not config bits
    EECON1bits.EEPGD=0;
    EEADR=addr;
    EECON1bits.RD=1;
    return (UINT8) EEDATA;
}

void eeprom_write_block(void *ptr, UINT8 addr, UINT8 len)
{
    UINT8* data = ptr;

    while (len--) {
        eeprom_write(addr++, *data++);
    }
}

void eeprom_read_block(UINT8 addr, void *res, UINT8 len)
{
    UINT8* data = res;

    while (len--) {
        *data = eeprom_read(addr++);
        data++;
    }
}

INT16 offsetBetweenAngle(UINT16 angle1, UINT16 angle2)
{
    INT16 offset = angle2 - angle1;

    if(fabs(offset) > MAX_SENSOR_ANGLE/2)
    {
        if(offset>0)
        {
            return offset - MAX_SENSOR_ANGLE;
        }
        else
        {
            return offset + MAX_SENSOR_ANGLE;
        }
    }

    return offset;
}