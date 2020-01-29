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
#include "executionProcessing.h"

typedef struct
{
    char** tokens;
    int numTokens;
} instruction;


void addToken(instruction* instr_ptr, char* tok);
void printTokens(instruction* instr_ptr);
void clearInstruction(instruction* instr_ptr);
void addNull(instruction* instr_ptr);

//IO&PIPING FUNCTION CALLS
void IOredirection( instruction* instr_ptr, int bGround, char* cmd);
void singlepipe( instruction* instr_ptr, int bGround);
void doublepipe( instruction* instr_ptr, int bGround, char* cmd);

//FUNCTIONS FUNCTION CALLS
void enVar(instruction* instr_ptr);     //expands all environmental variables entered in command line
void printPrompt();                     //prints the user@machine: working directory> onto command line
int shortRes(instruction* instr_ptr);   //Takes in tokens and expands '.', "..", and '~' to working directory,
                                        //parent directory, and home directory
int pathRes(instruction* instr_ptr);    //takes in tokens that are executables and expands them
void func(instruction * instr_ptr,int commandCounter, char* tem); //Called in main to implement all parts of project
int isFile(const char * path);          //checks if cstring is a file somewhere on the machine
int isDirectory(const char * path);     //checks if cstring is a directory somewhere on the machine



int main() {
    char* token = NULL;
    char* temp = NULL;

    instruction instr;
    instr.tokens = NULL;
    instr.numTokens = 0;
    int commandCounter = 0;
    int running = 1;

    backgroundInit();

    while (running)
    {
        checkQueue();
        printPrompt();
        char *cmd;

        // loop reads character sequences separated by whitespace
        do {
            //scans for next token and allocates token var to size of scanned token
            scanf("%ms", &token);
            cmd = malloc(strlen(token)+1); 
            strcpy(cmd, token); 
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

        if(shortRes(&instr) == 0)
        {
            printf("Invalid Directory or File Entered\n");
        }
        else
        {
            //printTokens(&instr);

            enVar(&instr);
            func(&instr, commandCounter, cmd);
        }

        commandCounter++;

        clearInstruction(&instr);
    }

    return 0;
}

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

void IOredirection( instruction* instr_ptr, int bGround, char* cmd)
{

    char * inputfile;
    char * outputfile;
    bool input = false;
    bool output = false;
    instruction cmd1;
   cmd1.tokens = NULL;
   cmd1.numTokens = 0;
  
    int i;
    int p = 0; 
    for (i = 0; i < instr_ptr->numTokens-1; i++)
    {
        if ((instr_ptr->tokens)[i] != NULL)
        {
            if (strcmp((instr_ptr->tokens)[i],"<") == 0 || strcmp((instr_ptr->tokens)[i],">") == 0)
            {
                p = 1; 
            }

            if (p == 0)
            {               
                addToken(&cmd1, (instr_ptr->tokens)[i]);
            }


            if (strcmp((instr_ptr->tokens)[i],"<") == 0)
            {
                input = true;
                inputfile = (char *) calloc(strlen((instr_ptr->tokens)[i+1]), sizeof(char));
                strcpy(inputfile, (instr_ptr->tokens)[i+1]);

            }
            if (strcmp((instr_ptr->tokens)[i],">") == 0)
            {
                output = true;
                outputfile = (char *) calloc(strlen((instr_ptr->tokens)[i+1]), sizeof(char));
                strcpy(outputfile, (instr_ptr->tokens)[i+1]);
            }
        }

    }

    addNull(&cmd1);
    pathRes(&cmd1);

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
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
        }


        //input redirection
        if (output == true)
        {
            int fd2 = open(outputfile, O_CREAT|O_WRONLY|O_TRUNC);      

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
        my_execute(cmd1.tokens, bGround, cmd);
       // execv(cmd1.tokens[0], cmd1.tokens);
        exit(1);



    }// end of pid if statement

    else    //parent function
    {
        waitpid(pid, NULL, 0);
    }

    if (input == true)  {   free(inputfile); }
    if (output == true) {   free (outputfile); }

}//end of function


