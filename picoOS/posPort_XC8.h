#ifndef __POSPORT_XC8_H__
#define __POSPORT_XC8_H__
// --------------------------------------------------------------------------
//
// MIT License
//
// Copyright (c) 2021 Axel Werner (ataweg)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// --------------------------------------------------------------------------

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
 #ifdef SMALL_STACK
  // we have a 8 bit stack pointer
  typedef unsigned char stackPtrType;
 #else
 // we have a 16 bit stack pointer
  typedef unsigned short stackPtrType;
 #endif

 #define SP          FSR1

 #include <xc.h>

#else
 # error "wrong compiler, require mplab c18 compiler"
#endif

// Critical section management.
#define posENTER_CRITICAL()                                   \
            asm volatile ( "in   __tmp_reg__, __SREG__" ::);   \
            asm volatile ( "cli" ::);                          \
            asm volatile ( "push __tmp_reg__" ::)

#define posEXIT_CRITICAL()                                    \
            asm volatile ( "pop  __tmp_reg__" ::);             \
            asm volatile ( "out  __SREG__, __tmp_reg__" ::)


/* Critical section management. */
#define posDISABLE_INTERRUPTS()   { INTCONbits.GIEH = 0; }
#define posENABLE_INTERRUPTS()    { INTCONbits.GIEH = 1; }

/* Initial interrupt enable state for newly created tasks.  This value is
copied into INTCON when a task switches in for the first time. */
#define posINITAL_INTERRUPT_STATE        0xc0

/* Just the bit within INTCON for the global interrupt flag. */
#define posGLOBAL_INTERRUPT_FLAG         0x80

/* Constant used for context switch macro when we require the interrupt
enable state to be unchanged when the interrupted task is switched back in. */
#define posINTERRUPTS_UNCHANGED       0x00

/* Some memory areas get saved as part of the task context.  These memory
area's get used by the compiler for temporary storage, especially when
performing mathematical operations, or when using 32bit data types.  This
constant defines the size of memory area which must be saved. */
#define posCOMPILER_MANAGED_MEMORY_SIZE  ( ( unsigned char ) 0x13 )

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#define posSTACK_GROWTH            1
#define posCONTEXT_SIZE            (26+portCOMPILER_MANAGED_MEMORY_SIZE +3+16)

#ifndef DEFAULT_STACKSIZE
#define DEFAULT_STACKSIZE          (64+portCONTEXT_SIZE)
#endif

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

// for microchip PIC controller
//   save the callers return address in the HW stack to the SW stack
//   save the framepointer FP = FSR2
//   save the stackpointer SP = FSR1
//   save the bankselect register BSR
//   save the complete(?) HW stack
// ...

#define  posSAVE_CONTEXT()             \
{                                       \
   /* Save the status and WREG registers first, as these will get modified \
   by the operations below. */          \
   asm( "MOVFF WREG, PREINC1        "); \
   asm( "MOVFF STATUS, PREINC1      "); \
                                        \
   posDISABLE_INTERRUPTS();            \
                                        \
   /* Store the necessary registers to the stack. */ \
   asm( "MOVFF BSR, PREINC1         "); \
   asm( "MOVFF FSR2L, PREINC1       "); \
   asm( "MOVFF FSR2H, PREINC1       "); \
   asm( "MOVFF FSR0L, PREINC1       "); \
   asm( "MOVFF FSR0H, PREINC1       "); \
   asm( "MOVFF TABLAT, PREINC1      "); \
   asm( "MOVFF TBLPTRU, PREINC1     "); \
   asm( "MOVFF TBLPTRH, PREINC1     "); \
   asm( "MOVFF TBLPTRL, PREINC1     "); \
   asm( "MOVFF PRODH, PREINC1       "); \
   asm( "MOVFF PRODL, PREINC1       "); \
   asm( "MOVFF PCLATU, PREINC1      "); \
   asm( "MOVFF PCLATH, PREINC1      "); \
   /* Store the .tempdata and MATH_DATA areas as described above. */ \
   asm( "CLRF  FSR0L                "); \
   asm( "CLRF  FSR0H                "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF POSTINC0, PREINC1    "); \
   asm( "MOVFF INDF0, PREINC1       "); \
   asm( "MOVFF FSR0L, PREINC1       "); \
   asm( "MOVFF FSR0H, PREINC1       "); \
   /* Store the hardware stack pointer in a temp register before we \
   modify it. */                        \
   asm( "MOVFF STKPTR, FSR0L        "); \
                                        \
   /* Store each address from the hardware stack. */ \
   while( STKPTR > (stackPtrType) 0 )   \
   {                                    \
      asm( "MOVFF TOSL, PREINC1     "); \
      asm( "MOVFF TOSH, PREINC1     "); \
      asm( "MOVFF TOSU, PREINC1     "); \
      asm( "POP                     "); \
   }                                    \
                                        \
   /* Store the number of addresses on the hardware stack (from the \
   temporary register). */              \
   asm(" MOVFF FSR0L, PREINC1       "); \
   asm(" MOVF  PREINC1, F           "); \
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

// FSR1  => SP
// FSR0  => working regsiter, index

#define posRESTORE_CONTEXT()           \
{                                       \
   /* How many return addresses are there on the hardware stack?  Discard  \
   the first byte as we are pointing to the next free space. */ \
   asm( "MOVFF POSTDEC1, FSR0L      "); \
   asm( "MOVFF POSTDEC1, FSR0L      "); \
                                        \
   /* Fill the hardware stack from our software stack. */ \
   STKPTR = 0;                          \
                                        \
   while( STKPTR < FSR0L )              \
   {                                    \
      asm(" PUSH                    "); \
      asm(" MOVF  POSTDEC1, W       "); \
      asm(" MOVWF TOSU              "); \
      asm(" MOVF  POSTDEC1, W       "); \
      asm(" MOVWF TOSH              "); \
      asm(" MOVF  POSTDEC1, W       "); \
      asm(" MOVWF TOSL              "); \
   }                                    \
                                        \
   /* Restore the .tmpdata and MATH_DATA memory. */ \
   asm( "MOVFF POSTDEC1, FSR0H      "); \
   asm( "MOVFF POSTDEC1, FSR0L      "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, POSTDEC0   "); \
   asm( "MOVFF POSTDEC1, INDF0      "); \
   /* Restore the other registers forming the tasks context. */ \
   asm( "MOVFF POSTDEC1, PCLATH     "); \
   asm( "MOVFF POSTDEC1, PCLATU     "); \
   asm( "MOVFF POSTDEC1, PRODL      "); \
   asm( "MOVFF POSTDEC1, PRODH      "); \
   asm( "MOVFF POSTDEC1, TBLPTRL    "); \
   asm( "MOVFF POSTDEC1, TBLPTRH    "); \
   asm( "MOVFF POSTDEC1, TBLPTRU    "); \
   asm( "MOVFF POSTDEC1, TABLAT     "); \
   asm( "MOVFF POSTDEC1, FSR0H      "); \
   asm( "MOVFF POSTDEC1, FSR0L      "); \
   asm( "MOVFF POSTDEC1, FSR2H      "); \
   asm( "MOVFF POSTDEC1, FSR2L      "); \
   asm( "MOVFF POSTDEC1, BSR        "); \
   asm( "MOVFF POSTDEC1, STATUS     "); \
   asm( "MOVFF POSTDEC1, WREG       "); \
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#define posPushShort( value)

#define posPopShort( value)

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#endif // __POSPORT_XC8_H__
