#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //to use atof function
#include <sys/socket.h>
#include <sys/types.h>
#define PORT 8080
#define MAX 80
#define SA struct sockaddr

/**********************************************************************************
*
*Run this script with the following command: "gcc server.c -lm"
*as the server's response is an automatically generated buffer, type "enter" and 
*the buffer is sent.
*
**********************************************************************************/

// Function designed for chat between client and server.
void func(int sockfd){
	char buff[MAX]; //receives the number to return the elements
	unsigned int c[MAX]; //to save as many elements as possible
	
	// infinite loop for chat
	for (;;) {
		bzero(c, MAX);
		bzero(buff, MAX);
		// read the message from client and copy it in buffer
		read(sockfd, buff, sizeof(buff));
		
		// print buffer which contains the client contents
		printf("\n\nFrom client: %s\t To client : ", buff);
		
		/***************************************** addition *****************************************/
		
		int cont=0, i=0, j=0, l=1; //counters to loop
		float a = atof(buff); //convert the char to float to rounding
		int x=round(a); //rounding
		while (j < x){ //loop to receive the elements 
			printf("%d ", i);
			c[cont++]=i; //add the element to vector
			j ++; //increment the counter

			if(j < x){ //conditional to insert new element			
				printf("%d ", l);
				c[cont++]=l; 
				j ++;
			}
			//increment the counters
			i += l; 
			l += i;
		}

		/*********************************************************************************************/

		// copy server message in the buffer
		while ((getchar()) != '\n') //press enter to send elements to the client
			;

		// and send that buffer to client
		write(sockfd, c, sizeof(c));

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}


// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}


