//
//  CommunicationUSB.c
//  PSC
//
//  Created by Quentin Fiard on 29/02/12.
//  Copyright (c) 2011 École Polytechnique. All rights reserved.
//

#include "CommunicationUSB.h"

#include "usb.h"
#include "usb_function_cdc.h"
#include "HardwareProfile.h"
#include "usb_config.h"
#include "usb_device.h"

#include "ControleMoteur.h"
#include "Capteurs.h"

#include "CommunicationProtocol.h"

typedef enum
{
    PR_NOREQUEST = 0,
    PR_ECHO,        // Debug mode, where the pic reemit everything he has received
    PR_MOTOR_CONTROL,     // Toggle motor speed or position control
    PR_READSENSOR
} ProcessRequest;

typedef enum
{
    MC_SET_POSITION
} MC_ACTION;

#pragma idata

static ProcessRequest currentMode = PR_NOREQUEST;

UINT8 dataAvailable = 0;
UINT8 currentByte = 0;
UINT8 nextEmptyByte = 0;

// MOTOR_CONTROL
static BOOL waitingForActionChoice = FALSE;
static BOOL waitingForMotorID = FALSE;
static BOOL waitingForAsservissementType = FALSE;

#pragma udata

char USB_In_Buffer[64];
char USB_Out_Buffer[64];

// MOTOR_CONTROL
static MC_ACTION action;
static Moteur motorID;
static TypeAsservissement typeAsservissement;

static void handleMotorControlRequest(void);

void loadNewData(void)
{
    if(dataAvailable==0)
    {
        dataAvailable = getsUSBUSART(USB_In_Buffer,64);
        nextEmptyByte = dataAvailable;
        currentByte = 0;

        return;
    }
    if(dataAvailable<64)
    {
        UINT8 oldDataAvailable = dataAvailable;
        if(currentByte<=nextEmptyByte)
        {
            dataAvailable += getsUSBUSART(USB_In_Buffer+nextEmptyByte,64-nextEmptyByte);
            dataAvailable += getsUSBUSART(USB_In_Buffer,currentByte);
        }
        else
        {
            dataAvailable += getsUSBUSART(USB_In_Buffer+nextEmptyByte,currentByte-nextEmptyByte);
        }
        nextEmptyByte = (nextEmptyByte+(dataAvailable-oldDataAvailable))%64;
    }
}

void prepareUSB(void)
{
    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
    					//variables to known states.

    USBDeviceAttach(); // Activate USB Interruptions
}

static void echo(void)
{
    loadNewData();
    if(dataAvailable>=1)
    {
        UINT8 i;

        for(i=0;i<dataAvailable;i++)
        {
            if(USB_In_Buffer[(currentByte+i)%64] == 0)
            {
                currentMode = PR_NOREQUEST;
                break;
            }
            USB_Out_Buffer[i] = USB_In_Buffer[(currentByte+i)%64];

        }

        putUSBUSART(USB_Out_Buffer,i);
        
        currentByte = (currentByte+i)%64;

        dataAvailable -= i;
    }
}

static void continueWithCurrentMode(void)
{
    if(currentMode == PR_ECHO)
    {
        echo();
    }
    else if(currentMode == PR_MOTOR_CONTROL)
    {
        handleMotorControlRequest();
    }
    else if(currentMode == PR_READSENSOR)
    {
    	if(dataAvailable==0)
        {
            loadNewData();
        }
    	if(dataAvailable>=1)
    	{
            SensorDataPacket toSend;
            Sensor sensor;

            sensor = USB_In_Buffer[currentByte];
            currentByte = (currentByte+1)%64;
            dataAvailable--;

            currentMode = PR_NOREQUEST;

            toSend = getDataPacketForSensor(sensor);

            *((SensorDataPacket*)USB_Out_Buffer) = toSend;

            putUSBUSART(USB_Out_Buffer,sizeof(SensorDataPacket));
    	}
    }
}

