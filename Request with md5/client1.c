#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

/******************************************************************************************
*
*Run this script with the following command: "gcc client1.c -o client1"
*as the server's response is an automatically generated buffer, type "enter" and 
*the buffer is sent.
*
******************************************************************************************/

void func(int sockfd){
	int n;
	char buff[MAX];

	for(;;) {
		// clean vector
		bzero(buff, sizeof(buff));

		printf("Enter the string : ");
		n = 0;

		// inputting the message
		while ((buff[n++] = getchar()) != '\n');

		// send the message to server
		write(sockfd, buff, sizeof(buff));
		
		// clean vector
		bzero(buff, sizeof(buff));

		// read server message-answer
		read(sockfd, buff, sizeof(buff));
		printf("Message sent: %s\n", buff);
		n = MAX;

		// loop to modify the message (while answer is not 0 or 1)
		while((n != 1) && (n != 0)){
			printf("Do you want to edit the message?[0-no/1-yes]: ");
			n = 0;
			while ((buff[n++] = getchar()) != '\n');
			n = atoi(buff);
		}

		// send answer to server
		write(sockfd, buff, sizeof(buff));

		// if not continue
		if(n == 0){
			break;
		}

		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int main(){
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}


