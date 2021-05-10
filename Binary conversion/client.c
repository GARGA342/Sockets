#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //to use atoi function
#include <sys/socket.h>
#define PORT 8080
#define MAX 80
#define SA struct sockaddr

/**********************************************************************************
*
*Run this script with the following command: "gcc server.c -lm"
*this program returns the required elements of the fibonacci sequence. 
*Just insert the number (preferably Integer) and then enter it.
*
**********************************************************************************/


void func(int sockfd){
	unsigned int buff[MAX]; //to store all numbers in the sequence
	char c[4]; //to save the number of elements in the sequence
	int n; //counter
	for (;;) {
		bzero(c, sizeof(c));
		printf("\nEnter the number : ");
		n = 0; //counter initialized
		while ((c[n++] = getchar()) != '\n') //scroll through the number entered
			;
			
		n = atoi(c); //convert to integer to enter the loop
		
		write(sockfd, c, sizeof(c));
		bzero(c, sizeof(c));
		
		read(sockfd, buff, sizeof(buff));
		printf("From Server : ");
		
		//in the next step the buffer will be traversed to show the sequence in order
		
		/***************************************** addition *****************************************/
		
		for(int i=0; i<n; i++){ //go through the elements of the sequence
			if(i == n-1){ //to show the last element
				printf("%d\n", buff[i]);
			}else{
				printf("%d, ", buff[i]); //to middle elements
			}	
		}

		/*********************************************************************************/
				
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int main()
{
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