static void handleMotorControlAction(void)
{
    if(action == MC_SET_POSITION)
    {
        if(dataAvailable<4)
    	{
            loadNewData();
    	}
    	if(dataAvailable>=4)
    	{
            float position;

            position = *((float*)(USB_In_Buffer+currentByte));

            setRelativePositionObjectiveForMotor(position,motorID);

            currentMode = PR_NOREQUEST;
            
            currentByte = (currentByte+4)%64;
            dataAvailable -= 4;
        }
    }
    else if(action == MC_SET_SPEED)
    {

    }
    else if(action == MC_SET_MIN_SIGNAL_DURATION)
    {
        if(dataAvailable<4)
    	{
            loadNewData();
    	}
    	if(dataAvailable>=4)
    	{
            float newDuration;

            newDuration = *((float*)(USB_In_Buffer+currentByte));

            setMinSignalDuration(newDuration);

            currentMode = PR_NOREQUEST;

            currentByte = (currentByte+4)%64;
            dataAvailable -= 4;
        }
    }
    else if(action == MC_SET_MAX_SIGNAL_DURATION)
    {
        if(dataAvailable<4)
    	{
            loadNewData();
    	}
    	if(dataAvailable>=4)
    	{
            float newDuration;

            newDuration = *((float*)(USB_In_Buffer+currentByte));

            setMaxSignalDuration(newDuration);

            currentMode = PR_NOREQUEST;

            currentByte = (currentByte+4)%64;
            dataAvailable -= 4;
        }
    }
    else if(action == MC_READ_MIN_SIGNAL_DURATION)
    {
        float duration = readMinSignalDuration();

        *((float*)USB_Out_Buffer) = duration;

        putUSBUSART(USB_Out_Buffer,4);

        currentMode = PR_NOREQUEST;
    }
    else if(action == MC_READ_MAX_SIGNAL_DURATION)
    {
        float duration = readMaxSignalDuration();

        *((float*)USB_Out_Buffer) = duration;

        putUSBUSART(USB_Out_Buffer,4);

        currentMode = PR_NOREQUEST;
    }
    else if(action == MC_SET_PID_SPEED_COEFFS)
    {
        if(dataAvailable<12)
    	{
            loadNewData();
    	}
    	if(dataAvailable>=12)
    	{
            PID_Coeffs newCoeffs;

            newCoeffs.gainProportionnel = *((float*)(USB_In_Buffer+currentByte));

            currentByte = (currentByte+4)%64;
            dataAvailable -= 4;

            newCoeffs.gainDifferentiel = *((float*)(USB_In_Buffer+currentByte));

            currentByte = (currentByte+4)%64;
            dataAvailable -= 4;

            newCoeffs.gainIntegral = *((float*)(USB_In_Buffer+currentByte));

            currentByte = (currentByte+4)%64;
            dataAvailable -= 4;

            setSpeedPIDCoeffs(&newCoeffs);

            currentMode = PR_NOREQUEST;
        }
    }
    else if(action == MC_SET_PID_POSITION_COEFFS)
    {
        if(dataAvailable<12)
    	{
            loadNewData();
    	}
    	if(dataAvailable>=12)
    	{
            PID_Coeffs newCoeffs;

            newCoeffs.gainProportionnel = *((float*)(USB_In_Buffer+currentByte));

            currentByte = (currentByte+4)%64;
            dataAvailable -= 4;

            newCoeffs.gainDifferentiel = *((float*)(USB_In_Buffer+currentByte));

            currentByte = (currentByte+4)%64;
            dataAvailable -= 4;

            newCoeffs.gainIntegral = *((float*)(USB_In_Buffer+currentByte));

            currentByte = (currentByte+4)%64;
            dataAvailable -= 4;

            setPositionPIDCoeffs(&newCoeffs);

            currentMode = PR_NOREQUEST;
        }
    }
    else if(action == MC_READ_PID_SPEED_COEFFS)
    {
        PID_Coeffs* coeffs = readSpeedPIDCoeffs();

        float* ptr = (float*)USB_Out_Buffer;

        *ptr = coeffs->gainProportionnel;
        *(ptr+1) = coeffs->gainDifferentiel;
        *(ptr+2) = coeffs->gainIntegral;

        putUSBUSART(USB_Out_Buffer,12);

        currentMode = PR_NOREQUEST;
    }
    else if(action == MC_READ_PID_POSITION_COEFFS)
    {
        PID_Coeffs* coeffs = readPositionPIDCoeffs();

        float* ptr = (float*)USB_Out_Buffer;

        *ptr = coeffs->gainProportionnel;
        *(ptr+1) = coeffs->gainDifferentiel;
        *(ptr+2) = coeffs->gainIntegral;

        putUSBUSART(USB_Out_Buffer,12);

        currentMode = PR_NOREQUEST;
    }
    else if(action == MC_READ_CHOIX_ASSERVISSEMENT)
    {
        ChoixAsservissement choix = readChoixAsservissement();
        UINT8 size = 1;
        USB_Out_Buffer[0] = choix.type;
        

        if(choix.type != ASSERVISSEMENT_SANS)
        {
            USB_Out_Buffer[1] = choix.motor;
            USB_Out_Buffer[2] = choix.sensor;
            size = 3;
        }

        putUSBUSART(USB_Out_Buffer,size);

        currentMode = PR_NOREQUEST;
    }
    else if(action == MC_SET_CHOIX_ASSERVISSEMENT)
    {
        if(typeAsservissement != ASSERVISSEMENT_SANS)
        {
            if(dataAvailable<2)
            {
                loadNewData();
            }
            if(dataAvailable>=2)
            {
                ChoixAsservissement choix;

                choix.type = typeAsservissement;

                choix.motor = USB_In_Buffer[currentByte];
                currentByte = (currentByte+1)%64;
                dataAvailable -= 1;

                choix.sensor = USB_In_Buffer[currentByte];
                currentByte = (currentByte+1)%64;
                dataAvailable -= 1;

                setChoixAsservissement(choix);

                currentMode = PR_NOREQUEST;
            }
        }
        else
        {
            ChoixAsservissement choix;

            choix.type = typeAsservissement;

            setChoixAsservissement(choix);

            currentMode = PR_NOREQUEST;
        }
        
    }
}

