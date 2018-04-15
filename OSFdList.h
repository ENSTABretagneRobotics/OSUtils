/***************************************************************************************************************:')

OSFdList.h

Linked list associated with a fd_set.

Fabrice Le Bars

Created : 2007

***************************************************************************************************************:)*/

// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef OSFDLIST_H
#define OSFDLIST_H

#include "OSNet.h"

/*
Debug macros specific to OSFdList.
*/
#ifdef _DEBUG_MESSAGES_OSUTILS
#	define _DEBUG_MESSAGES_OSFDLIST
#endif // _DEBUG_MESSAGES_OSUTILS

#ifdef _DEBUG_WARNINGS_OSUTILS
#	define _DEBUG_WARNINGS_OSFDLIST
#endif // _DEBUG_WARNINGS_OSUTILS

#ifdef _DEBUG_ERRORS_OSUTILS
#	define _DEBUG_ERRORS_OSFDLIST
#endif // _DEBUG_ERRORS_OSUTILS

#ifdef _DEBUG_MESSAGES_OSFDLIST
#	define PRINT_DEBUG_MESSAGE_OSFDLIST(params) PRINT_DEBUG_MESSAGE(params)
#else
#	define PRINT_DEBUG_MESSAGE_OSFDLIST(params)
#endif // _DEBUG_MESSAGES_OSFDLIST

#ifdef _DEBUG_WARNINGS_OSFDLIST
#	define PRINT_DEBUG_WARNING_OSFDLIST(params) PRINT_DEBUG_WARNING(params)
#else
#	define PRINT_DEBUG_WARNING_OSFDLIST(params)
#endif // _DEBUG_WARNINGS_OSFDLIST

#ifdef _DEBUG_ERRORS_OSFDLIST
#	define PRINT_DEBUG_ERROR_OSFDLIST(params) PRINT_DEBUG_ERROR(params)
#else
#	define PRINT_DEBUG_ERROR_OSFDLIST(params)
#endif // _DEBUG_ERRORS_OSFDLIST

#ifdef _MSC_VER
// Disable some Visual Studio warnings that happen in some Winsock macros
// related to fd_set.
#pragma warning(disable : 4127) 
#endif // _MSC_VER

// Element of a fd_list.
struct _SOCKET_DATA	
{
	SOCKET sock;
	long LastUsedTime;
	struct _SOCKET_DATA* prev; // Previous element in the fd_list.
	struct _SOCKET_DATA* next; // Next element in the fd_list.
};
typedef struct _SOCKET_DATA SOCKET_DATA;

// Linked list associated with a fd_set. 
// All the sockets in the fd_set should be in the fd_list.
struct _fd_list	
{
	SOCKET_DATA* first; // First element of the list.
	u_int fd_count; // Number of elements in the list.
	SOCKET max_socket; // Max socket number, used by select().
};
typedef struct _fd_list fd_list;

/*
Initialize the first list element with the server socket, add the socket to the socket set
and initialize sock_list->max_socket.
*/
inline int FD_INIT(SOCKET sock_server, fd_list* sock_list, fd_set* sock_set)	
{
#ifdef _WIN32
	if (sock_list->fd_count >= FD_SETSIZE)	
	{
		return EXIT_FAILURE;
	}
#else
	if (sock_list->max_socket+1 >= FD_SETSIZE)	
	{
		return EXIT_FAILURE;
	}
#endif

	// Initialize the first element of the SOCKET_DATA list with the server socket.
	sock_list->first = (SOCKET_DATA*)calloc(1, sizeof(SOCKET_DATA));

	if (sock_list->first == NULL)
	{
		PRINT_DEBUG_ERROR_OSFDLIST(("FD_INIT error (%s) : %s"
			"(sock_server=%d, sock_list=%#x, sock_set=%#x)\n", 
			strtime_m(), 
			szOSUtilsErrMsgs[EXIT_OUT_OF_MEMORY], 
			(int)sock_server, sock_list, sock_set));
		return EXIT_OUT_OF_MEMORY;
	}

	// Initialize the fd_set and add the server socket to it.
	FD_ZERO(sock_set); 
	FD_SET(sock_server, sock_set);

	sock_list->first->sock = sock_server;
	sock_list->first->LastUsedTime = GetTickCount();
	sock_list->first->prev = NULL;
	sock_list->first->next = NULL;

	// Update the number of sockets in the set.
	sock_list->fd_count = 1;

	// Max socket number, used by select().
	sock_list->max_socket = (u_int)sock_server;

	return EXIT_SUCCESS;
}

