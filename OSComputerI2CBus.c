/***************************************************************************************************************:')

OSComputerI2CBus.c

Computer I2C port handling (open, close, read, write,...).

Fabrice Le Bars

Created : 2019-01-30

***************************************************************************************************************:)*/

// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSComputerI2CBus.h"
