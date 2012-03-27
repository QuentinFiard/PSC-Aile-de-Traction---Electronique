//
//  ControleMoteur.h
//  PSC
//
//  Created by Quentin Fiard on 26/02/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "CommunicationProtocol.h"

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

void setMaxSignalDuration(float duration);

void setMinSignalDuration(float duration);

float readMaxSignalDuration(void);

float readMinSignalDuration(void);

PID_Coeffs* readSpeedPIDCoeffs(void);

PID_Coeffs* readPositionPIDCoeffs(void);

void setSpeedPIDCoeffs(PID_Coeffs* coeffs);

void setPositionPIDCoeffs(PID_Coeffs* coeffs);

ChoixAsservissement readChoixAsservissement(void);
void setChoixAsservissement(ChoixAsservissement choix);

void setPIDPeriod(float period);
float readPIDPeriod(void);
