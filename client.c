#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "miracl.h"   /* include MIRACL system */
#define MAX 5
#define PORT 8084
#define SA struct sockaddr

big keys_stack[MAX];

void func(int sockfd)
{
	char buff[MAX];
	int n;


	keys_stack[0]=mirvar(0);

	/*Initializes internal random number system. */
	long seed = 23422;
	irand(seed);

	getchar();

	/*Generate two big random numbers a and b in base 10, each consisting pf 150 digits */
	// bigdig(10,10,a);
	keys_stack[0] = mirvar(0);
	bigdig(10,10,keys_stack[0]);



	for (;;) {
		printf("Initialised!3\n");
		FILE *fp, *fp1;
		fp = fopen("server-response.txt", "rt");
		fp1 = fopen("client-request.txt", "w+");
		bzero(buff, sizeof(buff));

		big a,b, transf;
		a=mirvar(0);
		b=mirvar(-1);
		transf=mirvar(7);
		int requested_counter;
		fscanf(fp,"%d",&requested_counter); 
		a = mirvar(requested_counter);
		
		fclose(fp);

		if(mr_compare(a,b)==0){
			printf("Initialising! New base key: ");
			otnum(keys_stack[0],stdout);
			printf("Calculating keys stack...");
			int i;
			for(i = 1; i < MAX; i++){
				keys_stack[i] = mirvar(0);
				add(keys_stack[i-1],transf,keys_stack[i]);
			}
			FILE *fp3;
			fp3 = fopen("keys-stack.txt", "w+");
			for(i = 0; i < MAX; i++){
				otnum(keys_stack[i],fp3);
			}
			fclose(fp3);

			otnum(keys_stack[MAX-1],fp1);
			fclose(fp1);
			printf("Initialised!\n");
		}
		else {
			printf("next - %d\n", requested_counter);
			otnum(keys_stack[requested_counter],fp1);
			fclose(fp1);
		}
		
		n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		printf("Initialised1!\n");
		read(sockfd, buff, sizeof(buff));
		printf("Initialised!2\n");

		
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

	big a, b, c;
	
	/* base 10, 5000 digits per big */
	miracl *mip=mirsys(10000,10);

	

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}