void singlepipe(instruction * instr_ptr, int bGround)
{
    int status;          //these commands are temporary holders for the indivisual commands between pipes 
    instruction cmd1;
    instruction cmd2;
    cmd1.tokens = NULL;
    cmd1.numTokens = 0;
    cmd2.tokens = NULL;
    cmd2.numTokens = 0;

    int i;
    int p = 0; 
    for (i = 0; i < instr_ptr->numTokens; i++)             //this loop is to copy the commands into the holders
    {
        if ((instr_ptr->tokens)[i] != NULL)
        {   

            if (strcmp((instr_ptr->tokens)[i],"|") == 0)
            {
                p = 1; 
                i++; 
            }   

            if (p == 0)
            {
                addToken(&cmd1, (instr_ptr->tokens)[i]);
            }

             if (p == 1)
            {
                addToken(&cmd2, (instr_ptr->tokens)[i]);
            }  

            
        }
    }
    addNull(&cmd1);                                 
    pathRes(&cmd1);
    addNull(&cmd2);
    pathRes(&cmd2);


    int fd[2];
    pid_t p2;

    pid_t p1 = fork();
    if (p1 == 0)        // child 1 for output redirection
    {

        if (pipe(fd) < 0)
        {  printf("Pipe could not be initialized. \n");
        }

        p2 = fork();         //child 2 inside parent
        if (p2 == 0)        //child 2 for input redirection
        {
            close(STDOUT_FILENO);
            dup(fd[1]);
            close(fd[0]);
            close(fd[1]);
            if (execv(cmd1.tokens[0], cmd1.tokens) < 0)
            {
                printf("Command could not be executed. \n" );
            }
            exit(0);
        }
        else
        {
            waitpid(p2, &status, 0);

            close(STDIN_FILENO);
            dup(fd[0]);
            close(fd[0]);
            close(fd[1]);
            if (execv(cmd2.tokens[0], cmd2.tokens) < 0)
            {
                printf("Command could not be executed. \n" );
            }
            exit(0);
        }

    }
    else                 //parent
    {
        waitpid(p1, &status, 0);
        waitpid(p2, &status, 0);

    }

    // exit(1);
    clearInstruction(&cmd1); 
    clearInstruction(&cmd2); 

}//end of single pipe

void doublepipe(instruction * instr_ptr, int bGround, char* cmd)
{
                 //these commands are temporary holders for the indivisual commands between pipes 
    instruction cmd1;
    instruction cmd2;
    instruction cmd3; 
    cmd1.tokens = NULL;
    cmd1.numTokens = 0;
    cmd2.tokens = NULL;
    cmd2.numTokens = 0;
    cmd3.tokens = NULL; 
    cmd3.numTokens = 0; 

    int i;
    int p = 0; 
    for (i = 0; i < instr_ptr->numTokens; i++)                  //this loop is to copy the commands into the holders
    {
        if ((instr_ptr->tokens)[i] != NULL)
        {   

            if (strcmp((instr_ptr->tokens)[i],"|") == 0)
            {
                p++; 
                i++; 
            }   

            if (p == 0)
            {
                addToken(&cmd1, (instr_ptr->tokens)[i]);
            }

             if (p == 1)
            {
                addToken(&cmd2, (instr_ptr->tokens)[i]);
            }  
            if (p == 2)
            {
                addToken(&cmd3, (instr_ptr->tokens)[i]);
            } 

            
        }
    }
    addNull(&cmd1);
    pathRes(&cmd1);
    addNull(&cmd2);
    pathRes(&cmd2);
    addNull(&cmd3);
    pathRes(&cmd3);




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
                // waitpid(0);
                // waitpid(0);
                // waitpid(0);

            }
            else             //child #3
            {
                close(0);
                dup(5);
                close(3); close(4); close(5); close(6);
                execv(cmd3.tokens[0], cmd3.tokens);
                exit(1); 
            }
        }
        else             //child #2
        {
            close(0); dup(3); close(1); dup(6); close(3); close(4); close(5);
            execv(cmd2.tokens[0], cmd2.tokens);
                            exit(1); 

        }

    }
    else                //child #1
    {
        close(1); dup(4); close(3); close(4); close(5); close(6);
        execv(cmd1.tokens[0], cmd1.tokens);
                        exit(1); 

    }


}    //end of function

