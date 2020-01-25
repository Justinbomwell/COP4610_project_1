#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>

void IOredirection(char* inputfile, char* outputfile, bool input, bool output, char**function);
void singlepipes(char * cmd1, char *cmd2, int numofpipes);
void doublepipe(char ** cmd1, char ** cmd2, char ** cmd3);



void IOredirection(char* inputfile, char* outputfile, bool input, bool output, char ** function)
{
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
            
            //printf("testing 3 \n");
            
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
                
                //printf("testing 5 \n");
                
                dup2(fd2, STDOUT_FILENO);
                
                close (fd2);
                
            }
        }
        //printf(" test \n");
        
        //f();
        my_execute(function, 1);
        exit(1);
        
        
        
    }// end of pid if statement
    
    else
    {
        waitpid(0);
        
    }
    
}//end of function


void singlepipe(char ** cmd1, char ** cmd2)
{
    int fd[2];
    pipe(fd);
    pid_t p1 = fork();
    
    if (p1 == 0)        // child 1 for output redirection
    {
        close (fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        my_execute(cmd1, 1);
    }
    else                 //parent
    {
        pid_t p2 = fork();         //child 2 inside parent
        if (p2 == 0)        //child 2 for input redirection
        {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            my_execute(cmd1, 1);
        }
        
        else         //parent executing
        {
            waitpid(0);
            waitpid(0);
        }
        
    }
    exit(1);
    
}//end of single pipe

void doublepipe(char ** cmd1, char ** cmd2, char ** cmd3)
{
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
                my_execute(cmd3, 1);
            }
        }
        else             //child #2
        {
            close(0); dup(3); close(1); dup(6); close(3); close(4); close(5);
            my_execute(cmd2, 1);
        }
        
    }
    else                //child #1
    {
        close(1); dup(4); close(3); close(4); close(5); close(6);
    }
    
    
}    //end of function
