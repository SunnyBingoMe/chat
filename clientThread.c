
#include	"chat.h"


void	*writeToServer(void *);

static int	dConnection, clientStatus = 1, writeToServerIsDone;
static FILE	*dFile;

void clientThread(FILE *tdFile, int tdConnection)
{ /** this thread read from server/sock, a new thread will write to server */
	char		receiveBuf[MAX_MSG_SIZE+1];
	pthread_t	tid;
	int			tReadByte;

	dConnection = tdConnection;
	dFile = tdFile;

	if(pthread_create(&tid, NULL, writeToServer, NULL) != 0){
		printf("ERR: pthread_create().");
	}

	while ( (tReadByte = read(dConnection, receiveBuf, MAX_MSG_SIZE+1)) > 0){
		/*printf("read byte:%d.\n", tReadByte);*/ //including newline sign.
		receiveBuf[tReadByte] = 0;
		fputs(receiveBuf, stdout);
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

