/*
 -------------------------------------
 File: collatz_sequence.c
 Project: Assignment 2
 -------------------------------------
 Author:  Talei Ibhanesebhor
 ID:      169020238
 Email:   ibha0238@mylaurier.ca
 Version  2023-10-17
 -------------------------------------
 */

#include <stdio.h>     
#include <unistd.h>    
#include <sys/wait.h>  
#include <stdlib.h>    

/* main */
int calculate(int n);

int main(int argc, char** argv){
FILE *fd = fopen("start_numbers.txt","r");
char line[10];
int n;
for (int i = 0; i < 3; i++){
    fgets(line, 10, fd);
    int n = atoi(line);
    calculate(n);
}
fclose(fd);

}

int calculate(int n){
	/* if n < 1 or > 500000 we treat it as out of bounds */
	if((n < 1) || (n > 10000)){
		fprintf(stderr, "Integer %d entered is either negative or out of bounds.\n", n);
	} else {
        printf("The Parent Process: The positive integer read from  filee is %d.\n", n);
    }
	
	/* fork a child process*/
	pid_t pid;
	pid = fork();
	
	/* if the process id (pid) < 0 the fork failed */
	if(pid < 0){
		fprintf(stderr, "Fork Failed");
		return(1);
	}
	/* else if pid == 0 run child process */
	else if(pid == 0){
        printf(" Child process: The generated collatz sequence is ");
		printf("%d", n);
		printf(", ");
		
		/* output counter counts the number of numbers
			in the sequence so that the output line is never
			more than 10 numbers long */
		int output_counter = 0;
		
		/* while loop to run the algorithm */
		while(n != 1){
			if(n % 2 == 0){
				n /= 2;
			}
			else{
				n = 3*n + 1;
			}

			/* print formatting */			
			printf("%d", n);
			if(n != 1){
				printf(", ");			
			}
			else{
				printf(" ");
			}
			
			/* if line longer than 10 go to next line */
			output_counter++;
			if(output_counter >= 10){
				printf("\n");
				output_counter = 0;
			}
		}
	}
	else{
		/* parent process waits for child process to complete */
		wait(NULL);
		printf("\nChild Complete\n");
	}
	
	return(0);

}