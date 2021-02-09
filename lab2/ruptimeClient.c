// ==========================
// Sample code for the client
// ==========================

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
int main(int argc, char ** argv) {
	
  struct sockaddr_in serverAddr;
  int hSock;//Client socket
  int SERVER_PORT = 42069;
  char* SERVER_IP;
  if(argc==2)//make sure enough arguments are passed
  {
	  SERVER_IP = argv[1];
  }
  else
  {
	  printf("Not enough arguments given. Using default IP.\n");
	  SERVER_IP = "127.0.0.1";
  }
  if( (hSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)//Creating the socket, making sure it creates correctly
  {
	  perror("Socket error.\n");
	  return -1;
  }
  
  
  //Add TCP data to serverAddr struct
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(SERVER_PORT);
  if(inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr.s_addr)<=0)//Checks to make sure that IP is a valid address  
  { 
      printf("Invalid address/ Address not supported\n"); 
      return -1; 
  } 
  
  int outputval;
  outputval = connect(hSock,(struct sockaddr *) &serverAddr, sizeof(serverAddr));//Tries to send serverAddr info to socket
  if(outputval < 0)
  {
	  perror("Connection failed");//throws error if server cannot be connected to
	  return -1;
  }
  
  
/* Then client reads from the server */
  char readBuffer[1000];
  //collect data from server and save to readBuffer
  read(hSock, readBuffer, sizeof(readBuffer));
  //print server ip and uptime
  printf("%s%s",SERVER_IP,readBuffer);
  
  
/* close the socket to the client once you're done */
  close(hSock);
  return 0;
}