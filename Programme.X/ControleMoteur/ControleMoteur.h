//
//  ControleMoteur.h
//  PSC
//
//  Created by Quentin Fiard on 26/02/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Compiler.h"
#include "GenericTypeDefs.h"

typedef UINT8 Moteur;

typedef INT32 AngularPosition;

void prepareMotorControl(void);

void setRelativeSpeedForMotor(double ratio, Moteur moteur);

void setAngularPositionObjectiveForMotor(AngularPosition objective, Moteur moteur);

void setRelativePositionObjectiveForMotor(double ratio, Moteur moteur);

void stopControllingMotor(Moteur moteur);

void stopMotorSignal(void);

void startNextMotorControlSequence(void);

void openMotor(Moteur moteur);

double positionObjectiveForMotor(Moteur moteur);

void setPositionObjectiveForMotor(Moteur moteur,double objective);
