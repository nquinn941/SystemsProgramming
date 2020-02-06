/*
Client side of a client server connection 

Name: client.c 

Written by: Niall Quinn - April 2019

Purpose: Connect to server, send a filename and recieve input from server and write to file 

usage: ./serverRun & ./clientRun filename

Description of parameters: argv[1] - filename to be passed 

Subroutines/includes: see below

*/

#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <fcntl.h> 

#define MAX 1024 
#define PORT 53090 
#define SA struct sockaddr 

void writeToServer(int socketFD,char* filename);//define the sub routine prototype

//A method to write a filename to the server and read in messages returned from the server
//Parameters: socket descriptor to talk to server, the filename to write to server
void writeToServer(int socketFD,char* filename) { 
	char buff[MAX];//a buffer to store data
	strncpy(buff,filename,MAX); //copy MAX number of bytes from filename to the buffer 
 	int readValue; //the value the read function returns
	FILE *filePointer;//a pointer to the file to write the output to
	char outputFilename[]="labSourceFileLocalCopy";//the output file name 
	
	//write the contents of buff to the server
        write(socketFD, buff, sizeof(buff));        

	//clear the contents of the buffer
	bzero(buff, sizeof(buff));
	
	//opens the output file in write mode which clears the file
	filePointer = fopen(outputFilename,"w");
	 
	//run forever for communication 
	for(;;){ 
		//try to read the MAX number of bytes from the socket and store in buffer
        	readValue=read(socketFD, buff, MAX);
		
		//test print to see how many bytes were read
		printf("Number of bytes read from server is %d\n",readValue);

		//write the contents of buffer one character at  time to the file
		fwrite(buff,1,readValue,filePointer);
		
		//if less than 1024 bytes have been read, assumed EOF has been reached
       		if(readValue<1024){
			break;
		} 
		
       	}
	//close the file
	fclose(filePointer);	
} 


//Main driving function for client 
//Parameters: argc - number of arguments passed , argv- the arguments passed  
int main(int argc, char *argv[]){

	
	if(argc!=2){//if two arguments havent been passed the program will exit
		printf("Please supply a filename only\n");
		exit(1);
	}
 
	int socketFD;//socket for connection to server 
    	struct sockaddr_in serverAddress; //structure to connect to server
	char *filename = argv[1];//filename is the 1st element in argv

	//initilase the socket
    	socketFD = socket(AF_INET, SOCK_STREAM, 0); 
    	
	//if socket fails to create
	if (socketFD == -1) { 
        	perror("socket creation failed\n"); 
        	exit(1); 
    	} 
    	else{//test print to say it was successfully created 
		printf("Socket successfully created-client\n"); 
    	} 
      
	//set ip address and port for connection 
    	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr =inet_addr("192.197.151.116"); 
    	serverAddress.sin_port = htons(PORT); 
  
    	//try to connect to the server
    	if (connect(socketFD,(SA*)&serverAddress, sizeof(serverAddress)) < 0) { 
        	perror("connection with the server failed\n"); 
        	exit(1); 
    	} 

	else{//test print for successful connection 
		printf("connected to the server\n"); 
  	}
	
	// function for chat 
	writeToServer(socketFD,filename); 
  
 	// close the socket 
	close(socketFD);

	return(0); 
} 
