//////////////////////////////////////////////////////////////////////////
/// \file debugio.h
///
/// \brief Definitions of debugging pins behavior 
///
///    All rights released
///
/// \author Jacek Wastag    jwastag@gmail.com
///
//////////////////////////////////////////////////////////////////////////

#ifndef DEBUGIO_H_
#define DEBUGIO_H_

#include "typedefs.h"
#include "debug.h"
#include "hardwareconfig.h"

#if DEBUG_MODE_ENABLED
#define loopMeasurePinEnable()  LOOP_MEASURE_DIR |= (1<<LOOP_MEASURE_PIN)
#define loopMeasurePinLow()	    LOOP_MEASURE_PORT &= ~(1<<LOOP_MEASURE_PIN)
#define loopMeasurePinHigh()    LOOP_MEASURE_PORT |= (1<<LOOP_MEASURE_PIN)
#else
#define loopMeasurePinEnable()
#define loopMeasurePinLow()
#define loopMeasurePinHigh()
#endif

#if DEBUG_MODE_ENABLED
#define isrMeasurePinEnable()   ISR_MEASURE_DIR |= (1<<ISR_MEASURE_PIN)
#define isrMeasurePinLow()      ISR_MEASURE_PORT &= ~(1<<ISR_MEASURE_PIN)
#define isrMeasurePinHigh()     ISR_MEASURE_PORT |= (1<<ISR_MEASURE_PIN)
#else
#define isrMeasurePinEnable()
#define isrMeasurePinLow()
#define isrMeasurePinHigh()
#endif

#endif /* DEBUGIO_H_ */