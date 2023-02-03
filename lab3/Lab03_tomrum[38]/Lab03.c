//Lab03.c
//Tom Ruminski & Bruce Bitwayiki

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int main(int argc, char **argv) {

	//Source info parsed from command line
	char ipSrc[20];
	strcpy(ipSrc, strcat(argv[1],""));
	int portSrc = atoi(argv[2]);

	//Destiny info parse from command line
	char ipDest[20];
	strcpy(ipDest, strcat(argv[3],""));	
	int portDest = atoi(argv[4]);

	//Loss rate parsed from command line (Valid values are 0-1000)
	int lossRate = atoi(argv[5]);
	
	int socket_fd, bind_port, r;
	size_t recv_bytes, send_bytes;
	
	char read_buffer[5000]; //Buffer to relay packets from Source to Destiny
	size_t read_size = sizeof(read_buffer);

	//VLC Source Address
	struct sockaddr_in srcAddr = {0};
    socklen_t srcAddrlen = (sizeof(srcAddr));
	srcAddr.sin_family = PF_INET;
    srcAddr.sin_port = htons(portSrc);
    srcAddr.sin_addr.s_addr = inet_addr(ipSrc);

	//VLC Destiny Address
	struct sockaddr_in destAddr = {0};
    socklen_t destAddrlen = (sizeof(destAddr));
	destAddr.sin_family = PF_INET;
    destAddr.sin_port = htons(portDest);
    destAddr.sin_addr.s_addr = inet_addr(ipDest);

	struct sockaddr_in tempAddr; //Blank struct for recfrom() to put address information into
	socklen_t tempAddrlen = (sizeof(tempAddrlen));

	srand(time(NULL)); //Setting Randomizer Seed
	
	//Create Socket
	socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
    	if (socket_fd == 0)    {
        	perror("socket failed");
        	exit(EXIT_FAILURE);
    	}
	
	//Bind to Port
    	bind_port = bind(socket_fd,(struct sockaddr *)&srcAddr, srcAddrlen);
    	if (bind_port < 0)   {
        	perror("bind failed");
        	exit(EXIT_FAILURE);
    	}

	while(1) { //Keep receiving and sending packets until connection is terminated
		//Receive Data
		recv_bytes = recvfrom(socket_fd, read_buffer, read_size, 0, (struct sockaddr*)&tempAddr, &tempAddrlen);
		if (recv_bytes < 0)   {
			perror("Server read failed");
			exit(EXIT_FAILURE);
	      	}
	
		r = rand() % 1000; //Randomizer to determine whether a packet will be lost. Values from 0-999.
		
		//Send Data
		if(r >= lossRate) {
			send_bytes = sendto(socket_fd, read_buffer, recv_bytes, 0, (struct sockaddr*)&destAddr, destAddrlen);
			if (send_bytes < 0)   {
				perror("Server send failed");
				exit(EXIT_FAILURE);
	      		}
		}
	}
	
	close(socket_fd); //Close Socket
}
