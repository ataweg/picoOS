#ifndef __PICOOS_CONFIG_H__
#define __PICOOS_CONFIG_H__
// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#define DEFAULT_STACKSIZE     128
#define ROOT_STACKSIZE        128
#define MAX_TASKS             1
#define NUM_SEMAPHORES        8
// #define USE_POS_TIMER

// define semaphores used in this project

#define SysReset           ( 1 << ( NUM_SEMAPHORES - 1 ) )
#define AllSemaphores      ( ( 1 << ( NUM_SEMAPHORES - 1 ) ) - 1 )

#define posTimeout         ( -1  )

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#endif // __PICOOS_CONFIG_H__
