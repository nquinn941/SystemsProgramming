/*==============string_reverse.c==============================================
Reverse a given text file and store the output in a different file

Name: string_reverse.c

Written by: Niall Quinn 

Purpose: Reverse a given text file and store the output in a different file 

Usage: ./execute_string_reverse sourcefile output file 
	e.g ./execute_string_reverse source.txt output.txt 

Description of parameters: 
	argv[1] - name of source file 
	argv[2] - name of output file 

Subroutines/libraries required: 
	See include statmeents  

--------------------------------------------------------------
*/
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include <errno.h>


int main(int argc, char *argv[]){

	int fdin,fdout;//file pointers for source and destination files 	
	char inputfile[512];//source filename 
	char outputfile[512];//output filename 
	char buf;//buffer to write character to 
	int i;//counter variable 
	int arg1length;//length of argument 1 
	int arg2length;//length of argument 2 

	if(argc!=3){ //check if only 3 argumnets have been passed 
		printf("Please pass a source and destination file only\n");
		exit(-1);//exit the program if there aren't only 3 arguments 
	}
	
	//get size of arguments 1 and 2 passed to program 	
	arg1length = strlen(argv[1]);
	arg2length = strlen(argv[2]);

	
	if(arg1length>512){//if arg1length is greater than 512, exit 
		perror("Argument 1 too long");
		exit(-1);
	}
	
	if(arg2length>512){//if arg2length is greater than 512, exit
		perror("Argument 2 too long");
		exit(-1);
	}
	
	//copy the contents of the arguments passed to program into input/outputfile arrays
	strncpy(inputfile,argv[1],arg1length+1);
	strncpy(outputfile,argv[2],arg2length+1);
	//note: +1 is to ensure end character is copied to the new array
	

	if((fdin = open(inputfile,O_RDONLY))==-1){//check if source file opens
		printf("%s cannot be opened\terrno=%d\n",inputfile,errno);
		exit(-1);//exit if the file cant be open 
	}
	
	printf("Successfully opened %s\n",inputfile);//prints a statement for testing purpouses 

	if((fdout = open(outputfile,O_RDWR|O_CREAT))==-1){//checks if the output file can be opened
		printf("%s cannot be opened\n",outputfile);
		exit(-1);//exits if output file cant be opened 
	}
	
	printf("Successfully opened %s\n",outputfile);//prints a statement for testing purpose

	
	char buffer[512];//creates a buffer of where to store data 
	ssize_t nread;
	long total=0;//stores out many characters are in the source file
	while((nread = read(fdin,buffer,512))>0){//while there is still a character in the file
		total+=nread;//increment the total number of characters 
	}
	
	printf("Char count in %s is %li\n",inputfile,total);//prints the total number of characters in the source file for testing purposes 

	for(i=total-1;i>=0;i--){//start loop after EOF char and decrement
		if(lseek(fdin,i,SEEK_SET)==-1){
			perror("Seek error");
			exit(1);
		}

		if(read(fdin,&buf,1)==-1){
			perror("Read error");
			exit(1);
		}

		if(write(fdout,&buf,1)!=1){
			perror("Write error");
			exit(1);
		}
	}

	

	return 0;

}



