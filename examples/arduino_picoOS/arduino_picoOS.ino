// -----------------------------------------------------------------------------
//
//    2019-01-11  AWe   Test program for Arduino picoOS
//
// -----------------------------------------------------------------------------

#include <Arduino.h>

#include "picoOS.h"


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void other_task( void );

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

unsigned char TaskOK( void)
{
    return 1;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void setup()
{
   Serial.begin( 115200 );
   Serial.println( F( "Arduino picoOS (c) AWe 2019" ) );

   Serial.println( F( "Build " __DATE__ "  " __TIME__ ) );
   Serial.println( F( "libc version " __AVR_LIBC_VERSION_STRING__ ) );
   Serial.println( F( "gcc version " __VERSION__ ) );

   pinMode( DEUBG_LED, OUTPUT );
   digitalWrite( DEUBG_LED, LOW );

   // disable interrupts
   posDISABLE_INTERRUPTS();

  // put your setup code here, to run once:
   posInit( ROOT_STACKSIZE );

   // enable interrupts
   posENABLE_INTERRUPTS();

   unsigned char otherTaskID;

   // create one additional task
   otherTaskID = posCreateTask( other_task, DEFAULT_STACKSIZE );
   posStartTask( otherTaskID );                  // jump to new task
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
     // mainloop, root task

void loop()
{
  // put your main code here, to run repeatedly:

   volatile static int a;

   a++;
   digitalWrite( DEUBG_LED, HIGH );
   Serial.println( F( "B" ) );
   posNextTask();
   posWait( TaskOK);
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

void other_task( void )
{
   do
   {
      volatile static int b;

      b++;
      digitalWrite( DEUBG_LED, LOW );
      Serial.println( F( "A" ) );
      posNextTask();
   }
   while( 1 ); // endless loop
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