static void handleMotorControlRequest(void)
{
    if(waitingForActionChoice)
    {
    	if(dataAvailable==0)
    	{
            loadNewData();
    	}
    	if(dataAvailable>=1)
    	{
            waitingForActionChoice = FALSE;
            action = USB_In_Buffer[currentByte];

            if(action == MC_SET_POSITION || action == MC_SET_SPEED)
            {
                waitingForMotorID = TRUE;
            }
            else
            {
                waitingForMotorID = FALSE;
            }
            if(action == MC_SET_CHOIX_ASSERVISSEMENT)
            {
                waitingForAsservissementType = TRUE;
            }
            else
            {
                waitingForAsservissementType = FALSE;
            }
            
            currentByte = (currentByte+1)%64;
            dataAvailable--;
        }
    }
    if(waitingForMotorID)
    {
        if(dataAvailable==0)
    	{
            loadNewData();
    	}
    	if(dataAvailable>=1)
    	{
            waitingForMotorID = FALSE;
            motorID = USB_In_Buffer[currentByte];
            
            currentByte = (currentByte+1)%64;
            dataAvailable--;
        }
    }
    if(waitingForAsservissementType)
    {
        if(dataAvailable==0)
    	{
            loadNewData();
    	}
    	if(dataAvailable>=1)
    	{
            waitingForAsservissementType = FALSE;
            typeAsservissement = USB_In_Buffer[currentByte];

            currentByte = (currentByte+1)%64;
            dataAvailable--;
        }
    }
    if(!waitingForActionChoice && !waitingForMotorID)
    {
        handleMotorControlAction();
    }
}

static void handleProcessRequest(UINT8 pr)
{
    if(pr == PR_ECHO)
    {
        currentMode = pr;
    }
    else if(pr == PR_MOTOR_CONTROL)
    {
        waitingForActionChoice = TRUE;
        currentMode = pr;
        handleMotorControlRequest();
    }
    else if(pr == PR_READSENSOR)
    {
    	currentMode = pr;
    	continueWithCurrentMode();
    }
}