inline int FD_CLEAR(fd_list* sock_list, fd_set* sock_set)	
{
	SOCKET_DATA* sd_tmp = NULL;
	SOCKET_DATA* sd = sock_list->first; 

	sock_list->max_socket = 0;
	sock_list->fd_count = 0;

	while (sd)	
	{
		sd_tmp = sd->next;

		// Remove the socket from the list.
		if (sd->prev != NULL)	
		{
			sd->prev->next = sd->next;
		} 
		if (sd->next != NULL)	
		{
			sd->next->prev = sd->prev;
		} 

		// Remove the socket from the set.
		FD_CLR(sd->sock, sock_set); 

		free(sd);

		sd = sd_tmp;
	}

	return EXIT_SUCCESS;
}

/* 
Create a new SOCKET_DATA based on new_sock, add it to the end of the list,
add the socket to the socket set watched by select, increases sock_list->max_socket if needed.
*/
inline int FD_ADD(SOCKET new_sock, fd_list* sock_list, fd_set* sock_set)	
{
	SOCKET_DATA* new_sd = NULL;
	SOCKET_DATA* sd = NULL;

#ifdef _WIN32
	if (sock_list->fd_count >= FD_SETSIZE)	
	{
		return EXIT_FAILURE;
	}
#else
	if (sock_list->max_socket+1 >= FD_SETSIZE)	
	{
		return EXIT_FAILURE;
	}
#endif

	// Create a new SOCKET_DATA structure that will contain the new socket.
	new_sd = (SOCKET_DATA*)calloc(1, sizeof(SOCKET_DATA));

	if (new_sd == NULL)
	{
		PRINT_DEBUG_ERROR_OSFDLIST(("FD_ADD error (%s) : %s"
			"(new_sock=%d, sock_list=%#x, sock_set=%#x)\n", 
			strtime_m(), 
			szOSUtilsErrMsgs[EXIT_OUT_OF_MEMORY], 
			(int)new_sock, sock_list, sock_set));
		return EXIT_OUT_OF_MEMORY;
	}

	// Add the new socket to the socket set.
	FD_SET(new_sock, sock_set); 

	// Initialize it with the new socket.
	new_sd->sock = new_sock;
	new_sd->LastUsedTime = GetTickCount();


	//// Add it to the beginning of the list.
	//new_sd->prev = NULL;
	//new_sd->next = sock_list->first; 
	//sock_list->first->prev = new_sd;
	//sock_list->first = new_sd;


	// Look for the last element of the list.
	if (sock_list->first == NULL)
	{
		new_sd->prev = NULL;
		new_sd->next = NULL;
		sock_list->first = new_sd;
	}
	else
	{
		sd = sock_list->first;
		while (sd->next)	
		{
			sd = sd->next;
		} // sd is now the last element.

		// Add the new element at the end of the list.
		new_sd->prev = sd;
		new_sd->next = NULL;
		sd->next = new_sd;
	}

	// Update the number of sockets in the set.
	sock_list->fd_count++;

	// Increase max_socket if needed.
	sock_list->max_socket = (u_int)max(sock_list->max_socket, new_sock);

	return EXIT_SUCCESS;
}

/* 
Remove sd from the list, decrease sock_list->max_socket, remove sd from the socket
set watched by select(), free sd.
*/
inline int FD_REMOVE(SOCKET_DATA* sd, fd_list* sock_list, fd_set* sock_set)	
{
	SOCKET_DATA* sd_tmp = NULL;

	// Decrease sock_list->max_socket if needed.
	if (sd->sock == sock_list->max_socket)
	{ 
		// Look for the new max in the list.
		sd_tmp = sock_list->first;
		sock_list->max_socket = 0;
		while (sd_tmp)	
		{
			if (sd_tmp != sd)
				sock_list->max_socket = (u_int)max(sock_list->max_socket, sd_tmp->sock);
			sd_tmp = sd_tmp->next;
		} 
	}

	// Update the number of sockets in the set.
	sock_list->fd_count--;

	if (sd == sock_list->first)
	{
		// Remove the socket from the list.
		//if (sd->prev != NULL)	
		//{
		//	sd->prev->next = sd->next;
		//} 
		if (sd->next != NULL)	
		{
			sd->next->prev = NULL;
		} 
		sock_list->first = sd->next;
	}
	else
	{
		// Remove the socket from the list.
		if (sd->prev != NULL)	
		{
			sd->prev->next = sd->next;
		} 
		if (sd->next != NULL)	
		{
			sd->next->prev = sd->prev;
		} 
	}

	// Remove the socket from the set.
	FD_CLR(sd->sock, sock_set); 

	free(sd); sd = NULL;

	return EXIT_SUCCESS;
}

#ifdef _MSC_VER
// Restore the Visual Studio warnings previously disabled for Winsock macros
// related to fd_set.
#pragma warning(default : 4127) 
#endif // _MSC_VER

#endif // !OSFDLIST_H
