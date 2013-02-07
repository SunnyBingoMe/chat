#include	"chat.h"

int				dListen, dConnection;

void	*preServerThread(void);
void	*serverThread(int); /* each thread executes this function */

int main(int argc, char *argv[])
{
	pthread_t		tid;
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
		pthread_create(&tid, NULL, preServerThread, NULL);
	}
}

void *preServerThread(void){
	serverThread(dConnection);
	return (NULL);
}
