#include <stdio.h> //printf and fprintf
#include <string.h> // memset()
#include <stdlib.h> //atoi()
#include <sys/socket.h> //for socket connect send and close
#include <arpa/inet.h>
#include <unistd.h>
#include "DieWithError.h"

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage); //error handling function

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in echoServAddr;
	unsigned short echoServPort;
	char *servIP;
	char *echoString;
	char echoBuffer[RCVBUFSIZE];
	unsigned int echoStringLen;
	int bytesRcvd, totalBytesRcvd;

	if ((argc < 3 ) || (argc > 4))
	{
		fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
			argv[0]);
		exit(1);
	}

	servIP = argv[1];
	echoString = argv[2];

	if (argc ==4 )
		echoServPort = atoi(argv[3]);
	else
		echoServPort = 7;

	//Create a reliable stream socket using TCP
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("socket() failed");

	//Construct Server Address Structure
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);
	echoServAddr.sin_port = htons(echoServPort);

	//Establish the connection to the echo server
	if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)))
		DieWithError("connect() failed");

	echoStringLen = strlen(echoString); 

	//send the string to the server
	if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
		DieWithError("send() snet a different number of bytes than expected");

	//Recieve the same string back from the server 
	totalBytesRcvd = 0;
	printf("Revieved : ");
	while (totalBytesRcvd < echoStringLen)		
	{
		//recieve up to the buffer size minus 1 to leave space for the null terminator
		if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE -1, 0)) <= 0)
			DieWithError("recv() failed or connection closed prematurely");

		totalBytesRcvd += bytesRcvd;
		echoBuffer[bytesRcvd] = '\0'; //terminate the string
		printf("%s",echoBuffer);
	}

	printf("\n"); //print a final linefeed

	close(sock);
	exit(0);

}	