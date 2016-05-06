//#ifdef __MINGW32__
//
//#  include_next <unistd.h>
//
//#else

#ifndef UNISTD_H 
#define UNISTD_H

/* 
This file intended to serve as a drop-in replacement for unistd.h on Windows. 
Please add/remove functionality as neeeded.

See http://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html.
*/ 

#include <io.h> 
#include <direct.h>
#include <process.h>
//#include <getopt.h> // getopt from: http://www.pwilson.net/sample.html.
// To get Sleep().
#include <winsock2.h>

//#define srandom srand 
//#define random rand 

//const W_OK = 2; 
//const R_OK = 4; 

//#define access _access 
//#define ftruncate _chsize 

//#define ssize_t int 

__inline int usleep(unsigned int usec)
{
	Sleep(usec/1000);
	return 0;
}

#endif // UNISTD_H

//#endif // __MINGW32__
