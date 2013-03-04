
#include	"chat.h"


void	*writeToServer(void *);

int 	dConnection, clientStatus = statusHello, writeToServerIsDone;
FILE	*dFile; // stdin

void clientThread(FILE *tdFile, int tdConnection)
{ /** this thread read from server/sock, a new thread will write to server */
	char		receiveBuf[MAX_MSG_SIZE + 1];
	pthread_t	tid;
	int 		tReadByte;

	dConnection = tdConnection;
	dFile = tdFile;

	if(pthread_create(&tid, NULL, writeToServer, NULL) != 0){
		printf("ERR: pthread_create().\n");
	}

	while ( (tReadByte = read(dConnection, receiveBuf, MAX_MSG_SIZE+1)) > 0){
		/*printf("read byte:%d.\n", tReadByte);*/ //including newline sign.
		receiveBuf[tReadByte] = 0;
		if(strncmp("HELLO", receiveBuf, 5) == 0){
			if(clientStatus == statusHello){
				clientStatus = statusNick;
				printf("what's your nick name?\n");
			}else{
				printf("ERR: unexpected 'HELLO' in status %d.\n", clientStatus);
			}
		}else if(strncmp("OK", receiveBuf, 2) == 0){
			if(clientStatus == statusNick){
				clientStatus = statusMsg;
			}else{
				printf("ERR: unexpected 'OK' in status %d.\n", clientStatus);
			}
		}else if(strncmp("MSG", receiveBuf, 3) == 0){
			if(clientStatus != statusMsg){
				printf("ERR: unexpected 'MSG' in status %d.\n", clientStatus);
			}
			receiveBuf[strlen(receiveBuf) - 1] = '\0'; // delete  newline sign
			printf("%c[%d;%dm%s%c[%dm\n",27,1,33, receiveBuf + 3, 27,0); // colored text:  http://goo.gl/bVXtD
		}else if(strncmp("ERROR", receiveBuf, 5) == 0){
			if(clientStatus != statusHello){
				printf("ERR: unexpected 'ERROR' in status %d.\n", clientStatus);
			}
		}
	}

	if (writeToServerIsDone == 0){
		printf("server terminated actively.\n");
		exit(2);
	}
}

void * writeToServer(void *arg)
{
	char	aMsg[MAX_MSG_SIZE + 1];

	while (fgets(aMsg, MAX_MSG_SIZE + 1, dFile) != NULL){
		write(dConnection, aMsg, strlen(aMsg));
	}

	shutdown(dConnection, SHUT_WR);	/* EOF on stdin, send FIN */

	writeToServerIsDone = 1;
	return(NULL);
	/* return (i.e., thread terminates) when end-of-file on stdin */
}

