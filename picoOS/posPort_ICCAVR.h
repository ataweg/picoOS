#ifndef __POSPORT_ICCAVR_H__
#define __POSPORT_ICCAVR_H__
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
