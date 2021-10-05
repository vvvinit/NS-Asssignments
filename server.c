#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "miracl.h"   /* include MIRACL system */
#define MAX 5
#define PORT 8084
#define SA struct sockaddr


big stack_counter;
big last_key_counter;
big last_key;
big negone;

// Function designed for chat between client and server.
void func(int sockfd)
{
	char buff[MAX];
	int n;
	stack_counter = mirvar(-1);
	last_key_counter = mirvar(-1);
	last_key = mirvar(-1);
	negone = mirvar(-1);
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);
		
		// read the message from client and copy it in buffer
		read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		
		FILE *fp, *fp1;
		fp = fopen("server-response.txt", "w+");
		fp1 = fopen("client-request.txt", "rt");


		if(mr_compare(stack_counter,negone)==0){
			printf("Initialised!\n");
			last_key_counter = mirvar(MAX-1);
			innum(last_key,fp1);
			stack_counter = mirvar(MAX-2);
			otnum(stack_counter,fp);
			fclose(fp);
			otnum(stack_counter,stdout);
		}
		else {
			big a,b, transf;
			a=mirvar(0);
			b=mirvar(-1);
			transf=mirvar(-7);
			
			innum(a,fp1);
			fclose(fp1);

			add(last_key,transf,b);
	
			if(mr_compare(a,b)==0){
				printf("Key validated!...requesting next key!");
				last_key_counter = stack_counter;
				add(stack_counter,negone,stack_counter);
				last_key = a;
				otnum(stack_counter,fp);
				fclose(fp);
			}
			else {
				printf("\n-----------\ngot this->");
				otnum(a,stdout);
				printf("last->");
				otnum(last_key,stdout);
				printf("needed this->");
				otnum(b,stdout);
			}
		}
		
		bzero(buff, MAX);
		n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;

		// and send that buffer to client
		write(sockfd, buff, sizeof(buff));

	}
}

// Driver function
int main()
{	
	big a, b, c;
	
	/* base 10, 5000 digits per big */
	miracl *mip=mirsys(10000,10);

	/*Initialize variables*/
	a=mirvar(-1);

	FILE *fp;
	fp = fopen("server-response.txt", "w+");

	otnum(a,fp);
	otnum(a,stdout);

	fclose(fp);

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
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}