void printPrompt()      //prints the user@machine: working directory> onto the command line
{
    printf("%s", getenv("USER"));
    printf("@");
    printf("%s:", getenv("MACHINE"));
    printf("%s>", getenv("PWD"));
}

void enVar(instruction* instr_ptr) //Expands environmental variables
{
    int a;

    for(a = 0; a < instr_ptr->numTokens; a++)   //Loops through all tokens in instruction
    {
        if((instr_ptr->tokens)[a] != NULL)  //Make sure no segfault on last null token
        {
            if(strcmp((instr_ptr->tokens)[a], "$HOME") == 0)    //expands $HOME env. variable
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("HOME")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("HOME"));
            }
            else if(strcmp((instr_ptr->tokens)[a], "$PATH") == 0)   //expands $PATH env. variable
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("PATH")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("PATH"));
            }
            else if(strcmp((instr_ptr->tokens)[a], "$USER") == 0)   //expands $USER env. variable
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("USER")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("USER"));
            }
            else if(strcmp((instr_ptr->tokens)[a], "$SHELL") == 0)  //expands $SHELL env. variable
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("SHELL")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("SHELL"));
            }
            else if(strcmp((instr_ptr->tokens)[a], "$PWD") == 0)    //expands $PWD env. variable
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("PWD")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("PWD"));
            }
        }
    }
} //END OF FUNCTION

