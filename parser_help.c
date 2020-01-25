//COP4610
//Project 1 Starter Code
//example code for initial parsing

//*** if any problems are found with this code,
//*** please report them to the TA


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct
{
    char** tokens;
    int numTokens;
} instruction;

void addToken(instruction* instr_ptr, char* tok);
void printTokens(instruction* instr_ptr);
void clearInstruction(instruction* instr_ptr);
void addNull(instruction* instr_ptr);
void IOredirection(char* inputfile, char* outputfile, bool input, bool output, char**function);
void singlepipes(char * cmd1, char *cmd2, int numofpipes);
void doublepipe(char ** cmd1, char ** cmd2, char ** cmd3);


int main() {
    char* token = NULL;
    char* temp = NULL;
    
    instruction instr;
    instr.tokens = NULL;
    instr.numTokens = 0;
    
    
    while (1) {
        printf("Please enter an instruction: ");
        
        // loop reads character sequences separated by whitespace
        do {
            //scans for next token and allocates token var to size of scanned token
            scanf("%ms", &token);
            temp = (char*)malloc((strlen(token) + 1) * sizeof(char));
            
            int i;
            int start = 0;
            for (i = 0; i < strlen(token); i++) {
                //pull out special characters and make them into a separate token in the instruction
                if (token[i] == '|' || token[i] == '>' || token[i] == '<' || token[i] == '&') {
                    if (i-start > 0) {
                        memcpy(temp, token + start, i - start);
                        temp[i-start] = '\0';
                        addToken(&instr, temp);
                    }
                    
                    char specialChar[2];
                    specialChar[0] = token[i];
                    specialChar[1] = '\0';
                    
                    addToken(&instr,specialChar);
                    
                    start = i + 1;
                }
            }
            
            if (start < strlen(token)) {
                memcpy(temp, token + start, strlen(token) - start);
                temp[i-start] = '\0';
                addToken(&instr, temp);
            }
            
            //free and reset variables
            free(token);
            free(temp);
            
            token = NULL;
            temp = NULL;
        } while ('\n' != getchar());    //until end of line is reached
        
        
        addNull(&instr);
        printTokens(&instr);
        
        //IOredirection("input.txt", "output.txt", true, true);
        //singlepipe()
        
        clearInstruction(&instr);
    }
    
    return 0;
}

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





//reallocates instruction array to hold another token
//allocates for new token within instruction array
void addToken(instruction* instr_ptr, char* tok)
{
    //extend token array to accomodate an additional token
    if (instr_ptr->numTokens == 0)
        instr_ptr->tokens = (char**) malloc(sizeof(char*));
    else
        instr_ptr->tokens = (char**) realloc(instr_ptr->tokens, (instr_ptr->numTokens+1) * sizeof(char*));
    
    //allocate char array for new token in new slot
    instr_ptr->tokens[instr_ptr->numTokens] = (char *)malloc((strlen(tok)+1) * sizeof(char));
    strcpy(instr_ptr->tokens[instr_ptr->numTokens], tok);
    
    instr_ptr->numTokens++;
}

void addNull(instruction* instr_ptr)
{
    //extend token array to accomodate an additional token
    if (instr_ptr->numTokens == 0)
        instr_ptr->tokens = (char**)malloc(sizeof(char*));
    else
        instr_ptr->tokens = (char**)realloc(instr_ptr->tokens, (instr_ptr->numTokens+1) * sizeof(char*));
    
    instr_ptr->tokens[instr_ptr->numTokens] = (char*) NULL;
    instr_ptr->numTokens++;
}

void printTokens(instruction* instr_ptr)
{
    int i;
    printf("Tokens:\n");
    for (i = 0; i < instr_ptr->numTokens; i++) {
        if ((instr_ptr->tokens)[i] != NULL)
            printf("%s\n", (instr_ptr->tokens)[i]);
    }
}

void clearInstruction(instruction* instr_ptr)
{
    int i;
    for (i = 0; i < instr_ptr->numTokens; i++)
        free(instr_ptr->tokens[i]);
    
    free(instr_ptr->tokens);
    
    instr_ptr->tokens = NULL;
    instr_ptr->numTokens = 0;
}
