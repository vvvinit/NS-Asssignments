/**
	@file   server.c
	@author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
*/

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "miracl.h"
#include "constants.h"
#define SA struct sockaddr


big stack_index;
big last_OTP_index;
big last_OTP;
big NEGATIVE_ONE;

// Implementation of Lamport OTP scheme (server side)
void lamport(int sockfd)
{	
	printf("Requesting client for a base OTP!\n\n");
	char buff[BUFFER_SIZE];
	int n;
	stack_index = mirvar(-1);
	last_OTP_index = mirvar(-1);
	last_OTP = mirvar(-1);

	// Loop for server-client connection
	for (;;) {

		bzero(buff, BUFFER_SIZE);
		read(sockfd, buff, sizeof(buff));
		
		FILE *server_file, *client_file;
		server_file = fopen("server-response.txt", "w+");
		client_file = fopen("client-response.txt", "rt");

		// If OTP stack is empty, we have received a new base OTP!
		if(mr_compare(stack_index,NEGATIVE_ONE)==0){
			printf("Initialised!\nReceived base OTP: ");
			last_OTP_index = mirvar(STACK_SIZE-1);	// Set last_OTP_index to (stack_size - 1)
			innum(last_OTP,client_file);			// Get base OTP from client
			stack_index = mirvar(STACK_SIZE-2);		// Set stack index to (stack size - 1)
			otnum(stack_index,server_file);			// Request for next OTP
			fclose(server_file);
			otnum(last_OTP,stdout);
		}
		else {
			big received_otp,expected_otp, transf;
			received_otp=mirvar(0);
			expected_otp=mirvar(0);
			transf=mirvar(-7);
			
			innum(received_otp,client_file);	// Get OTP from client
			fclose(client_file);

			add(last_OTP,transf,expected_otp);	// Calculate what the received OTP should be

			// If received OTP is correct, validate it. Else, state that it is invalid!
			if(mr_compare(received_otp,expected_otp)==0){
				printf("OTP index: ");
				otnum(stack_index,stdout);
				printf("Received OTP: ");
				otnum(received_otp,stdout);
				printf("Expected OTP: ");
				otnum(expected_otp,stdout);
				printf("OTP validated!\n");
				last_OTP_index = stack_index;
				add(stack_index,NEGATIVE_ONE,stack_index);
				last_OTP = received_otp;
				otnum(stack_index,server_file);
				fclose(server_file);
			}
			else {
				printf("Invalid OTP! :(\nReceived: ");
				otnum(received_otp,stdout);
				printf("Required: ");
				otnum(expected_otp,stdout);
			}
		}
		
		bzero(buff, BUFFER_SIZE);
		
		// Check if the OTP stack is empty. If yes, then request for a new base OTP. Else, request for the next OTP.
		if(mr_compare(stack_index,NEGATIVE_ONE)==0)
			printf("OTP stack empty. Press return to request client for new base OTP!\n");
		else
			printf("Press return key to request next OTP!\n");
		n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
		if(mr_compare(stack_index,NEGATIVE_ONE)==0)
			printf("New base OTP requested!\n");
		else
			printf("Next OTP requested!\n\n");

		write(sockfd, buff, sizeof(buff));
	}
}

// Driver function
int main()
{	
	miracl *mip=mirsys(10000,10);
	// Initialisation
	NEGATIVE_ONE = mirvar(-1);
	
	// Send -1 as response to denote that the OTP stack has not yet been initialised.
	FILE *server_file;
	server_file = fopen("server-response.txt", "w+");
	otnum(NEGATIVE_ONE,server_file);
	fclose(server_file);

	// Following lines of code deal with socket creation and binding that facilitate client-server communication.
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1) {
		printf("Socket creation failed! :(\n");
		exit(0);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("Socket bind failed! Please free the socket or change port number in constants.h :)\n");
		exit(0);
	}

	if ((listen(sockfd, 5)) != 0) {
		printf("Error! Server cannot listen :(\n");
		exit(0);
	}
	else
		printf("Waiting for client to connect! :)\n");
	
	len = sizeof(cli);
	connfd = accept(sockfd, (SA*)&cli, &len);
	
	if (connfd < 0) {
		printf("Error! Client could not be connected :(");
		exit(0);
	}
	else
		printf("Client connected :)\n");

	// Lamport OTP function
	lamport(connfd);

	close(sockfd);
	return 0;
}

