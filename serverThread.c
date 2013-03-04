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
extern int 						clientCount, serverThreadNodeListLock;
pthread_t 						tidEchoClientAll;

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
	pThisServerThreadNode -> serverStatus = statusHello;
	pThisServerThreadNode -> pFileIn = fdopen(pThisServerThreadNode -> dConnection, "r");
	pThisServerThreadNode -> pFileOut = fdopen(pThisServerThreadNode -> dConnection, "w");
	setbuf(pThisServerThreadNode -> pFileIn, NULL);
	setbuf(pThisServerThreadNode -> pFileOut, NULL);

	sprintf(pThisServerThreadNode -> line, "HELLO, v%s.\n", chatSysVersion);
	fputs(pThisServerThreadNode -> line, pThisServerThreadNode -> pFileOut);
	pThisServerThreadNode -> serverStatus = statusNick;

	while(1){ // set nick 
		fgets(pThisServerThreadNode -> line, MAX_MSG_SIZE + 1, pThisServerThreadNode -> pFileIn);
		pThisServerThreadNode -> line[strlen(pThisServerThreadNode -> line) - 1] = '\0';
		if(strlen(pThisServerThreadNode -> line) <= MAX_NICK_SIZE){
			strncpy(pThisServerThreadNode -> nick, pThisServerThreadNode -> line, MAX_NICK_SIZE + 1);
			pThisServerThreadNode -> nick[MAX_NICK_SIZE] = '\0';
			pThisServerThreadNode -> serverStatus = statusMsg;
			fputs("OK, nick accepted.\n", pThisServerThreadNode -> pFileOut);
			printf("nick: %s.\n", pThisServerThreadNode -> nick);
			break;
		}
	}
	

	while (fgets(pThisServerThreadNode -> line, MAX_MSG_SIZE + 1, pThisServerThreadNode -> pFileIn) != NULL){
		pthread_create(&tidEchoClientAll, NULL, echoClientAll, pThisServerThreadNode);
	}

	fclose(pThisServerThreadNode -> pFileIn);
	fclose(pThisServerThreadNode -> pFileOut);
	close(pThisServerThreadNode -> dConnection);
	deleteServerThreadNode(pThisServerThreadNode);
	return(NULL);
}

void *echoClientAll(void *pThisServerThreadNode)
{
	pthread_detach(pthread_self());

	pthread_t 					tidEchoClientEach;
	struct serverThreadNode 	*pThisServerThreadNodeToEcho = pServerThreadNodeListHead;
	struct serverThreadPair 	*pServerThreadPair;

	while(1){
		if((pThisServerThreadNodeToEcho != pThisServerThreadNode) && (pThisServerThreadNodeToEcho -> serverStatus == statusMsg)){
			pServerThreadPair = malloc(sizeof(struct serverThreadPair));
			bzero(pServerThreadPair, sizeof(struct serverThreadPair));
			pServerThreadPair -> pFrom = pThisServerThreadNode;
			pServerThreadPair -> pTo = pThisServerThreadNodeToEcho;
			pthread_create(&tidEchoClientEach, NULL, echoClientEach, pServerThreadPair);
		}
		if(pThisServerThreadNodeToEcho -> pNext != NULL){
			pThisServerThreadNodeToEcho = pThisServerThreadNodeToEcho -> pNext;
		}else{
			return(NULL);
		}
	}
}

void *echoClientEach(void * tpServerThreadPair)
{
	pthread_detach(pthread_self());
	struct serverThreadPair *pServerThreadPair = (struct serverThreadPair *) tpServerThreadPair;
	char					outLine[3 + MAX_NICK_SIZE + 2 + MAX_MSG_SIZE + 1];

	sprintf(outLine, "MSG%s: %s", pServerThreadPair -> pFrom -> nick, pServerThreadPair -> pFrom -> line);
	fputs(outLine, pServerThreadPair -> pTo -> pFileOut);
	free(pServerThreadPair);
	return(NULL);
}

void *deleteServerThreadNode(struct serverThreadNode *pThisServerThreadNode)
{
	while(!serverThreadNodeListLock){
		serverThreadNodeListLock = 1;

		clientCount --;
		if(pThisServerThreadNode -> pPrevious != NULL){ // if not head
			pThisServerThreadNode -> pPrevious -> pNext = pThisServerThreadNode -> pNext;
		}else{ // head is dying
			pServerThreadNodeListHead = pThisServerThreadNode -> pNext;
		}
		if(pThisServerThreadNode -> pNext != NULL){ // if not end
			pThisServerThreadNode -> pNext -> pPrevious = pThisServerThreadNode -> pPrevious;
		}else{ // end is dying
			pServerThreadNodeListEnd = pThisServerThreadNode -> pPrevious;
		}

		serverThreadNodeListLock = 0;
		break;
	}
	printf("clientCount:%d.\n",clientCount);
	free(pThisServerThreadNode); // free mem or move to mem pool
	return(NULL);
}

