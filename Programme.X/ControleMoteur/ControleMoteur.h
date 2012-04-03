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

void prepareMotorControl(void);

void setPositionObjective(UINT16 objective);

void setSpeedObjective(float speed);

void stopControllingMotor(Moteur moteur);

void stopMotorSignal(void);

void startNextMotorControlSequence(void);

void openMotor(Moteur moteur);

double signalObjectiveForMotor(Moteur moteur);

void setSignalObjectiveForMotor(Moteur moteur,double objective);

void setMaxSignalDuration(float duration);
void setNeutralSignalDuration(float duration);
void setMinSignalDuration(float duration);

float readMaxSignalDuration(void);
float readNeutralSignalDuration(void);
float readMinSignalDuration(void);

PID_Coeffs* readSpeedPIDCoeffs(void);

PID_Coeffs* readPositionPIDCoeffs(void);

void setSpeedPIDCoeffs(PID_Coeffs* coeffs);

void setPositionPIDCoeffs(PID_Coeffs* coeffs);

ChoixAsservissement readChoixAsservissement(void);
void setChoixAsservissement(ChoixAsservissement choix);

void setPIDPeriod(float period);
float readPIDPeriod(void);

void updateControl(void);
