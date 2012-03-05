#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile

# Environment
# Adding MPLAB X bin directory to path
PATH:=/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/:$(PATH)
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Programme.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Programme.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Capteurs/Capteurs.o ${OBJECTDIR}/ControleMoteur/ControleMoteur.o ${OBJECTDIR}/Interrupts/Interrupts.o ${OBJECTDIR}/main.o ${OBJECTDIR}/USB/CommunicationUSB.o ${OBJECTDIR}/USB/usb_descriptors.o ${OBJECTDIR}/USB/usb_device.o ${OBJECTDIR}/USB/usb_function_cdc.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Capteurs/Capteurs.o.d ${OBJECTDIR}/ControleMoteur/ControleMoteur.o.d ${OBJECTDIR}/Interrupts/Interrupts.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/USB/CommunicationUSB.o.d ${OBJECTDIR}/USB/usb_descriptors.o.d ${OBJECTDIR}/USB/usb_device.o.d ${OBJECTDIR}/USB/usb_function_cdc.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Capteurs/Capteurs.o ${OBJECTDIR}/ControleMoteur/ControleMoteur.o ${OBJECTDIR}/Interrupts/Interrupts.o ${OBJECTDIR}/main.o ${OBJECTDIR}/USB/CommunicationUSB.o ${OBJECTDIR}/USB/usb_descriptors.o ${OBJECTDIR}/USB/usb_device.o ${OBJECTDIR}/USB/usb_function_cdc.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

# Path to java used to run MPLAB X when this makefile was created
MP_JAVA_PATH="/System/Library/Java/JavaVirtualMachines/1.6.0.jdk/Contents/Home/bin/"
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC="/Applications/microchip/mplabc18/v3.40/bin/mcc18"
# MP_BC is not defined
MP_AS="/Applications/microchip/mplabc18/v3.40/bin/../mpasm/MPASMWIN"
MP_LD="/Applications/microchip/mplabc18/v3.40/bin/mplink"
MP_AR="/Applications/microchip/mplabc18/v3.40/bin/mplib"
DEP_GEN=${MP_JAVA_PATH}java -jar "/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/extractobjectdependencies.jar" 
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps
MP_CC_DIR="/Applications/microchip/mplabc18/v3.40/bin"
# MP_BC_DIR is not defined
MP_AS_DIR="/Applications/microchip/mplabc18/v3.40/bin/../mpasm"
MP_LD_DIR="/Applications/microchip/mplabc18/v3.40/bin"
MP_AR_DIR="/Applications/microchip/mplabc18/v3.40/bin"
# MP_BC_DIR is not defined

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Programme.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F2550
MP_PROCESSOR_OPTION_LD=18f2550
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0x7dc0 -u_DEBUGCODELEN=0x240 -u_DEBUGDATASTART=0x3f4 -u_DEBUGDATALEN=0xb
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Capteurs/Capteurs.o: Capteurs/Capteurs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Capteurs 
	@${RM} ${OBJECTDIR}/Capteurs/Capteurs.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/Capteurs/Capteurs.o   Capteurs/Capteurs.c 
	@${DEP_GEN} -d ${OBJECTDIR}/Capteurs/Capteurs.o 
	
${OBJECTDIR}/ControleMoteur/ControleMoteur.o: ControleMoteur/ControleMoteur.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ControleMoteur 
	@${RM} ${OBJECTDIR}/ControleMoteur/ControleMoteur.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/ControleMoteur/ControleMoteur.o   ControleMoteur/ControleMoteur.c 
	@${DEP_GEN} -d ${OBJECTDIR}/ControleMoteur/ControleMoteur.o 
	
${OBJECTDIR}/Interrupts/Interrupts.o: Interrupts/Interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Interrupts 
	@${RM} ${OBJECTDIR}/Interrupts/Interrupts.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/Interrupts/Interrupts.o   Interrupts/Interrupts.c 
	@${DEP_GEN} -d ${OBJECTDIR}/Interrupts/Interrupts.o 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/main.o   main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/main.o 
	
${OBJECTDIR}/USB/CommunicationUSB.o: USB/CommunicationUSB.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/CommunicationUSB.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USB/CommunicationUSB.o   USB/CommunicationUSB.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USB/CommunicationUSB.o 
	
${OBJECTDIR}/USB/usb_descriptors.o: USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USB/usb_descriptors.o   USB/usb_descriptors.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USB/usb_descriptors.o 
	
${OBJECTDIR}/USB/usb_device.o: USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_device.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USB/usb_device.o   USB/usb_device.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USB/usb_device.o 
	
${OBJECTDIR}/USB/usb_function_cdc.o: USB/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_function_cdc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USB/usb_function_cdc.o   USB/usb_function_cdc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USB/usb_function_cdc.o 
	
else
${OBJECTDIR}/Capteurs/Capteurs.o: Capteurs/Capteurs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Capteurs 
	@${RM} ${OBJECTDIR}/Capteurs/Capteurs.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/Capteurs/Capteurs.o   Capteurs/Capteurs.c 
	@${DEP_GEN} -d ${OBJECTDIR}/Capteurs/Capteurs.o 
	
${OBJECTDIR}/ControleMoteur/ControleMoteur.o: ControleMoteur/ControleMoteur.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ControleMoteur 
	@${RM} ${OBJECTDIR}/ControleMoteur/ControleMoteur.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/ControleMoteur/ControleMoteur.o   ControleMoteur/ControleMoteur.c 
	@${DEP_GEN} -d ${OBJECTDIR}/ControleMoteur/ControleMoteur.o 
	
${OBJECTDIR}/Interrupts/Interrupts.o: Interrupts/Interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Interrupts 
	@${RM} ${OBJECTDIR}/Interrupts/Interrupts.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/Interrupts/Interrupts.o   Interrupts/Interrupts.c 
	@${DEP_GEN} -d ${OBJECTDIR}/Interrupts/Interrupts.o 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/main.o   main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/main.o 
	
${OBJECTDIR}/USB/CommunicationUSB.o: USB/CommunicationUSB.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/CommunicationUSB.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USB/CommunicationUSB.o   USB/CommunicationUSB.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USB/CommunicationUSB.o 
	
${OBJECTDIR}/USB/usb_descriptors.o: USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_descriptors.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USB/usb_descriptors.o   USB/usb_descriptors.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USB/usb_descriptors.o 
	
${OBJECTDIR}/USB/usb_device.o: USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_device.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USB/usb_device.o   USB/usb_device.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USB/usb_device.o 
	
${OBJECTDIR}/USB/usb_function_cdc.o: USB/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_function_cdc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) --verbose -w3 -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Common" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/ControleMoteur" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/AcquisitionCapteurs" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Interrupts" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/USB" -I"/Users/quentinfiard/Documents/PSC/GitHub/Electronique/Programme.X/Capteurs"  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USB/usb_function_cdc.o   USB/usb_function_cdc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USB/usb_function_cdc.o 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Programme.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG   -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/Programme.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/Programme.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w    -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/Programme.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "/Applications/microchip/mplabx/mplab_ide.app/Contents/Resources/mplab_ide/mplab_ide/modules/../../bin/"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
