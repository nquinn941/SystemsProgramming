/*

Name: lab3code.c

Written by: Niall Quinn - Feb 2019 

Purpouse: print out information on directories based on what parameters are passed 

Usage: ./lab3execute -parameters to use pathToDir
	e.g ./lab3execute -i -p /home/COIS/3380 

Description of parameters: argv[i] - the parameters or directory path to be passed to file 

Subroutines/Libraries required: See include statements

*/


#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


//a char array to store the current working directory 
char workingDirectory[4096];

//bools used to check what identifiers are being used 
bool usingI,usingP,usingA,usingM,usingS,usingB,usingD=false;


//ints used to compare to file size when using -s and -b parameters 
int smallCmpr,bigCmpr;

//declerations of functions to be used later on 
void setDirectory(char *);
void openDirectory(char *);
void displayPermissions(struct stat *);
void displayInodeNo(struct stat *);
void displayFileSize(struct stat *);
void getModTime(struct stat *);
void setOptions(int , char **);
int getFileSize(struct stat *);
void getAccessTime(struct stat *);
void printHeaders();
bool isDir(struct stat *);

int main(int argc,char* argv[]){
	
	char *lastArg = argv[argc-1];//get the last argument passed to the command line
	setDirectory(lastArg); //set the directory to be used
	printf("Current directory is %s \n",workingDirectory);//testing print statement to ensure the correct directory is being used
	
	setOptions(argc,argv);//set which paramenters have been passed to the program 
	openDirectory(workingDirectory);//open the directory and print contents 
	
	return 0;
}

void setOptions(int count,char** args){
	
	int i;//counter variable
	for (i=1;i<count;i++){//starts at 1 as arg[0] is always the name of the executable 
		char *parameter = args[i];//set the variable parameter to the ith argument
		
		//check the parameter passed against each of the potential parameters 
		
		if(strcmp(parameter,"-i")==0){
			//set the bool to true
			usingI=true;
		}

	 	else if(strcmp(parameter,"-p")==0){
                        usingP=true;
           	}
		
		else if(strcmp(parameter,"-a")==0){
                        usingA=true;
                }
		
		else if(strcmp(parameter,"-m")==0){
                        usingM=true;
                }

		else if(strcmp(parameter,"-s")==0){
                        usingS=true;
			//the next argument will be the number to compare to
			smallCmpr = atoi(args[i+1]);//convert the string to an int
                }
		
		else if(strcmp(parameter,"-b")==0){
                        usingB=true;
			bigCmpr = atoi(args[i+1]);
                }
		
		else if(strcmp(parameter,"-d")==0){
                        usingD=true;        
                }
	}
}

void setDirectory(char* lastArg){
	int argLength = strlen(lastArg);//gets the length of the argument passed to it 
	
	//if the first character of the last argument is a -, i assume that there has been no directory specified 
	if(lastArg[0] == '-'){
		
		//copies the '.' for pwd to working directory 
		strncpy(workingDirectory,".",1);
	}
	
	else{
		//copies the name of the directory to the working directory 
		strncpy(workingDirectory,lastArg,argLength);
	}
}

