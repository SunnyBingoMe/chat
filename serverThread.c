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

void *echoClientAll(void *);
void *echoClientEach(void *);

int					dConnection, clientCount = 0;
char				line[MAX_MSG_SIZE + 1];
struct clientNode	*pClientNodeListHead = NULL, *pClientNodeListEnd = NULL;
pthread_t			tidEchoClientAll;

void *serverThread(int tdConnection)
{
	pthread_detach(pthread_self());
	struct clientNode *tpClientNode = NULL;
	clientCount ++;

	tpClientNode = malloc(sizeof(struct clientNode));
	bzero(tpClientNode, sizeof(struct clientNode));

	if(clientCount == 1){
		pClientNodeListHead = pClientNodeListEnd = tpClientNode;
	}else{
		tpClientNode -> pPrevious = pClientNodeListEnd;
		pClientNodeListEnd -> pNext = tpClientNode;
		pClientNodeListEnd = tpClientNode;
	}

	pClientNodeListEnd -> dConnection = tdConnection;
	pClientNodeListEnd -> pFileIn = fdopen(dConnection, "r");
	pClientNodeListEnd -> pFileOut = fdopen(dConnection, "w");
	setbuf(pClientNodeListEnd -> pFileIn, NULL);
	setbuf(pClientNodeListEnd -> pFileOut, NULL);

	while (fgets(line, MAX_MSG_SIZE + 1, pClientNodeListEnd -> pFileIn) != NULL){
		pthread_create(&tidEchoClientAll, NULL, echoClientAll, NULL);
	}

	close(dConnection);
	return(NULL);
}

void *echoClientAll(void * ttt){
	pthread_detach(pthread_self());
	pthread_t	tidEchoClientEach;
	struct		clientNode *tpClientNodeToEcho = pClientNodeListHead;

	while(1){
		pthread_create(&tidEchoClientEach, NULL, echoClientEach, tpClientNodeToEcho -> pFileOut);
		if(tpClientNodeToEcho -> pNext != NULL){
			tpClientNodeToEcho = tpClientNodeToEcho -> pNext;
		}else{
			return(NULL);
		}
	}
}

void *echoClientEach(void * pFileOut){
	pthread_detach(pthread_self());
	fputs(line, pFileOut);
	return(NULL);
}