/********************************************************************
 * Function:        void ProcessUSBData(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/
void ProcessUSBData(void)
{
    // User Application USB tasks
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

    if(USBUSARTIsTxTrfReady())
    {
        if(currentMode != PR_NOREQUEST)
        {
            continueWithCurrentMode();
        }
        else
        {
            if(dataAvailable==0)
            {
                loadNewData();
            }
            if(dataAvailable > 0)
            {
                UINT8 oldCurrentByte = currentByte;
            	currentByte = (currentByte+1)%64;
                dataAvailable--;

                handleProcessRequest(USB_In_Buffer[oldCurrentByte]);
            }
        }
        
    }

    CDCTxService();
}		//end ProcessIO



// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Example power saving code.  Insert appropriate code here for the desired
	//application behavior.  If the microcontroller will be put to sleep, a
	//process similar to that shown below may be used:

	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause
	//things to not work as intended.


    #if defined(__C30__)
        USBSleepOnSuspend();
    #endif
}

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *
 *					This call back is invoked when a wakeup from USB suspend
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// a few milliseconds of wakeup time, after which the device must be
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.


}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.

	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckCDCRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This
 *					callback function should initialize the endpoints
 *					for the device's usage according to the current
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    CDCInitEP();
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function will only be able to wake up the host if
 *                  all of the below are true:
 *
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.
 *
 *                  If the host has not armed the device to perform remote wakeup,
 *                  then this function will return without actually performing a
 *                  remote wakeup sequence.  This is the required behavior,
 *                  as a USB device that has not been armed to perform remote
 *                  wakeup must not drive remote wakeup signalling onto the bus;
 *                  doing so will cause USB compliance testing failure.
 *
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            This function does nothing and returns quickly, if the USB
 *                  bus and host are not in a suspended condition, or are
 *                  otherwise not in a remote wakeup ready state.  Therefore, it
 *                  is safe to optionally call this function regularly, ex:
 *                  anytime application stimulus occurs, as the function will
 *                  have no effect, until the bus really is in a state ready
 *                  to accept remote wakeup.
 *
 *                  When this function executes, it may perform clock switching,
 *                  depending upon the application specific code in
 *                  USBCBWakeFromSuspend().  This is needed, since the USB
 *                  bus will no longer be suspended by the time this function
 *                  returns.  Therefore, the USB module will need to be ready
 *                  to receive traffic from the host.
 *
 *                  The modifiable section in this routine may be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of ~3-15 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at least 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;

    //First verify that the host has armed us to perform remote wakeup.
    //It does this by sending a SET_FEATURE request to enable remote wakeup,
    //usually just before the host goes to standby mode (note: it will only
    //send this SET_FEATURE request if the configuration descriptor declares
    //the device as remote wakeup capable, AND, if the feature is enabled
    //on the host (ex: on Windows based hosts, in the device manager
    //properties page for the USB device, power management tab, the
    //"Allow this device to bring the computer out of standby." checkbox
    //should be checked).
    if(USBGetRemoteWakeupStatus() == TRUE)
    {
        //Verify that the USB bus is in fact suspended, before we send
        //remote wakeup signalling.
        if(USBIsBusSuspended() == TRUE)
        {
            USBMaskInterrupts();

            //Clock switch to settings consistent with normal USB operation.
            USBCBWakeFromSuspend();
            USBSuspendControl = 0;
            USBBusIsSuspended = FALSE;  //So we don't execute this code again,
                                        //until a new suspend condition is detected.

            //Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            //device must continuously see 5ms+ of idle on the bus, before it sends
            //remote wakeup signalling.  One way to be certain that this parameter
            //gets met, is to add a 2ms+ blocking delay here (2ms plus at
            //least 3ms from bus idle to USBIsBusSuspended() == TRUE, yeilds
            //5ms+ total delay since start of idle).
            delay_count = 3600U;
            do
            {
                delay_count--;
            }while(delay_count);

            //Now drive the resume K-state signalling onto the USB bus.
            USBResumeControl = 1;       // Start RESUME signaling
            delay_count = 1800U;        // Set RESUME line for 1-13 ms
            do
            {
                delay_count--;
            }while(delay_count);
            USBResumeControl = 0;       //Finished driving resume signalling

            USBUnmaskInterrupts();
        }
    }
}


/*******************************************************************
 * Function:        void USBCBEP0DataReceived(void)
 *
 * PreCondition:    ENABLE_EP0_DATA_RECEIVED_CALLBACK must be
 *                  defined already (in usb_config.h)
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called whenever a EP0 data
 *                  packet is received.  This gives the user (and
 *                  thus the various class examples a way to get
 *                  data that is received via the control endpoint.
 *                  This function needs to be used in conjunction
 *                  with the USBCBCheckOtherReq() function since
 *                  the USBCBCheckOtherReq() function is the apps
 *                  method for getting the initial control transfer
 *                  before the data arrives.
 *
 * Note:            None
 *******************************************************************/
#if defined(ENABLE_EP0_DATA_RECEIVED_CALLBACK)
void USBCBEP0DataReceived(void)
{
}
#endif

/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch( (INT)event )
    {
        case EVENT_TRANSFER:
            //Add application specific callback task or callback function here if desired.
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER_TERMINATED:
            //Add application specific callback task or callback function here if desired.
            //The EVENT_TRANSFER_TERMINATED event occurs when the host performs a CLEAR
            //FEATURE (endpoint halt) request on an application endpoint which was
            //previously armed (UOWN was = 1).  Here would be a good place to:
            //1.  Determine which endpoint the transaction that just got terminated was
            //      on, by checking the handle value in the *pdata.
            //2.  Re-arm the endpoint if desired (typically would be the case for OUT
            //      endpoints).
            break;
        default:
            break;
    }
    return TRUE;
}
