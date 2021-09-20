// --------------------------------------------------------------------------
// Copyright (c) 2002, Alexander Popov (sasho@vip.bg)
// Copyright (c) 2002,2004,2005 Joerg Wunsch
// Copyright (c) 2005, Helmut Wallner
// Copyright (c) 2007, Dmitry Xmelkov
// Copyright (c) 2004,2012 Kustaa Nyholm / SpareTimeLabs
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this list
// of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice, this
// list of conditions and the following disclaimer in the documentation and/or other
// materials provided with the distribution.
//
// Neither the name of the Kustaa Nyholm or SpareTimeLabs nor the names of its
// contributors may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
// --------------------------------------------------------------------------

#include <stddef.h>  /* size_t */
#include <stdbool.h>

#include "picoPrintf.h"

// https://www.cplusplus.com/reference/cstdio/putchar/
extern int putchar( int character );

static int vfprintf( pStream_t * stream, const char * format, va_list args );

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

int pPutc( int c, pStream_t *stream )
{
   if( stream->flags & __SSTR )
   {
      if( stream->len < stream->size )
         *stream->buf++ = c;
      stream->len++;
      return c;
   }
   else
   {
      if( putchar( c ) == c )
      {
         stream->len++;
         return c;
      }
      else
         return EOF;
   }
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

static void ui2a( unsigned int num, unsigned int base, bool uc, char * buf )
{
   int n = 0;
   unsigned int d = 1;
   while( num / d >= base )
      d *= base;

   while( d != 0 )
   {
      unsigned int dgt = num / d;
      num %= d;
      d /= base;
      if( n || dgt > 0 || d == 0 )
      {
         *buf++ = dgt + ( dgt < 10 ? '0' : ( uc ? 'A' : 'a' ) - 10 );
         ++n;
      }
   }
   *buf = '\0';
}

static void i2a( int num, char * buf )
{
   if( num < 0 )
   {
      num = -num;
      *buf++ = '-';
   }
   ui2a( ( unsigned int )num, 10, false, buf );
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
// ltoa() see F:\Projects\InternetOfThings\Devices\Wifi-Switch\Firmware\Wifi-Switch\source\libproj\ltoa.c

static int a2d( char ch )
{
   if( ch >= '0' && ch <= '9' )
      return ch - '0';
   else if( ch >= 'a' && ch <= 'f' )
      return ch - 'a' + 10;
   else if( ch >= 'A' && ch <= 'F' )
      return ch - 'A' + 10;
   else
      return -1;
}

static int a2i( char** str, int base )
{
   char* p = *str;
   int num = 0;
   int digit;

   while( ( digit = a2d( *p ) ) >= 0 )
   {
      if( digit > base )
         break;
      p++;
      num = num * base + digit;
   }
   *str = p;

   return num;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
// print leading blanks and/or zeros

static void putchw( pStream_t *stream, int width, bool leading_zeros, char* buf )
{
   char blank_or_zero = leading_zeros ? '0' : ' ';
   char ch;
   char* p = buf;

   while( *p++ && width > 0 )
      width--;

   if( leading_zeros && *buf == '-' )
   {
      pPutc( *buf++, stream );
   }

   while( width-- > 0 )
      pPutc( blank_or_zero, stream );

   while( ( ch = *buf++ ) )
      pPutc( ch, stream );
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
// for reference see https://www.cplusplus.com/reference/cstdio/printf/
// see F:\working\simavr\avr-libc-2.0.0\libc\stdio\printf.c

int printf( const char * format, ... )
{
   pStream_t stream = { 0 };

   va_list args;

   va_start( args, format );
   int ret = vfprintf( &stream, format, args );
   va_end( args );

   return ret;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

// see F:\working\simavr\avr-libc-2.0.0\libc\stdio\snprintf.c

int snprintf( char * str, size_t len, const char * format, ... )
{
   va_list args;
   pStream_t stream;

   stream.flags = __SSTR;
   stream.buf = str;

   if( ( int )len < 0 )
      len = ( unsigned )INT_MAX + 1;
   stream.len = len - 1;

   va_start( args, format );
   int ret = vfprintf( &stream, format, args );
   va_end( args );

   if( stream.size >= 0 )
      str[stream.len < stream.size ? stream.len : stream.size] = '\0';

   return ret;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
// see F:\working\simavr\avr-libc-2.0.0\libc\stdio\sprintf.c

int sprintf( char * str, const char * format, ... )
{
   va_list args;
   pStream_t stream;

   stream.flags = __SSTR;
   stream.buf = str;
   stream.size = INT_MAX;

   va_start( args, format );
   int ret = vfprintf( &stream, format, args );
   va_end( args );

   str[stream.len] = 0;

   return ret;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
// see F:\working\simavr\avr-libc-2.0.0\libc\stdio\vfprintf.c

int vfprintf( pStream_t * stream, const char * _format, va_list args )
{
   char * format = ( char * )_format;
   char buf[12];
   char c;
   bool have_long = false;
   bool upper_case_hex = false;

   while( true )
   {
      c = *format++;
      if( c == '\0' )
         goto leave;
      else if( c != '%' )
      {
         pPutc( c, stream );
      }
      else
      {
         // found %
         // [0] [0-9]+ [l] [d|u|X|x|c|s]

         bool leading_zeros = false;

         c = *format;
         if( c == '0' )
         {
            // leading zeros requested
            leading_zeros = true;
            format++;
         }

         // get width
         int width = a2i( &format, 10 );

         c = *format;
         if( c == 'l' )
         {
            have_long = true;
            format++;
         }

         c = *format++;
         switch( c )
         {
            case 'd':
            {
               int d = ( int )va_arg( args, int );
               i2a( d, buf );
               // print leading blanks and/or zeros
               putchw( stream, width, leading_zeros, buf );
            }
            break;

            case 'u':
            {
               unsigned int u = ( unsigned int )va_arg( args, unsigned int );
               ui2a( u, 10, false, buf );
               // print leading blanks and/or zeros
               putchw( stream, width, leading_zeros, buf );
            }
            break;

            case 'X':
               upper_case_hex = true;
            case 'x':
            {
               unsigned int x = ( unsigned int )va_arg( args, unsigned int );
               ui2a( x, 16, upper_case_hex, buf );
               // print leading blanks and/or zeros
               putchw( stream, width, leading_zeros, buf );
            }
            break;

            case 'c':
            {
               int ch = ( int ) va_arg( args, int );
               pPutc( ch, stream );
            }
            break;

            case 's':
            {
               char * str = ( char * ) va_arg( args, char * );
               putchw( stream, width, false, str );
            }
            break;

            case '%':
            {
               pPutc( '%', stream );
            }
            break;

            case 0:
               goto leave;

            default:
               ;
         }
      }
   }
leave:
   return stream->len;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
// see F:\working\simavr\avr-libc-2.0.0\libc\stdio\vprintf.c

int vprintf( const char *format, va_list args )
{
   pStream_t stream = { 0 };

   return vfprintf( &stream, format, args );
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
// F:\working\simavr\avr-libc-2.0.0\libc\stdio\vsnprintf.c

int vsnprintf( char * str, size_t len, const char * format, va_list args )
{
   pStream_t stream;

   stream.flags = __SSTR;
   stream.buf = str;

   if( ( int )len < 0 )
      len = ( unsigned )INT_MAX + 1;
   stream.size = len - 1;

   int ret = vfprintf( &stream, format, args );

   if( stream.size >= 0 )
      str[ stream.len < stream.size ? stream.len : stream.size ] = 0;

   return ret;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
// F:\working\simavr\avr-libc-2.0.0\libc\stdio\vsprintf.c

int vsprintf( char * str, const char * format, va_list args )
{
   pStream_t stream;

   stream.flags = __SSTR;
   stream.buf = str;
   stream.size = INT_MAX;

   int ret = vfprintf( &stream, format, args );

   str[ stream.len ] = 0;

   return ret;
}

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
