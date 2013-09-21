#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "DieWithError.h"

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);

void HandleTCPClient(int clntSocket)
{
	char echoBuffer[RCVBUFSIZE];
	int recvMsgSize;

	//Recieve message from client
	if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
		DieWithError("recv() failed");
	
	// Send recieved string and recieve again until the end of the transmission
	while (recvMsgSize > 0)
	{
		//Echo Message Back to client
		if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
			DieWithError("send() failed");

		//See if there is a more data to recieve
		if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
			DieWithError("recv() failed");

	}
	
	//close client socket
	close(clntSocket);	

}