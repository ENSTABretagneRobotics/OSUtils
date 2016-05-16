// https://jakash3.wordpress.com/2011/12/23/conio-h-for-linux/

/*
 *  terminal_io.h
 *  Author: Jakash3
 *  Date: 22 Dec 2011
 *  Some conio.h functions for GNU/Linux
 */
#ifndef CONIO_H 
#define CONIO_H
 
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

// Turns terminal line buffering on or off
static __inline__ void terminal_lnbuf(int yn) {
   struct termios oldt, newt;
   tcgetattr(0, &oldt);
   newt = oldt;
   if (!yn) newt.c_lflag &= ~ICANON;
   else newt.c_lflag |= ICANON;
   tcsetattr(0, TCSANOW, &newt);
}
 
// Turns terminal keyboard echo on or off
static __inline__ void terminal_echo(int yn) {
   struct termios oldt, newt;
   tcgetattr(0, &oldt);
   newt = oldt;
   if (!yn) newt.c_lflag &= ~ECHO;
   else newt.c_lflag |= ECHO;
   tcsetattr(0, TCSANOW, &newt);
}
 
// Set cursor position
static __inline__ void gotoxy(int x, int y) { printf("\x1B[%d;%df", y, x); }
 
// Clear terminal screen and set cursor to top left
static __inline__ void clrscr() { printf("\x1B[2J\x1B[0;0f"); }
 
// Get next immediate character input (no echo)
static __inline__ int getch() {
   register int ch;
   terminal_lnbuf(0);
   terminal_echo(0);
   ch = getchar();
   terminal_lnbuf(1);
   terminal_echo(1);
   return ch;
}
 
// Get next immediate character input (with echo)
static __inline__ int getche() {
   register int ch;
   terminal_lnbuf(0);
   ch = getchar();
   terminal_lnbuf(1);
   return ch;
}
 
// Check if a key has been pressed at terminal
static __inline__ int kbhit() {
   register int ret;
   fd_set fds;
   terminal_lnbuf(0);
   terminal_echo(0);
   struct timeval tv;
   tv.tv_sec = 0;
   tv.tv_usec = 0;
   FD_ZERO(&fds);
   FD_SET(0, &fds);
   select(1, &fds, 0, 0, &tv);
   ret = FD_ISSET(0, &fds);
   terminal_lnbuf(1);
   terminal_echo(1);
   return ret;
}

#endif // CONIO_H
