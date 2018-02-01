/*
Streaming.h - Arduino library for supporting the << streaming operator
Copyright (c) 2010-2012 Mikal Hart.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// Code from http://arduiniana.org/libraries/streaming/
/*
 Modified by Chris claxton, removed macros for Arduino version detection
 Added a few examples from the comments on the source website
 Added a couple of my own functions
*/

#ifndef ARDUINO_STREAMING
#define ARDUINO_STREAMING

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
//#include "WProgram.h"
#endif

#define STREAMING_LIBRARY_VERSION 5

// Generic template
template<class T> 
inline Print &operator <<(Print &stream, T arg) 
{ stream.print(arg); return stream; }

struct _BASED 
{ 
  long val; 
  int base;
  _BASED(long v, int b): val(v), base(b) 
  {}
};

//#if ARDUINO >= 100

struct _BYTE_CODE
{
	byte val;
	_BYTE_CODE(byte v) : val(v)
	{}
};
#define _BYTE(a)    _BYTE_CODE(a)

inline Print &operator <<(Print &obj, const _BYTE_CODE &arg)
{ obj.write(arg.val); return obj; } 

// #else
// #define _BYTE(a)    _BASED(a, BYTE)
// #endif

#define _HEX(a)     _BASED(a, HEX)
#define _DEC(a)     _BASED(a, DEC)
#define _OCT(a)     _BASED(a, OCT)
#define _BIN(a)     _BASED(a, BIN)

// Specialization for class _BASED
// Thanks to Arduino forum user Ben Combee who suggested this 
// clever technique to allow for expressions like
//   Serial << _HEX(a);
inline Print &operator <<(Print &obj, const _BASED &arg)
{ obj.print(arg.val, arg.base); return obj; } 

//#if ARDUINO >= 18
// Specialization for class _FLOAT
// Thanks to Michael Margolis for suggesting a way
// to accommodate Arduino 0018's floating point precision
// feature like this:
//   Serial << _FLOAT(gps_latitude, 6); // 6 digits of precision
struct _FLOAT
{
  double val;
  int digits;
  _FLOAT(double v, int d): val(v), digits(d)
  {}
};

inline Print &operator <<(Print &obj, const _FLOAT &arg)
{ obj.print(arg.val, arg.digits); return obj; }
//#endif

// Specialization for enum _EndLineCode
// Thanks to Arduino forum user Paul V. who suggested this
// clever technique to allow for expressions like
//   Serial << "Hello!" << endl;
enum _EndLineCode { endl };

inline Print &operator <<(Print &obj, _EndLineCode arg) 
{ obj.println(); return obj; }

// Additional code added from comments
// fill output with some char repeated
struct _FILL
{
  char ch;
  int len;
  _FILL(char c, int l): ch(c), len(l)
  {}
};

inline Print &operator <<(Print &obj, const _FILL &arg) { 
  for (int i=0; i< arg.len; i++) obj.write(arg.ch);
  return obj;
}

// Additional code added from comments
// prints a formatted time
struct _TIME
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  _TIME(uint8_t h, uint8_t m, uint8_t s): hour(h), minute(m), second(s)
  {}
};
inline Print &operator <<(Print &obj, const _TIME &arg) { 
  obj.print(((arg.hour<10)?"0":"")); obj.print(int(arg.hour));
  obj.print(((arg.minute<10)?":0":":")); obj.print(int(arg.minute));
  obj.print(((arg.second<10)?":0":":")); obj.print(int(arg.second));
  return obj;
}

// print an array seperated by specified char
// Added by Chris Claxton
struct _INT_ARRAY
{
  int *ptr;
  int len;
  char ch;
  _INT_ARRAY(int *p, int l, char c): ptr(p), len(l), ch(c)
  {}
};

inline Print &operator <<(Print &obj, const _INT_ARRAY &arg) { 
  for (int i=0; i< arg.len; i++) {
    obj.print(*(arg.ptr+i));
    obj.write(arg.ch);
  }
  return obj;
}

// Print  Hex value, zero padded to specified length (max 7 zeros)
// Added by Chris Claxton
struct _HEX_P
{
  long val;
  byte width;
  _HEX_P(long v, byte w): val(v), width(w)
  {}
};
inline Print &operator <<(Print &obj, const _HEX_P &arg) { 
  byte x = 1;
  for (long i=1; i < arg.val; i *= 16 ) {
    if (x > 7) break; //prevent long overflow
    obj.write('0');
    x++;
  }
  obj.print(arg.val, HEX);
  return obj;
}

#endif
