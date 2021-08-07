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

#if defined( __XC8)
#include "picoOS.h"

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

//      for( i = posCONTEXT_SIZE; i != 0 ; i--)
//      {
//         *(unsigned char*)newStackPtr = 0xC5;
//         newStackPtr += posSTACK_GROWTH;
//     }

stackPtrType posInitialiseStack( stackPtrType newStackPtr, taskFunctionType task )
{
   unsigned char ucBlock;
   unsigned long ulAddress;

   /* Place a few bytes of known values on the bottom of the stack.
   This is just useful for debugging. */

   *( unsigned char* )newStackPtr = 0x11;
   newStackPtr++;
   *( unsigned char* )newStackPtr = 0x22;
   newStackPtr++;
   *( unsigned char* )newStackPtr = 0x33;
   newStackPtr++;

   /* Simulate how the stack would look after a call to vPortYield() generated
   by the compiler. */

   /* Next we just leave a space.  When a context is saved the stack pointer
   is incremented before it is used so as not to corrupt whatever the stack
   pointer is actually pointing to.  This is especially necessary during
   function epilogue code generated by the compiler. */
   *( unsigned char* )newStackPtr = 0x44;
   newStackPtr++;

   /* Next are all the registers that form part of the task context. */

   *( unsigned char* )newStackPtr = 0x66; /* WREG. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0xcc; /* Status. */
   newStackPtr++;

   /* INTCON is saved with interrupts enabled. */
   *( unsigned char* )newStackPtr = posINITAL_INTERRUPT_STATE; /* INTCON */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x11; /* BSR. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x22; /* FSR2L. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x33; /* FSR2H. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x44; /* FSR0L. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x55; /* FSR0H. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x66; /* TABLAT. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x00; /* TBLPTRU. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x88; /* TBLPTRUH. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x99; /* TBLPTRUL. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0xaa; /* PRODH. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0xbb; /* PRODL. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x00; /* PCLATU. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x00; /* PCLATH. */
   newStackPtr++;

   /* Next the .tmpdata and MATH_DATA sections. */
   for( ucBlock = 0; ucBlock <= posCOMPILER_MANAGED_MEMORY_SIZE; ucBlock++ )
   {
      *( unsigned char* )newStackPtr = ucBlock;
      *( unsigned char* )newStackPtr++;
   }

   /* Store the top of the global data section. */
   *( unsigned char* )newStackPtr = posCOMPILER_MANAGED_MEMORY_SIZE; /* Low. */
   newStackPtr++;

   *( unsigned char* )newStackPtr = 0x00; /* High. */
   newStackPtr++;

   /* The only function return address so far is the address of the
   task. */
   ulAddress = ( unsigned long ) task;

   /* TOS low. */
   *( unsigned char* )newStackPtr = ( ulAddress & ( unsigned long ) 0x00ff );
   newStackPtr++;
   ulAddress >>= 8;

   /* TOS high. */
   *( unsigned char* )newStackPtr = ( ulAddress & ( unsigned long ) 0x00ff );
   newStackPtr++;
   ulAddress >>= 8;

   /* TOS even higher. */
   *( unsigned char* )newStackPtr = ( ulAddress & ( unsigned long ) 0x00ff );
   newStackPtr++;

   /* Store the number of return addresses on the hardware stack - so far only
   the address of the task entry point. */
   *( unsigned char* )newStackPtr = 1;
   newStackPtr++;

   return newStackPtr;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
#endif