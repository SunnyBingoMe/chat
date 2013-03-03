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
void *deleteServerThreadNode(struct serverThreadNode *);

extern struct serverThreadNode	*pServerThreadNodeListHead, *pServerThreadNodeListEnd;
extern int clientCount, serverThreadNodeListLock;
char						line[MAX_MSG_SIZE + 1];
pthread_t					tidEchoClientAll;

void *serverThread(int tdConnection) // after accept, one "serverThread" for each client
{
	pthread_detach(pthread_self());
	struct serverThreadNode		*pThisServerThreadNode = NULL;

	clientCount ++;
	printf("clientCount:%d.\n",clientCount);

	pThisServerThreadNode = malloc(sizeof(struct serverThreadNode));
	bzero(pThisServerThreadNode, sizeof(struct serverThreadNode));

	while(!serverThreadNodeListLock){
		serverThreadNodeListLock = 1;

		if(clientCount == 1){
			pServerThreadNodeListHead = pServerThreadNodeListEnd = pThisServerThreadNode;
		}else{
			pThisServerThreadNode -> pPrevious = pServerThreadNodeListEnd;
			pServerThreadNodeListEnd -> pNext = pThisServerThreadNode;
			pServerThreadNodeListEnd = pThisServerThreadNode;
		}

		serverThreadNodeListLock = 0;
		break;
	}
	pThisServerThreadNode -> dConnection = tdConnection;
	pThisServerThreadNode -> pFileIn = fdopen(pThisServerThreadNode -> dConnection, "r");
	pThisServerThreadNode -> pFileOut = fdopen(pThisServerThreadNode -> dConnection, "w");
	setbuf(pThisServerThreadNode -> pFileIn, NULL);
	setbuf(pThisServerThreadNode -> pFileOut, NULL);

	while (fgets(line, MAX_MSG_SIZE + 1, pThisServerThreadNode -> pFileIn) != NULL){
		pthread_create(&tidEchoClientAll, NULL, echoClientAll, pThisServerThreadNode);
	}

	close(pThisServerThreadNode -> dConnection);
	deleteServerThreadNode(pThisServerThreadNode);
	return(NULL);
}

void *echoClientAll(void *pThisServerThreadNode)
{
	pthread_detach(pthread_self());

	pthread_t 				tidEchoClientEach;
	struct serverThreadNode	*pThisServerThreadNodeToEcho = pServerThreadNodeListHead;

	while(1){
		if(pThisServerThreadNodeToEcho != pThisServerThreadNode){
			pthread_create(&tidEchoClientEach, NULL, echoClientEach, pThisServerThreadNodeToEcho -> pFileOut);
		}
		if(pThisServerThreadNodeToEcho -> pNext != NULL){
			pThisServerThreadNodeToEcho = pThisServerThreadNodeToEcho -> pNext;
		}else{
			return(NULL);
		}
	}
}

void *echoClientEach(void * pFileOut)
{
	pthread_detach(pthread_self());
	fputs(line, pFileOut);
	return(NULL);
}

void *deleteServerThreadNode(struct serverThreadNode *pThisServerThreadNode)
{
	while(!serverThreadNodeListLock){
		serverThreadNodeListLock = 1;

		if(pThisServerThreadNode -> pPrevious != NULL){ // if not head
			pThisServerThreadNode -> pPrevious -> pNext = pThisServerThreadNode -> pNext;
		}
		if(pThisServerThreadNode -> pNext != NULL){ // if not end
			pThisServerThreadNode -> pNext -> pPrevious = pThisServerThreadNode -> pPrevious;
		}

		serverThreadNodeListLock = 0;
		break;
	}
	clientCount --;
	printf("clientCount:%d.\n",clientCount);
	//free mem or move to mem pool
	return(NULL);
}

