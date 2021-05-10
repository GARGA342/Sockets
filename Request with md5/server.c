#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> // to set types for the md5
#include <openssl/md5.h> // to use the md5
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

/******************************************************************************************
*
*Run this script with the following command: "gcc server.c -o server -lssl -lcrypto"
*as the server's response is an automatically generated buffer, type "enter" and 
*the buffer is sent.
*
******************************************************************************************/

// Function designed for chat between client and server.
void func(int sockfd, int sockfd2){
	char buff[MAX], backup[MAX];
	unsigned char result[MD5_DIGEST_LENGTH], resultbkp[MD5_DIGEST_LENGTH];
	int answer, cont=0;

	// infinite loop for chat
	for (;;) {
		// clean the vectors
		bzero(buff, MAX);
		bzero(result, MAX);

		// read the message from client and copy it in buffer
		read(sockfd, buff, sizeof(buff));

		// backup of buffer
		for(int i=0; i<=MAX; i++){
			backup[i] = buff[i];
		}

		// md5 format
		MD5(buff, strlen(buff), result);

		// print buffer which contains the client contents
		printf("From client: %s\t To client: ", buff);

		// show md5 message to server
		for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
			printf("%02x", result[i]);
		}
		printf("\n");

		// pressed enter to send
		while(getchar() != '\n');

		if(cont == 0){
			// md5 message backup
			for(int i=0; i <= sizeof(result); i++){
				resultbkp[i] = result[i];
			}
			// send backup to client2
			write(sockfd2, backup, sizeof(backup));
			cont++;
		}else if((result != resultbkp) && cont == 1){
			// send modification message to client2
			write(sockfd2, "Essa mensagem foi modificada", 30);
			cont++;
		}else if((result != resultbkp) && cont == 2){
			write(sockfd2, "Essa mensagem foi modificada várias vezes", 45);
			cont++;
		}

		// send message to the client for changes
		write(sockfd, backup, sizeof(backup));
		
		// read the answer
		bzero(buff, MAX);
		read(sockfd, buff, sizeof(buff));
		answer = atoi(buff);

		// if answer is not
		if(answer == 0){
			bzero(buff, MAX);
			buff[0] = '\n';
			write(sockfd2, buff, sizeof(buff));
			break;
		}

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

// Driver function
int main(){
	int sockfd, connfd1, connfd2, len;
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

	// Accept the data packet from client1 and verification
	connfd1 = accept(sockfd, (SA*)&cli, &len);
	if (connfd1 < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client1...\n");

	// Function for chatting between client2 and server
	connfd2 = accept(sockfd, (SA*)&cli, &len);
	if (connfd2 < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client2...\n");

	func(connfd1, connfd2);

	// After chatting close the socket
	close(sockfd);
}