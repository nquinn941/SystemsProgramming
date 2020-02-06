/*
Simple program to catch signals passed to the command line

Name: lab5.c

Written by: Niall Quinn - March 2019

Purpouse: Catch 3 different types of signal 

Usage: ./lab5execute 

Subroutines/libraries required: 
	see include statements.

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//define the prototypes for the methods
void userSignal(int signalNo);
void intSignal(int signalNo);
void quitSignal(int signalNo);

//a constant parameter for number of times it will catch ^C
#define MAX 5

//number of time ^C has been pressed 
int cPresses=0;

int main(){
	//if SIGINT signal has been caught
	if(signal(SIGINT,intSignal) == SIG_ERR){
		//if the SIGINT results in an error, print an error message
		printf("SIGINT cannot be caught\n");
		exit(1);
	}
	
	//if SIGQUIT has been caught
	if(signal(SIGQUIT,quitSignal) == SIG_ERR){
		//if it cant be caught, print an error message
                printf("SIGQUIT  cannot be caught\n");
		exit(1);        
	}
	
	if(signal(SIGUSR1,userSignal) == SIG_ERR){
		printf("SIGUSR1 cannot be caught\n");
		exit(1);
	}	

	//run forever
	for(;;){
		//print waiting message
		printf("Wait for a signal\n");
		pause();//pause to wait for signal
	}
	//return 0 to close main 
	return 0;
} 

//Sub routine to catch SIGINT signals
void intSignal(int sigNo){
	if(sigNo == SIGINT){
		//increment number of times c has been pressed
		cPresses++;
		
		//if the number is now greater than max, exit
		if(cPresses>MAX){
			exit(0);
		}
		
		//if it is equal to number of presses, print a message to let them know the max has been reached
		if(cPresses==MAX){
			printf("\nMax presses of ^C has been reached\n");
		}
		
		//if it is less than max
		else if(cPresses<MAX){
			//print the number of times it has been pressed 
			printf ("\nThis is the %d time you pressed ^C\n",cPresses);
			//reset signal
			signal(SIGINT,intSignal);		
		}
	}

	else{//if the signal recieved isnt SIGINT, print an error 
		printf("Error: recieved singal %d\n",sigNo);
		exit(1);
	}
		
}

//subroutine to catch SIGQUIT signals 
void quitSignal(int sigNo){
	pid_t pid,parent;//defines the pid for fork
	int status; //sets up status for killing child process
	
	//if signal recieved is SIGQUIT
	if(sigNo==SIGQUIT){
		//fork to create a child
		pid=fork();

	
		if(pid==-1){
			printf("Fork error\n");
			exit(1);
		}	
		if(pid==0){//fork successful 
	
			printf("\nI am Child, sending signal\n");//test print
			
			parent = getppid();
			
			printf("Killing parent with pid %d\n",parent);//test print
	
			kill(parent,SIGUSR1);//kills the parent processes 
				
			exit(0);//exits
		}

		//explicitily waits for the child to finish 
		waitpid(pid,&status,WNOHANG);
	}
}

//subroutine to catch user signals
void userSignal(int sigNo){
	if(sigNo==SIGUSR1){
		//test print for exiting programming 
		printf("Exiting program\n");
		//exit program
		exit(0);

	}

	else{
		printf("Wrong signal passed! \n");
		exit(1);
	}
}