int shortRes(instruction* instr_ptr)
{
    int a, c, d, e, f, g, h, i;
    int chk;
    int count = 0;
    int checkDelim = 0;         //Used to track allocation and deallocation of tmp strings
    int t2Check = 0;
    int t3Check = 0;
    int t4Check = 0;

    char * t1 = NULL;           //Instantiate temporary cstrings that are going to be allocated
    char * t2 = NULL;
    char * t3 = NULL;
    char * t4 = NULL;
    char * delim = NULL;

    for(a = 0; a < instr_ptr->numTokens; a++)   //For loop through all the instructions tokens
    {
        if((instr_ptr->tokens)[a] != NULL)      //Make sure no seg fault for last null token
        {
            if(strcmp((instr_ptr->tokens)[a], ".") == 0)    //If token == '." make it $PWD variable
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("PWD")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("PWD"));
            }
            else if(strcmp((instr_ptr->tokens)[a], "..") == 0)  //If token == ".." make token directory one up
            {
                for(c = strlen(getenv("PWD"))-1; c >= 0; c--)   //find where second to last '/' is in token
                {
                    if(getenv("PWD")[c] == '/')
                    {
                        chk = c;
                        break;
                    }
                }

                (instr_ptr->tokens)[a] = (char *) calloc(chk, sizeof(char)); //New size where second to last '/' was

                for(d = 0; d < chk; d++)                            //make token cstring up until second to last '/'
                {
                    ((instr_ptr->tokens)[a])[d] = getenv("PWD")[d];
                }
            }
            else if(strcmp((instr_ptr->tokens)[a], "~") == 0)       //if token == '~' change it to $HOME env. variable
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("HOME")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("HOME"));
            }
            else if(strpbrk((instr_ptr->tokens)[a], "/") != NULL)   //if token contains '/' anywhere in it
            {
                if(((instr_ptr->tokens)[a])[0] == '~')  //if '~' in first spot of string only
                {
                    //temp cstring allocated for size of $HOME + original token -1
                    t1  = (char *) calloc(strlen(getenv("HOME")) + strlen((instr_ptr->tokens)[a]) - 1, sizeof(char));

                    //concatentate $HOME to beginning of temp cstring
                    for(e = 0; e < strlen(getenv("HOME")); e++)
                    {
                        t1[e] = getenv("HOME")[e];
                    }

                    //concatenate everything from token after "~/" onto temp string
                    for(f = strlen(getenv("HOME")); f < strlen(getenv("HOME"))+strlen((instr_ptr->tokens)[a])-1; f++)
                    {
                        t1[f] = ((instr_ptr->tokens)[a])[f-strlen(getenv("HOME"))+1];
                    }

                    //copy temp string onto original token
                    (instr_ptr->tokens)[a]  = (char *) calloc(strlen(getenv("HOME")) + strlen((instr_ptr->tokens)[a]) - 1, sizeof(char));
                    strcpy((instr_ptr->tokens)[a], t1);

                    //free temp string memory
                    free(t1);
                }


                delim = strtok((instr_ptr->tokens)[a], "/");    //tokenize string with "/" as delimeter into cstring delim

                while(delim != NULL)                            //run until token empty(NULL)
                {
                    if(strcmp(delim, ".") == 0)                 //if tokenized delim cstring is == '.'
                    {
                        checkDelim = 1;                         //make sure delim is deallocated later

                        delim = (char *) calloc(strlen(getenv("PWD")), sizeof(char)); //allocate PWD strlen into this token
                        strcpy(delim, getenv("PWD"));   //set token delim equal to working directory

                        if(t2 != NULL)  //if strings have already been concatenated into t2 then go into this
                        {
                            t3Check = 1;    //make sure t3 deallocaated after this allocation
                            t3 = (char *) calloc(strlen(t2), sizeof(char));

                            for(g = 0; g < strlen(t2); g++) //make t3 contents same as t2
                                t3[g] = t2[g];


                            t2Check = 1; //deallocate t2
                            //add the size of delim to original t2
                            t2 = (char *) calloc(strlen(delim)+strlen(t3)+1, sizeof(char));

                            //concatentenate original t2 to t3 and then the new delim with $PWD in it
                            strcat(t2, t3);
                            strcat(t2, delim);
                        }
                        else    //if t2 is empty just put the working directory into t2
                        {
                            t2Check = 1; //make sure t2 is deallocated
                            t2 = (char *) calloc(strlen(delim)+1, sizeof(char));
                            strcat(t2, delim);
                        }
                    }
                    else if(strcmp(delim, "..") == 0)   //if new token delim equals ".." change t2 to one directory up
                    {
                        if(count != 0)  //make sure this isnt the first token (I.E. used on root directory)
                        {
                            h = strlen(t2);
                            while(t2[h] != '/') //find second to last '/' in t2
                                h--;

                            t4Check = 1;    //make sure t4 is deallocated
                            t4 = (char *) calloc(strlen(t2), sizeof(char)); //make t4 contents equal to t2
                            strcpy(t4, t2);

                            t2Check = 1;    //make sure t2 is deallocated
                            t2 = (char *) calloc(h+1, sizeof(char)); // t2's new size is euqal to second to last '/' position

                            if(t4[0] != '/')    //make sure tokens seperated by '/' & move new size t4 = t2
                            {
                                strcat(t4, "/");

                                for(i = 1; i < h+1; i++)
                                    t2[i] = t4[i-1];
                            }
                            else
                            {
                                for(i = 0; i < h; i++)
                                    t2[i] = t4[i];
                            }

                            if(strlen(t2) == 0) // root directory case
                                strcat(t2, "/");

                        }
                        else    //if used on root directory break while loop
                        {
                            break;
                        }
                    }
                    else //if delim token not '.' or ".." then just concatenate onto t2
                    {
                        if(t2 != NULL)  //if t2 already has contents concatenated into it
                        {
                            t3Check = 1;    //make sure t3 is deallocated after its contents become t2's contents
                            t3 = (char *) calloc(strlen(t2), sizeof(char));

                            for(g = 0; g < strlen(t2); g++) //make t3 = t2's contents
                                t3[g] = t2[g];

                            t2Check = 1;    //make sure t2 is deallocated

                            //t2's new size is t3 size + delim token + 1 for '/'
                            t2 = (char *) calloc(strlen(delim)+strlen(t3)+1, sizeof(char));

                            //concat original t2(t3) to t2
                            strcat(t2, t3);

                            //concat a '/' if none on delims 1st position
                            if(delim[0] != '/')
                                strcat(t2, "/");

                            //concat delim token to t2
                            strcat(t2, delim);
                        }
                        else    //if first use of t2
                        {
                            //make t2 size of delim + '/'
                            t2Check = 1;
                            t2 = (char *) calloc(strlen(delim)+1, sizeof(char));

                            if(delim[0] != '/')
                                strcat(t2, "/");

                            //concat delim to t2
                            strcat(t2, delim);
                        }
                    }

                    //get new token from instr_ptr->tokens[a]
                    delim = strtok(NULL, "/");
                    count++;    //add how many times concat happened to t2
                }

                if(count != 0)  //if something in t2
                {
                    //make instr_ptr->tokens[a] equal contents of t2 which is final instruction token
                    (instr_ptr->tokens)[a] = (char *) calloc(strlen(t2), sizeof(char));
                    strcpy((instr_ptr->tokens)[a], t2);
                }

                //deallocate all temporary strings if needed
                if(t2Check == 1) free(t2); t2 = NULL;
                if(t3Check == 1) free(t3); t3 = NULL;
                if(t4Check == 1) free(t4); t4 = NULL;
                if(checkDelim == 1) free(delim); delim = NULL;

                //check to see if tokens that went through shortRes are actual files or directories
                //if not return 0
                if(isFile((instr_ptr->tokens)[a]) == 0 && isDirectory((instr_ptr->tokens)[a]) == 0)
                    return 0;
            }
        }
    }
    //return 1 if all resolutions are files or directories
    return 1;
} //END OF FUNCTION

