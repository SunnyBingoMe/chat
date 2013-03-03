#include	"chat.h"

void	*preServerThread(void *);
void	*serverThread(int); /* each thread executes this function */

int 					dListen, dConnection;
pthread_t				tidPreServerThread;
struct serverThreadNode *pServerThreadNodeListHead = NULL, *pServerThreadNodeListEnd = NULL;
int 					clientCount = 0, serverThreadNodeListLock = 0;

int main(int argc, char *argv[])
{
	struct sockaddr_in	uServer;

	dListen = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&uServer, sizeof(uServer));
	uServer.sin_family = AF_INET;
	uServer.sin_addr.s_addr = htons(INADDR_ANY);
	uServer.sin_port = htons(chatPort);
	bind(dListen, (struct sockaddr *) &uServer, sizeof(uServer));
	listen(dListen, listenBacklogLength);

	for ( ; ; ) {
		dConnection = accept(dListen, NULL, NULL);
		pthread_create(&tidPreServerThread, NULL, preServerThread, NULL);
	}
}

void *preServerThread(void * ttt){
	serverThread(dConnection);
	return (NULL);
}