void openDirectory(char* directory){
	
	DIR *dirPointer;//pointer in the directory 
	struct dirent *dp;//directory structure is defined 
	struct stat fileStatus;// a new stat structure is created 
	
	
	//opens the directory specified 
	dirPointer = opendir(directory);
	
	//if the directory cant be opened, exit the program 
	if(dirPointer == NULL){
		printf("Couldn't open %s\n",directory);
		exit(1);
	}
	
	chdir(directory);
	
	printHeaders();

	for (;;){//runs while there are files left to read 
		//int returnValue;
		//struct stat fileStatus;
      		dp = readdir(dirPointer);//reads in the directory
 
       		if (dp == NULL){
			break;
		}
                                                     
       		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
			continue; //if the file name is '.' or '..' skip 
       		}
		
		int filenameLength = strlen(dp->d_name);//get the filename length 
		char filename[4096];
		int fileSize;
		
		//copies the name from directory into the filename variable 
		strncpy(filename,dp->d_name,filenameLength);
		filename[filenameLength]='\0';//add a null character to the end of the string to enusre no garbage is left behind 

		stat(filename, &fileStatus);//performs a stat on the filename 
		
		//checks what parameters have been passed and calls methods to print out that information		
		
		fileSize = getFileSize(&fileStatus);	
	
		if(!usingD && isDir(&fileStatus)){
			continue;
		}
		
		
		if(usingS){
			//compares the size of the file to the value passed to the command line
			if(fileSize>smallCmpr){
				continue;
			}
		}

		if(usingB){
			//compares the size of the file to value passed to command line
			if(fileSize<bigCmpr){
				continue;
			}
		}	

		
		if(usingI){
			displayInodeNo(&fileStatus);
			//printf("%ld\t",(long) dp->d_ino);
		}
		
		if(usingP){
			displayPermissions(&fileStatus);	
		}
		
		if(usingM){
			getModTime(&fileStatus);
		}

		if(usingA){
			getAccessTime(&fileStatus);
		}
		
		
		//always finishes by printing file size and name as these will always be printed 
		displayFileSize(&fileStatus);
		//if file size is larger than 5 characters, print less spacing than if it is smaller to ensure formatting
		if(fileSize>99999){
			printf("\t%s\n",filename);
		}
		else{
			printf("\t\t%s\n",filename);
    		}
	}
	
	//closes the open directory 
	closedir(dirPointer);
	
}

//got this code from stack overflow 
//checks the permissions on the file and prints the appropriate letter if true or a dash if false
void displayPermissions(struct stat *sb){
	printf( (S_ISDIR(sb->st_mode)) ? "d" : "-");
        printf( (sb->st_mode & S_IRUSR) ? "r" : "-");
        printf( (sb->st_mode & S_IWUSR) ? "w" : "-");
        printf( (sb->st_mode & S_IXUSR) ? "x" : "-");
        printf( (sb->st_mode & S_IRGRP) ? "r" : "-");
        printf( (sb->st_mode & S_IWGRP) ? "w" : "-");
        printf( (sb->st_mode & S_IXGRP) ? "x" : "-");
        printf( (sb->st_mode & S_IROTH) ? "r" : "-");
        printf( (sb->st_mode & S_IWOTH) ? "w" : "-");
        printf( (sb->st_mode & S_IXOTH) ? "x" : "-");
        printf("\t");
}

void displayInodeNo(struct stat *sb){
	//prints indoe information for the file
	printf("%ld\t", (long) sb->st_ino);
}

void displayFileSize(struct stat *sb){
	
	printf("%lld",(long long) sb->st_size);
}

int getFileSize(struct stat *sb){
	return (sb->st_size);//return the size of the file
}

void getModTime(struct stat *sb){
        struct tm ts;//define new time structure 
        char buf[80];//define buffer to write into 


        ts = *localtime(&sb->st_mtime);//get local time when file was last modified 
        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z",&ts);//write time into buf in correct format
        printf("%s\t", buf);//print buf
}

void getAccessTime(struct stat *sb){
	struct tm ts;//define a new time structure
	char buf[80];//use a buffer to write time into 

	
	ts = *localtime(&sb->st_atime);//get the local time of the files when last accessed 
	strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z",&ts);//write time into buf in applicable format 
	printf("%s\t", buf);//print out buf, followed by tab
}

void printHeaders(){
	//checks all the options and prints their header if applicable 
	if(usingI){
		printf("INODE\t");
	}

	if(usingP){
		printf("PERMISSIONS\t");
	}
	
	if(usingA){
		printf("LAST ACCESS\t\t\t");
	}
	
	if(usingM){
		printf("LAST MOD\t\t\t");
	}
	//prints size and name headers every time it is run
	
	printf("SIZE\t\t");
	printf("NAME\n");
	
}

bool isDir(struct stat *sb){
	return S_ISDIR(sb->st_mode);
}

