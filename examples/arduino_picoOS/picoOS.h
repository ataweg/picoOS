#ifndef __pico_OS_H__
#define __pico_OS_H__
// --------------------------------------------------------------------------
//
// Project       MidiMerge
//
// File          Axel Werner
//
// Author        picoOS.h
//
// --------------------------------------------------------------------------
// Changelog
//
//    08.11.2012  AWe   start implemtation
//
// --------------------------------------------------------------------------

#include "picoOS_config.h"    // MAX_TASKS, semaphores

#if defined(__ICCAVR__)  /* This is IAR, not Imagecraft! */
   #include "posPort_ICCAVR.h"

#elif defined(__AVR__)
   #include "posPort_AVRGCC.h"

#elif defined(__RL78__ )
   #include "posPort_RL78.h"

#elif defined( __18CXX)
   #include "posPort_18CXX.h"

#elif defined( __XC8)
   #include "posPort_XC8.h"

#endif

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#ifndef MAX_TASKS
   // number of task which we can create
   #define MAX_TASKS          2
#endif

#ifndef DEFAULT_STACKSIZE
   #define DEFAULT_STACKSIZE  128
#endif

#ifndef ROOT_STACKSIZE
   #define ROOT_STACKSIZE     128
#endif

#define NEXT_TASK             -1
#define ROOT_TASK              0

#if NUM_SEMAPHORES <= 8
   #define posSemaphores_t       unsigned char
#elif NUM_SEMAPHORES <= 16
   #define posSemaphores_t       unsigned short
#else
   #define posSemaphores_t       unsigned long
#endif

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Defines the prototype to which task functions must conform. Defined in this
 * file to ensure the type is known before portable.h is included.
 */
// for the picoOS the task functions don't have any parameters
typedef void ( *taskFunctionType )( void );

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void posInit( stackSizeType root_stack_size );
unsigned char posCreateTask( taskFunctionType task, stackSizeType stack_size );
void posTaskSwitch( signed char task_num );
void posWait( unsigned char( *waitForReadyFunc )( void ) );
short posCheckStack( void );
stackPtrType posGetStackEnd( signed char task_num );
stackPtrType posGetCurrentStackEnd( void );

void yield( void );

void posDelay( unsigned long timeout );
void posSemaphoreGive( posSemaphores_t semaphores );
posSemaphores_t posSemaphoreTake( posSemaphores_t semaphores );
posSemaphores_t posSemaphoreGet( posSemaphores_t semaphores );
posSemaphores_t posSemaphoreWait( posSemaphores_t semaphores, unsigned long timeout );

// some build in Mutex, Semaphores
posSemaphores_t posMemoryLock( void );
posSemaphores_t posMemoryUnlock( void );

// --------------------------------------------------------------------------
// post related functions
// --------------------------------------------------------------------------

stackPtrType posInitialiseStack( stackPtrType newStackPtr, taskFunctionType task ); // defined in posPort_XXX.c

#ifdef USE_POS_TIMER
void posTimerSetup( stackPtrType timerTicksPerSecond );
void posTimerISR( void );
unsigned long posMillis( void );
#endif
// macros defined in the pos impemetation
// posDISABLE_INTERRUPTS()
// posENABLE_INTERRUPTS()
// posSAVE_CONTEXT()
// posRESTORE_CONTEXT()
// stackPtrType posPushShort( value )
// stackPtrType posPopShort( value )

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#define posNextTask()            posTaskSwitch( NEXT_TASK)
#define posStartTask( taskID)    posTaskSwitch( taskID)

#ifdef __cplusplus
}
#endif

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
#endif // __pico_OS_H__
