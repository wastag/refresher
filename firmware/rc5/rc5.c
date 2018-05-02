//////////////////////////////////////////////////////////////////////////
///
/// \file rc5.c
///
/// \brief Module contains functions used to decode RC5 data
///
///  All rights released
///
/// \author Jacek Wastag    jwastag@gmail.com
///
//////////////////////////////////////////////////////////////////////////

#include "rc5.h"
#include "rc5data.h"
#include "lllrc5.h"
#include "lllio.h"
#include "lllsys.h"
#include "rc5process.h"
#include "qpn.h"

//////////////////////////////////////////////////////////////////////////
/// \brief 
//////////////////////////////////////////////////////////////////////////
typedef struct
{
    uint8_t remoteDeviceAddress;
    uint8_t command;
    uint8_t fToggleBit          :1;
    uint8_t                     :7;
} RC5Data_t;

//////////////////////////////////////////////////////////////////////////
/// \brief 
//////////////////////////////////////////////////////////////////////////
static sRC5DecoderStatus_t RC5DecoderStatus = 
{
	DEFAULT_DEVICE_ADDRESS,
	FALSE,
};

//////////////////////////////////////////////////////////////////////////
/// \brief The local variables
//////////////////////////////////////////////////////////////////////////
static RC5Data_t rc5Data = 
{
	DEFAULT_DEVICE_ADDRESS, // uint8_t remoteDeviceAddress
	0,						// uint8_t command
	0,						// uint8_t fToggleBit
};

RC5Frames_t rc5framesLocal = 
{
	0x0000,
	0x0000,	
};

/// Local functions prototypes
static void rc5dataReadyCallback(void);
static int16_t decodeRC5(void);

static void rc5dataReadyCallback(void)
{
    lllrc5GetDataFrames(&rc5framesLocal);

	QACTIVE_POST_ISR((QActive *)&AO_RC5Process, RC5_DATA_READY_SIG, 0U);

}

//////////////////////////////////////////////////////////////////////////
/// \brief The function decodes RC5 data depending on the RC5 buffer
///
/// \param None
/// \return None
//////////////////////////////////////////////////////////////////////////
static int16_t decodeRC5(void)
{
    uint16_t rc5Frame = 0x0000;
	int16_t ret_status = RC5_DECODING_OK;

	rc5Frame = (~rc5framesLocal.negativeFrame) & (TOGGLE_BIT_MASK | DEVICE_ADDRESS_MASK | COMMAND_MASK);
	if(rc5framesLocal.positiveFrame != rc5Frame)
	{
	  ret_status = -RC5_DECODING_FRAME_ERROR;
	}
    else
	{
      rc5Data.fToggleBit = (rc5Frame & TOGGLE_BIT_MASK)>>TOGGLE_SHIFT;
      rc5Data.remoteDeviceAddress = (rc5Frame & DEVICE_ADDRESS_MASK)>>ADDRESS_SHIFT;
      rc5Data.command = (rc5Frame&COMMAND_MASK);
    }

	return ret_status;
}

//////////////////////////////////////////////////////////////////////////
/// \brief The function initializes the RC5 decoder
//////////////////////////////////////////////////////////////////////////
void rc5InitDecoder(void)
{
	rc5Data.command = 0;
	rc5Data.fToggleBit = 0;
	rc5Data.remoteDeviceAddress = DEFAULT_DEVICE_ADDRESS;
	rc5framesLocal.negativeFrame = 0x0000;
	rc5framesLocal.positiveFrame = 0x0000;
	lllrc5dataCompleteCallbackRegister(&rc5dataReadyCallback);
	lllrc5InitModule();
	RC5DecoderStatus.rc5DecoderEnabled = TRUE;
}

//////////////////////////////////////////////////////////////////////////
/// \brief The function initializes the RC5 decoder
//////////////////////////////////////////////////////////////////////////
void rc5EnableDecoder(void)
{
	// llldisable
	RC5DecoderStatus.rc5DecoderEnabled = TRUE;
}

//////////////////////////////////////////////////////////////////////////
/// \brief The function initializes the RC5 decoder
//////////////////////////////////////////////////////////////////////////
void rc5DisableDecoder(void)
{
	/// llldisable
	RC5DecoderStatus.rc5DecoderEnabled = FALSE;
}

//////////////////////////////////////////////////////////////////////////
/// \brief The function sets device's rc5 standard address
///
/// \param unsigned char ucRC5Address, - new device's address
/// \return Nothing
//////////////////////////////////////////////////////////////////////////
void rc5SetDevicesAddress(unsigned char ucRC5Address)
{
	RC5DecoderStatus.deviceAddress = ucRC5Address;
}

//////////////////////////////////////////////////////////////////////////
/// \brief The Function returns commands when available
//////////////////////////////////////////////////////////////////////////
int16_t rc5GetCommand(void)
{
           int16_t ret_val = -RC5_DATA_NOT_AVAILABLE;
	static uint8_t previousToggleBitState = 0;
	static uint8_t first = 1;
	
	if (first)
	{
	  first = 0;
	  previousToggleBitState = rc5Data.fToggleBit;
	}

	lllsysDisableInterrupts();

    if(RC5_DECODING_OK > decodeRC5())
	{
      ret_val =-RC5_DATA_CORRUPTED;
	}
	else if(RC5DecoderStatus.deviceAddress != rc5Data.remoteDeviceAddress)
	{
	  ret_val = -RC5_BAD_ADDRESS;
	}
	else
	{
	  switch(rc5Data.command)
	  {
	    // All the commands listed below ignore the toggle bit
	    case RC5_CODE_VOLUME_UP:
	    case RC5_CODE_VOLUME_DOWN:
	    case RC5_CODE_CHANNEL_UP:
	    case RC5_CODE_CHANNEL_DOWN:
		{
		  ret_val = (int16_t)rc5Data.command;
		  break;
		}
	    // All the other commands are managed by the procedure below and are toggle-sensitive
	    default:
		{
		  if(   (previousToggleBitState == rc5Data.fToggleBit) )
		  {
			ret_val = -RC5_BUTTON_HOLD;
		  }
		  else
		  {
		    ret_val = (int16_t)rc5Data.command;
		  }
		  break;
		}
	  }
	  previousToggleBitState = rc5Data.fToggleBit;
	}

	lllsysEnableInterrupts();

	return ret_val;
}
