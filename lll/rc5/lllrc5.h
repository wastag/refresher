//////////////////////////////////////////////////////////////////////////
/// \file rc5ints.h
///
/// \brief Header file
///
///    All rights released
///
/// \author Jacek Wastag    jwastag@gmail.com
///
//////////////////////////////////////////////////////////////////////////

#ifndef RC5INTS_H_
#define RC5INTS_H_

#include "typedefs.h"

typedef struct 
{
	uint16_t positiveFrame;
	uint16_t negativeFrame;
} RC5Frames_t;

void lllrc5InitModule(void);
void lllrc5GetDataFrames(RC5Frames_t * rc5_frames);
void lllrc5dataCompleteCallbackRegister(pFunction_t callback);
void lllrc5DisableDecoder(void);

#endif /* RC5INTS_H_ */