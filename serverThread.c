/*
 * =====================================================================================
 *
 *       Filename:  serverThread.c
 *
 *    Description:  chat sys server -> one thread for each client
 *
 *        Version:  1.0
 *        Created:  02/07/2013 04:21:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BinSun#mail.com (http://blog.SunnyBoy.Me)
 *   Organization:  BTH
 *
 * =====================================================================================
 */
#include "chat.h"

int		dConnection;
char	line[MAX_MSG_SIZE + 1];
FILE	*pFileIn, *pFileOut;

void *serverThread(int tdConnection)
{
	pthread_detach(pthread_self());

	dConnection = tdConnection;

	pFileIn = fdopen(dConnection, "r");
	pFileOut = fdopen(dConnection, "w");
	setbuf(pFileIn, NULL);
	setbuf(pFileOut, NULL);

	while (fgets(line, MAX_MSG_SIZE + 1, pFileIn) != NULL){
		fputs(line, pFileOut);
	}

	close(dConnection);
	return(NULL);
}


