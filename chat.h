/*
 * =====================================================================================
 *
 *       Filename:  chat.h
 *
 *    Description:  chat sys
 *
 *        Version:  1.0
 *        Created:  02/06/2013 06:08:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BinSun#mail.com (http://blog.SunnyBoy.Me)
 *   Organization:  BTH
 *
 * =====================================================================================
 */
#ifndef __CHAT_H
#define __CHAT_H

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/wait.h>
//#include	<sys/sockio.h>
#include	<pthread.h>

#define MAX_NICK_SIZE 12
#define MAX_MSG_SIZE 255
#define chatPort 9997
#define listenBacklogLength 100

static char chatSysVersion[] = "1.0";

// chat status
#define statusNick		1
#define statusMsg		2




#endif

