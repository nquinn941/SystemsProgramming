/*
The server side of a client server connection 

Name: server.c

Written by: Niall Quinn - April 2019

Purpose: To accept messages from the client, read a file and write back to client

Usage: ./serverRun & ./clientRun filename

Description of paramaters - N/A

Sub routines/libraries - see include statments
*/


#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

#define MAX 1024 //max number of bytes for one buffer
#define PORT 53090 //Port number
#define SA struct sockaddr //a macro so i dont have to type out struct sockaddr everytime 


//Method used to communicate between the client and server 
//Parameter - socketFD, the socket used to communicate between the client and server 
void chatToClient(int socketFD){ 
	char* filename;//array to store filename 
	FILE *fp;//file pointer
	char contentsBuffer[MAX];//array to store the contents of messages to be sent 
	char filenameBuffer[MAX];//array to store the name of the file to read from 
	int i,readValue;//loop counter and value of what the read function returns
	
	
	//read message from client with filename
	read(socketFD,filenameBuffer,sizeof(filenameBuffer));
	
	//open file name in read only mode  
	fp= fopen(filenameBuffer,"r");
	
	//checks if file has been opened 
	if(fp ==NULL){
		perror("File not opened\n");
		exit(1);
	}
	
	//file open is successful 
	else{
		printf("File Opened\n");
	}
	
	//runs until broken
	for(;;){
		//tries to read in 1024 characters from the file 
		readValue=fread(contentsBuffer,1,MAX,fp);
		
		//print read value for testing 
		printf("%d\n",readValue);

		//write the contents buffer out to the client
		write(socketFD,contentsBuffer,readValue);

		//if the file pointer has reached the end of file
		if(feof(fp)){
			//test print statement
			printf("End of file reached - server\n");
			break;//break from loop 
		}
	}
}

//driver function for the server code 
int main(){
		
	int socketFD,connectFD,length;//socket descriptor
	struct sockaddr_in serverAddress,client;
	
	//create socket
	socketFD = socket(AF_INET,SOCK_STREAM,0);
	
	//check socket is valid
	if(socketFD ==-1){
		//if not print that it has failed and exits
		printf("Socket creation failed-server\n");
		exit(1);
	}
	
	else{	//else print it was successful
		printf("Socket creation successful-server\n");
	}
	
	//assign the IP address and Port number for the communication 
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(PORT);
		
	//bind the socket and check if successful 
	if((bind(socketFD, (SA*)&serverAddress, sizeof(serverAddress)))<0){
		printf("Bind failed-server\n");
		exit(1);
	}
	
	else{//else print it is successful 
		printf("Bind successful-server\n");
	}
	
	//check the server can listen to the client
	if((listen(socketFD,5))!=0){
		printf("Listen failed\n");
		exit(1);
	}
	
	else{
		printf("Listen successful\n");
	}
	
	//length of the client address
	length = sizeof(client);
	
	//accept connection from client 
	connectFD = accept(socketFD,(SA*) &client, &length);
	
	//checks the accept has successeded
	if(connectFD<0){
		printf("Server accept fail\n");
		exit(1);
	}
	
	else{
		printf("Server accepted\n");
	}
	
	//call function to chat to the client 
	chatToClient(connectFD);
	
	//close the socket 
	close(socketFD);
	
	
	return(0);	
}	
	
