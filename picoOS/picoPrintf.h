#ifndef __PRINTF_H__
#define __PRINTF_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>

typedef struct
{
   char *buf;              /* buffer pointer */
   int   size;             /* size of buffer */
   int   len;              /* characters read or written so far */
   uint8_t  flags;         /* flags, see below */
#define __SSTR 0x0004      /* this is an sprintf/snprintf string */
} pStream_t;

#define EOF (-1)

#define printf          pico_printf
#define snprintf        pico_snprintf
#define sprintf         pico_sprintf
#define vsnprintf       pico_vsnprintf
#define vsprintf        pico_vsprintf

// for reference see https://www.cplusplus.com/reference/cstdio/printf/
int printf( const char * format, ... );
int snprintf( char * str, size_t len, const char * format, ... );
int sprintf( char * str, const char * format, ... );
int vsnprintf( char * str, size_t len, const char * format, va_list args );
int vsprintf( char * str, const char * format, va_list args );

#ifdef __cplusplus
}
#endif

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
#endif   // __PRINTF_H__