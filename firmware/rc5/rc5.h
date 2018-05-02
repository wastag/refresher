//////////////////////////////////////////////////////////////////////////
/// \file rc5.h
///
/// \brief Header file
///
///    All rights released
///
/// \author Jacek Wastag    jwastag@gmail.com
///
//////////////////////////////////////////////////////////////////////////

#ifndef RC5_H_
#define RC5_H_

#include "typedefs.h"

//////////////////////////////////////////////////////////////////////////
/// \brief RC5 commands definitions
//////////////////////////////////////////////////////////////////////////
#define RC5_CODE_STANDBY 12 // stand by
#define RC5_CODE_MUTE    13 // mute button
#define RC5_CODE_VOLUME_UP    16 // volume +
#define RC5_CODE_VOLUME_DOWN  17 // volume -
#define RC5_CODE_CHANNEL_UP 32 // channel+ button
#define RC5_CODE_CHANNEL_DOWN 33 // channel- button
#define RC5_CODE_MENU         59 // menu button

/////////////////////////////////////////////////////////////////////////
/// \brief RC5 - errors enumeration
//////////////////////////////////////////////////////////////////////////
enum eRC5Errors
{
    RC5_NO_ERRORS,
    RC5_DATA_NOT_AVAILABLE,
    RC5_DATA_CORRUPTED,
    RC5_BAD_ADDRESS,
    RC5_BUTTON_HOLD,
};

//////////////////////////////////////////////////////////////////////////
/// \brief API functions types definitions
//////////////////////////////////////////////////////////////////////////
typedef void (RC5InitDecoder_t)(void);
typedef void (RC5EnableDecoder_t)(void);
typedef void (RC5DisableDecoder_t)(void);
typedef void (RC5SetDevicesAddress_t)(unsigned char ucRC5Address);
typedef int16_t (RC5GetCommand_t)(void);
typedef void (RC5dataCompleteCallbackRegister_t)(pFunction_t);

extern RC5InitDecoder_t         rc5InitDecoder;
extern RC5EnableDecoder_t       rc5EnableDecoder;
extern RC5DisableDecoder_t      rc5DisableDecoder;
extern RC5SetDevicesAddress_t   rc5SetDevicesAddress;
extern RC5GetCommand_t          rc5GetCommand;

#endif /* RC5_H_ */
