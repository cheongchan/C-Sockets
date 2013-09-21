#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include "DieWithError.h"
#include "HandleTCPClient.h"

#define MAXPENDING 5

void DieWithError(char *errorMessage);
void HandleTCPClient(int clntSocket);

int main(int argc, char *argv[])
{
	int servSock;
	int clntSock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned short echoServPort;
	unsigned int clntLen;

	if (argc !=2) //test for correct number of arguements
	{
		fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
		exit(1);
	}

	echoServPort = atoi(argv[1]);

	//Create Socket for incoming connections
	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("socket() failed");

	//Construct local address structure
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	echoServAddr.sin_port = htons(echoServPort);

	//bind to the local address
	if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr))<0)
		DieWithError("bind() failed");

	//Mark the socket so it will listen for the incoming connection
	if (listen(servSock, MAXPENDING) < 0)
		DieWithError("listen() failed");

	for (;;) //run forever
	{
		//set the size of the in-out parameter
		clntLen = sizeof(echoClntAddr);

		//wait for client to connect
		if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
				DieWithError("accept() failed");

		//clnt Sock is connected to client!
		
		printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

		HandleTCPClient(clntSock);

	}
	//Not reached
}