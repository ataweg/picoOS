#ifndef __POSPORT_18CXX_H__
#define __POSPORT_18CXX_H__
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

#if defined( __18CXX)
#ifdef SMALL_STACK
// we have a 8 bit stack pointer
typedef unsigned char stackPtrType;
#else
// we have a 16 bit stack pointer
typedef unsigned short stackPtrType;
#endif

#define SP          FSR1

#else
#error "wrong compiler, require mplab c18 compiler"
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
#define posDISABLE_INTERRUPTS()    INTCONbits.GIEH = 0;
#define posENABLE_INTERRUPTS()     INTCONbits.GIEH = 1;

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

#define  posSAVE_CONTEXT()            \
{                                      \
   _asm                                \
      /* Save the status and WREG registers first, as these will get modified \
      by the operations below. */      \
      MOVFF WREG, PREINC1              \
      MOVFF STATUS, PREINC1            \
   _endasm                             \
                                       \
   posDISABLE_INTERRUPTS();           \
                                       \
   _asm                                \
   /* Store the necessary registers to the stack. */ \
      MOVFF BSR, PREINC1               \
      MOVFF FSR2L, PREINC1             \
      MOVFF FSR2H, PREINC1             \
      MOVFF FSR0L, PREINC1             \
      MOVFF FSR0H, PREINC1             \
      MOVFF TABLAT, PREINC1            \
      MOVFF TBLPTRU, PREINC1           \
      MOVFF TBLPTRH, PREINC1           \
      MOVFF TBLPTRL, PREINC1           \
      MOVFF PRODH, PREINC1             \
      MOVFF PRODL, PREINC1             \
      MOVFF PCLATU, PREINC1            \
      MOVFF PCLATH, PREINC1            \
   /* Store the .tempdata and MATH_DATA areas as described above. */ \
      CLRF  FSR0L, 0                   \
      CLRF  FSR0H, 0                   \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF POSTINC0, PREINC1          \
      MOVFF INDF0, PREINC1             \
      MOVFF FSR0L, PREINC1             \
      MOVFF FSR0H, PREINC1             \
      /* Store the hardware stack pointer in a temp register before we \
      modify it. */                    \
      MOVFF STKPTR, FSR0L              \
   _endasm                             \
                                       \
   /* Store each address from the hardware stack. */ \
   while( STKPTR > (stackPtrType) 0 )  \
   {                                   \
         _asm                          \
            MOVFF TOSL, PREINC1        \
            MOVFF TOSH, PREINC1        \
            MOVFF TOSU, PREINC1        \
            POP                        \
         _endasm                       \
   }                                   \
                                       \
   _asm                                \
      /* Store the number of addresses on the hardware stack (from the \
      temporary register). */          \
      MOVFF FSR0L, PREINC1             \
      MOVF  PREINC1, 1, 0              \
   _endasm                             \
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

// FSR1  => SP
// FSR0  => working regsiter, index

#define posRESTORE_CONTEXT()          \
{                                      \
   _asm                                \
      /* How many return addresses are there on the hardware stack?  Discard  \
      the first byte as we are pointing to the next free space. */ \
      MOVFF POSTDEC1, FSR0L            \
      MOVFF POSTDEC1, FSR0L            \
   _endasm                             \
                                       \
   /* Fill the hardware stack from our software stack. */ \
   STKPTR = 0;                         \
                                       \
   while( STKPTR < FSR0L )             \
   {                                   \
      _asm                             \
         PUSH                          \
         MOVF  POSTDEC1, 0, 0          \
         MOVWF TOSU, 0                 \
         MOVF  POSTDEC1, 0, 0          \
         MOVWF TOSH, 0                 \
         MOVF  POSTDEC1, 0, 0          \
         MOVWF TOSL, 0                 \
      _endasm                          \
   }                                   \
                                       \
   _asm                                \
   /* Restore the .tmpdata and MATH_DATA memory. */ \
      MOVFF POSTDEC1, FSR0H            \
      MOVFF POSTDEC1, FSR0L            \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, POSTDEC0         \
      MOVFF POSTDEC1, INDF0            \
   /* Restore the other registers forming the tasks context. */ \
      MOVFF POSTDEC1, PCLATH           \
      MOVFF POSTDEC1, PCLATU           \
      MOVFF POSTDEC1, PRODL            \
      MOVFF POSTDEC1, PRODH            \
      MOVFF POSTDEC1, TBLPTRL          \
      MOVFF POSTDEC1, TBLPTRH          \
      MOVFF POSTDEC1, TBLPTRU          \
      MOVFF POSTDEC1, TABLAT           \
      MOVFF POSTDEC1, FSR0H            \
      MOVFF POSTDEC1, FSR0L            \
      MOVFF POSTDEC1, FSR2H            \
      MOVFF POSTDEC1, FSR2L            \
      MOVFF POSTDEC1, BSR              \
      MOVFF POSTDEC1, STATUS           \
      MOVFF POSTDEC1, WREG             \
   _endasm                             \
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#define posPushShort( value) \
      _asm  \
      _endasm \

#define posPopShort( value) \
      _asm  \
      _endasm \

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#endif // __POSPORT_18CXX_H__
