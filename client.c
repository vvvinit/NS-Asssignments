#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "miracl.h"
#include "constants.h"
#define SA struct sockaddr

big keys_stack[STACK_SIZE];
big NEGATIVE_ONE;

// Implementation of Lamport OTP scheme (client side)
void lamport(int sockfd)
{
	char buff[BUFFER_SIZE];
	int n;
	keys_stack[0]=mirvar(0);

	// Initializes internal random number system.
	long seed = 23422;
	irand(seed);

	// Generate a big random number (Base OTP).
	keys_stack[0] = mirvar(0);
	bigdig(10,10,keys_stack[0]);

	// Loop for server-client connection
	for (;;) {
		FILE *server_file, *client_file;
		server_file = fopen("server-response.txt", "rt");
		client_file = fopen("client-response.txt", "w+");
		bzero(buff, sizeof(buff));

		big requested_OTP_index, transf;
		requested_OTP_index=mirvar(0);
		transf=mirvar(7);
		int requested_counter;

		// Get requested OTP's index from server
		fscanf(server_file,"%d",&requested_counter); 
		requested_OTP_index = mirvar(requested_counter);
		fclose(server_file);

		// If requested OTP index = -1, stack is empty. Hence, we have to generate a new OTPs stack.
		if(mr_compare(requested_OTP_index,NEGATIVE_ONE)==0){
			printf("Server requesting for a base OTP. Press return key to create an OTPs stack!\n");
			getchar();
			printf("Initialising!\n");
			int i;
	
			bigdig(10,10,keys_stack[0]);	// Create a new random number as seed (base OTP).
			
			// Calculate remanining OTPs. 
			for(i = 1; i < STACK_SIZE; i++){
				keys_stack[i] = mirvar(0);
				add(keys_stack[i-1],transf,keys_stack[i]);
			}

			// Save all the OTPs in keys-stack.txt file for simple manual verification.
			FILE *OTPs_list_file;
			OTPs_list_file = fopen("keys-stack.txt", "w+");
			for(i = 0; i < STACK_SIZE; i++){
				otnum(keys_stack[i],OTPs_list_file);
			}
			fclose(OTPs_list_file);
			printf("OTPs stack generated. It can be cross verified through keys-stack.txt.\nNew base OTP: ");
			otnum(keys_stack[0],stdout);			
			
			// Send base OTP to server!
			otnum(keys_stack[STACK_SIZE-1],client_file);
			fclose(client_file);
		}
		else {
			// Send requested OTP to server
			printf("Server requesting %dth OTP:", requested_counter);
			otnum(keys_stack[requested_counter],client_file);
			otnum(keys_stack[requested_counter],stdout);
			fclose(client_file);
		}
		
		n = 0;
		printf("Press return key to send!");
        while ((buff[n++] = getchar()) != '\n')
            ;
		printf("OTP sent! Go to server to request next OTP :)\n\n", requested_counter);
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
	}
}

int main()
{	
	miracl *mip=mirsys(10000,10);
	// Initialisation
	NEGATIVE_ONE = mirvar(-1);

	// Following lines of code deal with socket creation and binding that facilitate client-server communication.
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1) {
		printf("Socket creation failed! :(\n");
		exit(0);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Cannot connect with server :(\n");
		exit(0);
	}
	else
		printf("Connected to server :)\n");

	// Lamport OTP function
	lamport(sockfd);

	close(sockfd);
	return 0;
}

