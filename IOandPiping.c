#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>


void IOredirection( instruction* instr_ptr, int bGround);
void singlepipe( instruction* instr_ptr, int bGround);
void doublepipe( instruction* instr_ptr, int bGround);


void IOredirection( instruction* instr_ptr, int bGround)
{

    char * inputfile; 
    char * outputfile; 
    bool input = false; 
    bool output = false; 
    char * cmd[2];


    strcpy(cmd[0], (instr_ptr->tokens)[0]);
    cmd[1] = NULL; 

    int i;
    for (i = 0; i < instr_ptr->numTokens; i++) 
    {
        if ((instr_ptr->tokens)[i] != NULL)
        {

            if (strcmp((instr_ptr->tokens)[i],"<") == 0)
            {
                input = true; 
                strcpy(inputfile, (instr_ptr->tokens)[i+1]);
            }
            if (strcmp((instr_ptr->tokens)[i],">") == 0)
            {
                output = true; 
                strcpy(outputfile, (instr_ptr->tokens)[i+1]);
            }
        }

    }
    //bool input and output are for if there is input and output redirection needed
    
    //fd is file descriptor
    //cd is copy file descriptor
    
    //output redirection
    
    
    
    pid_t pid = fork();
    if (pid == 0)
    {
        
        if (input == true)
        {
            int fd =  open(inputfile, O_RDONLY);
            
            if (fd == -1)            //will return -1 if file can't be opened
            {
                printf("Error. Cannot open filename: %s\n", inputfile);
            }
            else                 //returns positive number if opened properly
            {
                printf("testing 2 \n");
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
        }
        
        
        //input redirection
        if (output == true)
        {
            //    printf("testing 4 \n");
            int fd2 = open(outputfile, O_CREAT|O_WRONLY|O_TRUNC);         //WHAT DOES TRUNC do
            
            if (fd2 == -1)
            {
                printf("Error. Cannot open filename: %s\n", outputfile);
                exit(0);
            }
            else
            {

                dup2(fd2, STDOUT_FILENO);             
                close (fd2);  
            }
        }
        my_execute(cmd, 1, bGround);
        exit(1);
        
        
        
    }// end of pid if statement
    
    else	//parent function 
    {
        waitpid(0);
        
    }
    
}//end of function


void singlepipe(instruction * instr_ptr, int bGround)
{
	char * cmd1[2]; 
	char * cmd2[2]; 
    cmd[1] = NULL; 
    cmd[2] = NULL; 

	int i;
    for (i = 0; i < instr_ptr->numTokens; i++) {
        if ((instr_ptr->tokens)[i] != NULL)
        {
            if (strcmp((instr_ptr->tokens)[i],"|") == 0)
            {
                strcpy(cmd1[0], (instr_ptr->tokens)[i-1]);
                strcpy(cmd2[0], (instr_ptr->tokens)[i+1]);
            }
        }
    }


    int fd[2];
    pipe(fd);
    pid_t p1 = fork();
    
    if (p1 == 0)        // child 1 for output redirection
    {
        close (fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        my_execute(cmd1, 1, bGround);
    }
    else                 //parent
    {
        pid_t p2 = fork();         //child 2 inside parent
        if (p2 == 0)        //child 2 for input redirection
        {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            my_execute(cmd2, 1, bGround);
        }
        
        else         //parent executing
        {
            waitpid(0);
            waitpid(0);
        }
        
    }
    exit(1);
    
}//end of single pipe

void doublepipe(instruction * instr_ptr, int bGround)
{
    char * cmd1[2]; 
    char * cmd2[2]; 
    char * cmd3[2]; 
     cmd[1] = NULL; 
    cmd[2] = NULL; 
    cmd[3] = NULL; 
	int i; 
	int x = 0; 
    for (i = 0; i < instr_ptr->numTokens; i++) {
        if ((instr_ptr->tokens)[i] != NULL)
            if (strcmp((instr_ptr->tokens)[i],"|") == 0)
            {
            	x = 1; 
                strcpy(cmd1[0], (instr_ptr->tokens)[i-1]);
                strcpy(cmd2[0], (instr_ptr->tokens)[i+1]);
            }
            if ((strcmp((instr_ptr->tokens)[i],"<") == 0) && x == 1)
            {
            	strcpy(cmd3[0], (instr_ptr->tokens)[i+1]);

            }
    }

    int fd1[2];
    int fd2[2];
    
    pipe(fd1);
    pipe(fd2);
    
    pid_t p1 = fork();
    
    if (p1 > 0)            //parent (now 1 child)
    {
        pid_t p2 = fork();         //parent (now 2 children)
        if (p2 > 0)
        {
            pid_t p3 = fork();
            if (p3 > 0)            //parent (now 3 children)
            {
                close(3);
                close(4);
                close(5);
                close(6);
                waitpid(0);
                waitpid(0);
                waitpid(0);
                
            }
            else             //child #3
            {
                close(0);
                dup(5);
                close(3); close(4); close(5); close(6);
                my_execute(cmd3, 1, bGround);
            }
        }
        else             //child #2
        {
            close(0); dup(3); close(1); dup(6); close(3); close(4); close(5);
            my_execute(cmd2, 1, bGround);
        }
        
    }
    else                //child #1
    {
        close(1); dup(4); close(3); close(4); close(5); close(6);
        my_execute(cmd1, 1, bGround); 
    }
    
    
}    //end of function