int pathRes(instruction* instr_ptr) //Everything that isnt a built in or a path with '/' in it needs to be resolved
{
    int a;

    char * path1;   //used to tokenize to see if an executable file is in one of the tokens
    char * delim;   //individual tokens of path
    char * t1;      //used for copying purposes

    for(a = 0; a <instr_ptr->numTokens; a++)    //for loop thorugh all the tokens
    {
        //make sure not a builtin or a token containing '/' which indicates its already resolved
        if((instr_ptr->tokens)[a] != NULL && strpbrk((instr_ptr->tokens)[a], "/") == NULL && strcmp((instr_ptr->tokens)[a], "cd") != 0 &&
           strcmp((instr_ptr->tokens)[a], "jobs") != 0 && strcmp((instr_ptr->tokens)[a], "exit") != 0 && strcmp((instr_ptr->tokens)[a], "echo") != 0)
        {
            //make path1 c-string into environmental variable $PATH
            path1 = (char *) calloc(strlen(getenv("PATH")), sizeof(char));
            strcpy(path1, getenv("PATH"));

            //make delim tokenized elements of path1 using ':' as a delimeter
            delim = strtok(path1, ":");

            //continues until delim equals the null part of path1
            while(delim != NULL)
            {
                //makes t1 size of tokenized delim + executable command from instruction token
                t1 = (char *) calloc(strlen(delim)+strlen((instr_ptr->tokens)[a])+1, sizeof(char));
                //places all of these parts together with a '/' in between
                strcat(t1, delim);
                strcat(t1, "/");
                strcat(t1, (instr_ptr->tokens)[a]);

                //checks if t1 is a file
                if(access(t1, F_OK) != -1)
                {
                    //if t1 is a file make instr_ptr->tokens[a] into the resolved path for executable command
                    (instr_ptr->tokens)[a] = (char *) calloc(strlen(t1), sizeof(char));
                    strcpy((instr_ptr->tokens)[a], t1);
                    return 1;
                }

                delim = strtok(NULL, ":"); // continue tokenizing path1 into telim until null
            }

            //free variables
            if(path1 != NULL) free(path1);
            if(t1 != NULL) free(t1);
        }
    }

    //return 0 if no paths were resolved correctly
    return 0;
}

