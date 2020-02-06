/* 

Name: lab4.c

Written by: Niall Quinn - March 2019

Purpouse: Creates two children and a granchild process and performs various tasks

Usage: 	gcc -Wall -O lab4execute lab4.c 
	./lab4execute

Subroutines/libraries: See below

*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include<fcntl.h>
#include<string.h>
#include <errno.h>

int main(int argc, char *argv[]){

	pid_t child1,child2,grandchild,finishedChild,finishedSecond; //create pid's for the two children, grandchild and to see which pid finishes first 
	int status,grandchildStatus,fdin,fdout,i;//ints for status of children/grandchildren, file pointers and a loop counter
	char inputfile[] = "/home/COIS/3380/lab4_sourcefile";//string for input file
	char outputfile[] = "/home/niallquinn/lab4/lab4_file_copy";//string for output file
	char buf,buffer[2048];//buffer for reading from file and a buffer used for reading the amount of chars in file 
	ssize_t nread;//signed size block of memory 
	long total =0;//total used for number of characters in input file

	printf("Parent: PID: %d\n",getpid());//print the pid of the parent process for testing purpouses 
	
	child1 = fork();//fork child 1
	
	//if child1 isnt equal to 0, the fork has failed 
	if(child1==-1){
		//print out error
		printf("Fork Failed\n");
		//exit the system
		exit(1);
	}
	
	//if child1 equals 0, the fork has been successful 
	if(child1==0){

		//print the pid and ppid of child1 for testing purpouses 
		printf("Child 1 with pid %d and ppid %d has been created\n\n",getpid(),getppid());

		//fork the grandchild process
		grandchild = fork();

		//if granchild isnt equal to 0, the fork has failed
		if(grandchild==-1){
			//print error
			perror("Fork Failed");
			//exit the system
			exit(1);
		}
		
		//if granchild does equal 0, the fork has been successful 
		if(grandchild ==0){
			//print the pid and ppid for the grandchild process for testing purpouses 
			printf("Grandchild with pid %d and ppid %d has been created\n\n",getpid(),getppid());
			
			//sleep the process for 3 seconds
			sleep(3);


			printf("\nPrinting head of lab4.c\n");
			//calls the execlp function to print the head of this file 
			execlp("head","head","-n","20","lab4.c",NULL);
			printf("\n");
			
			//exits with exit status of 0
			exit(0);
		}
		
		//waits explicitly for the grandchild process to finish
		while(waitpid(grandchild,&grandchildStatus,WNOHANG)==0){
			//sleeps for one second when not successful
			sleep(1);
			//prints try again while its not done
			printf("Try again\n");
			
		}

		//checks if the grandchild has exited normally
		if(WIFEXITED(grandchildStatus)){
			//print the pid of the process that has finished for testing
			printf("Grandchild %d has exited\n",grandchild);
		}
		
		//exits the child1 process
		exit(0);
	}
	
	
	//fork child2
	child2 = fork();

	//if child2 doesnt equal 0, the fork has failed
	if(child2==-1){
		perror("Fork Failed");
		exit(1);
	}
	

	//if child2 equals 0, the fork has been successful 
	if(child2==0){
		//print pid and ppid of the child for testing purpouses 
		printf("Child 2 with pid %d and ppid %d has been created\n\n",getpid(),getppid());
		
		//tries to open the inputfile with read only privilages 
		if((fdin = open(inputfile,O_RDONLY))==-1){
			//if it cant print error message and number
                	printf("%s cannot be opened\terrno=%d\n",inputfile,errno);
                	exit(-1);//exit with code -1 if cant be open
		}


		//tries to open the outputfile with read,write and create privilages
		if((fdout = open(outputfile,O_RDWR|O_CREAT))==-1){
			//print error message
                	printf("%s cannot be opened\n",outputfile);
                	exit(-1);//exits if output file cant be opened
		}
		
		//print both files have been opened for testing
		printf("files %s and %s have been opened\n",inputfile,outputfile);
		
		//while nread can read from the file 
		while((nread = read(fdin,buffer,512))>0){
                	total+=nread;//add the value from nread to the total
		}

		//print char count of the file for testing
		printf("Char count in %s is %li\n",inputfile,total);
	
	
		//starts at start of file and goes until EOF character
		for(i=0;i<total;i++){
			//seeks the pointer to i
			if(lseek(fdin,i,SEEK_SET)==-1){
				//print error and exit
        			perror("Seek error");
                		exit(1);
    			}
			
			//tries to read from file to buffer
     			if(read(fdin,&buf,1)==-1){
				//print error and exit
                		perror("Read error");
                		exit(1);
       			}	
			
			//tries to write from buf to outputfile
        		if(write(fdout,&buf,1)!=1){
				//print error and exit
                 		perror("Write error");
                 		exit(1);
        		}
		}
		
		//exit child2
                exit(0);
	}
	
	//wait for any child to exit and assign it to finished child pid
	finishedChild = wait(NULL);
	//print which child has finished for testing
	printf("Child with pid %d has finished\n",finishedChild);
	

	//if child1 is the finishedchild
	if(child1 == finishedChild){
		//wait explicitly for child2 to finish
		while(waitpid(child2,&status,WNOHANG)==0){
			//prints waiting and sleeps process
			printf("Waiting on child2\n");
			sleep(1);
		}
		//sets pid of finishedsecond to child2
		finishedSecond = child2;
	}

	
	else{
		//else wait for child1
		while(waitpid(child1,&status,WNOHANG)==0){
			//print wait and sleep	
			printf("Waiting on child1\n");
			sleep(1);		
		}
		//set pid of finished second to child1
		finishedSecond = child1;
	}
	
	//if the child has been exited normally
	if(WIFEXITED(status)){
		//print the pid of the child that finished second
		printf("Child with pid %d has finished\n",finishedSecond);
	}	
	
	//return the main
	return(0);
}
		
