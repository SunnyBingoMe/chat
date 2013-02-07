/*
 * =====================================================================================
 *
 *       Filename:  chatClient.c
 *
 *    Description:  chat system client
 *
 *        Version:  1.0
 *        Created:  02/06/2013 06:12:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BinSun#mail.com (http://blog.SunnyBoy.Me)
 *   Organization:  BTH
 *
 * =====================================================================================
 */

#include "chat.h"

void clientThread(FILE *, int );

int main(int argc, char *argv[])
{
	int					dConnection;
	char				aServerIp[] = "127.000.000.001";
	int					serverPort = chatPort;
	struct sockaddr_in	uServer;
	

	if (argc == 2){
		memcpy(aServerIp, argv[1], strlen(argv[1]) + 1);
	}else if (argc == 3){
		memcpy(aServerIp, argv[1], strlen(argv[1]) + 1);
		serverPort = atoi(argv[2]);
	}

	dConnection = socket(AF_INET, SOCK_STREAM, 0);

	uServer.sin_family = AF_INET;
	uServer.sin_addr.s_addr = inet_addr(aServerIp);
	uServer.sin_port = htons(serverPort);

	printf("connecting to server:%s:%d.\n", aServerIp, serverPort);
	if (connect(dConnection, (struct sockaddr *)&uServer, sizeof(uServer)) == 0){
		printf("connected.\n");
	}else {
		printf("ERR: connect.\n");
	}

	clientThread(stdin, dConnection);

	exit(0);
	return 0;
}

