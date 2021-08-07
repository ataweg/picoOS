#ifndef __POSPORT_ICCAVR_H__
#define __POSPORT_ICCAVR_H__
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

#if defined(__ICCAVR__)  /* This is IAR, not Imagecraft! */
#include "avr2iar.h"

#include <ioavr.h>
#include <intrinsics.h>
#include <pgmspace.h>

#define PRAGMA(x) _Pragma( #x )
#define ISR(vec) PRAGMA( vector=vec ) __interrupt void handler_##vec(void)

#ifdef __ATtiny4313__
// we have a 16 bit stack pointer
typedef unsigned short stackPtrType;
#else
// we have a 8 bit stack pointer
typedef unsigned char stackPtrType;
#endif

#pragma diag_suppress=Pe1053

#else
#error "wrong compiler, require IAR avr compiler"
#endif

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#define posSTACK_GROWTH            ( -1 )

// Critical section management.
#define posENTER_CRITICAL()                                   \
            asm volatile ( "in   __tmp_reg__, __SREG__" ::);   \
            asm volatile ( "cli" ::);                          \
            asm volatile ( "push __tmp_reg__" ::)

#define posEXIT_CRITICAL()                                    \
            asm volatile ( "pop  __tmp_reg__" ::);             \
            asm volatile ( "out  __SREG__, __tmp_reg__" ::)

#define posDISABLE_INTERRUPTS()    asm( "cli" )
#define posENABLE_INTERRUPTS()     asm( "sei" )

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#endif // __POSPORT_ICCAVR_H__
