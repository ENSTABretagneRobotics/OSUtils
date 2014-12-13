// From https://code.google.com/p/madp-win/, http://www.linuxjournal.com/article/5574, 
// http://pubs.opengroup.org/onlinepubs/7908799/xsh/systimes.h.html...

#ifndef _SYS_TIMES_H_
#define _SYS_TIMES_H_

#ifdef _WIN32
//#include <sys/timeb.h>
//#include <sys/types.h>
//#include <winsock2.h>

// Why here?
//static __inline__ gettimeofday(struct timeval* t,void* timezone)
//{       
//	struct _timeb timebuffer;
//	_ftime( &timebuffer );
//	t->tv_sec=timebuffer.time;
//	t->tv_usec=1000*timebuffer.millitm;
//	return 0;
//}

// from linux's sys/times.h

//#include <features.h>

#define __need_clock_t
#include <time.h>

/* Structure describing CPU time used by a process and its children.  */
struct tms
{
	clock_t tms_utime;          /* User CPU time.  */
	clock_t tms_stime;          /* System CPU time.  */

	clock_t tms_cutime;         /* User CPU time of dead children.  */
	clock_t tms_cstime;         /* System CPU time of dead children.  */
};

/* Store the CPU time used by this process and all its
   dead children (and their dead children) in BUFFER.
   Return the elapsed real time, or (clock_t) -1 for errors.
   All times are in CLK_TCKths of a second.  */
#ifdef __GNUC__
static __inline__ clock_t times (struct tms *__buffer) 
#else
__inline clock_t times (struct tms *__buffer) 
#endif // __GNUC__
{
	__buffer->tms_utime = clock();
	__buffer->tms_stime = 0;
	__buffer->tms_cstime = 0;
	__buffer->tms_cutime = 0;
	return __buffer->tms_utime;
}

#ifndef DISABLE_SYSCONF__SC_CLK_TCK_DEFINITION
// sysconf(_SC_CLK_TCK) might also be missing, so an implementation is proposed here...
// From http://pubs.opengroup.org/onlinepubs/7908799/xsh/time.h.html, 
// http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysconf.html, 
// http://stackoverflow.com/questions/12475028/no-definition-of-sc-clk-tck...
#ifndef _SC_CLK_TCK
#define _SC_CLK_TCK 2
#ifdef __GNUC__
static __inline__ long int sysconf(int name)
#else
__inline long int sysconf(int name) 
#endif // __GNUC__
{
	long int res = 0;

	switch (name)
	{
	case _SC_CLK_TCK : 
		res = CLK_TCK;
		break;
	default :
		break;
	}

	return res;
}
#endif // _SC_CLK_TCK
#endif // DISABLE_SYSCONF__SC_CLK_TCK_DEFINITION

// Why here?
//typedef long long suseconds_t ;

#endif  // _WIN32

#endif // _SYS_TIMES_H_