void func(instruction * instr_ptr, int commandCounter, char* tem)
{
    int a;
    int check = 0; // 1 ==> execute, 2 ==> I/O, 3 ==> piping
    int bGround = 0;
    int numofpipes = 0;


    //----------------------------------------------------------------------------checking for redirection, built ins, pipes, and regular execution
    for(a = 0; a < instr_ptr->numTokens-1; a++)
    {

        // if (strcmp((instr_ptr->tokens)[a], NULL) == 0) break;
        //if (instr_ptr->numTokens == 1) break;

        if(strcmp((instr_ptr->tokens)[a], "<") == 0 || strcmp((instr_ptr->tokens)[a], ">") == 0)
        {    check = 2;  break;  }
        else if(strcmp((instr_ptr->tokens)[a], "|") == 0)
        {    check = 3;  break;  }


        else if(strcmp((instr_ptr->tokens)[a], "exit") == 0)
        {    exitShell((char**) instr_ptr, commandCounter); break;   }

        else if(strcmp((instr_ptr->tokens)[a], "jobs") == 0)
        {    int i = 0;
            while(pEntry[i] != NULL)
            {
                jobs(pEntry, i);
                i++;
            }
            break;
        }

        else if(strcmp((instr_ptr->tokens)[a], "cd") == 0)
        {    changeDirectory(instr_ptr->tokens[1]); break;   }

        else if (strcmp((instr_ptr->tokens)[a], "echo") == 0)
        {    int i = 1;
            while((instr_ptr->tokens)[a + i] != NULL)
            {
                echo(instr_ptr->tokens[a+i]);
                echo(" ");
                i++;
            }
            printf("\n");
            break;  }


        else if (check != 2 && check != 3 && check != 4)
        {    check = 1;  }
        //------------------------------------------------------------------------------------------------------check for background processing + removing "&"

        if(strcmp((instr_ptr->tokens)[a], "&") == 0)
        {

            if (a == 0)     //case 1: "&"" is before the instructions and is ignored
            {

                //ignore
            }
            if ((a != 0)  && ((instr_ptr->tokens)[a+1] != NULL))        //case 2: & is in the middle and the syntax is invalid
            {

                printf("%s\n", "Invalid Syntax");
            }
            else if ((instr_ptr->tokens)[a+1] == NULL)  //case 3: & is at the end of instructions and backgroudn processing happends
            {

                bGround = -1;
            }

        }


    }//end of for loop

    //printf("%d  Path res is : \n", pathRes(instr_ptr));
    // printf("%d  check is  : \n", check);

    /*if (pathRes(isntr_ptr) == 1)
    {
        printf("%s\n", "File not found");
    }*/

    //---------------------------------------------------------------------------------------------------------executing regular, redirection, and piping

    if(check == 1 && pathRes(instr_ptr) == 1 ) // check if no '>', '<', or ,'|' in any token
    {
        my_execute(instr_ptr->tokens, bGround, tem);
    }
    else if(check == 2) // && pathRes(instr_ptr) == 1) // check for '<', '>' to perform i/o
    {
        if (instr_ptr->numTokens < 4)
        {
            printf("%s\n", "Invalid Syntax");

        }
        else if(pathRes(instr_ptr) == 1)
        {   IOredirection(instr_ptr, bGround, tem); }


    }

    else if((check == 3)) // && pathRes(instr_ptr) == 1 ) // check for '|' to perform piping
    {
        int i;
        for (i = 0; i < instr_ptr->numTokens-1; i++)
        {
            if ((instr_ptr->tokens)[i] != NULL)
            {
                if (strcmp((instr_ptr->tokens)[i], "|") == 0)
                {
                    numofpipes++;
                }
            }
        }// end of for loop

        if (numofpipes == 1)
        {
            if(instr_ptr->numTokens < 4) printf("%s\n", "Invalid Syntax");
            else if(pathRes(instr_ptr) == 1) singlepipe(instr_ptr, bGround);
        }
        if (numofpipes == 2)
        {
            if(instr_ptr->numTokens < 6) printf("%s\n", "Invalid Syntax");
            else if(pathRes(instr_ptr) == 1)doublepipe(instr_ptr,bGround, tem);
        }

    }
}

int isFile(const char * path)   //checks if cstring is a file in the machine
{
    if(access(path, F_OK) == -1)    //uses access to see if file exists, if so return 1. else return 0
        return 0;

    return 1;
}

int isDirectory(const char * path)  //checks if cstring is a directory
{
    struct stat stats;

    stat(path, &stats);

    if(S_ISDIR(stats.st_mode))      //if directory return 1, else return 0
        return 1;

    return 0;
}
