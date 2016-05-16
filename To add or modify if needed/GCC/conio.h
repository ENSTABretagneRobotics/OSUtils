// http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html

#ifndef CONIO_H 
#define CONIO_H

/* 
This file intended to serve as a drop-in replacement for conio.h on Linux. 
Please add/remove functionality as neeeded.
*/ 

#include <sys/select.h>
#include <curses.h> // To get getch(), ungetch(), etc. that are also in Windows conio.h...

// Comptability with curses.h?
// The Turbo C++ kbhit() returns 0xFFFF when the key is hit, not 1?
static __inline__ int kbhit(void)
{
  struct timeval tv;
  fd_set read_fd;

  /* Do not wait at all, not even a microsecond */
  tv.tv_sec=0;
  tv.tv_usec=0;

  /* Must be done first to initialize read_fd */
  FD_ZERO(&read_fd);

  /* Makes select() ask if input is ready:
   * 0 is the file descriptor for stdin    */
  FD_SET(0,&read_fd);

  /* The first parameter is the number of the
   * largest file descriptor to check + 1. */
  if(select(1, &read_fd,NULL, /*No writes*/NULL, /*No exceptions*/&tv) == -1)
    return 0;  /* An error occured */

  /*  read_fd now holds a bit map of files that are
   * readable. We test the entry for the standard
   * input (file 0). */
  
  if(FD_ISSET(0,&read_fd))
    /* Character pending on stdin */
    return 1;

  /* no characters were pending */
  return 0;
}

#endif // CONIO_H
