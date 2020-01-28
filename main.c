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
void singlepipe( instruction* instr_ptr, int bGround, char* cmd);
void doublepipe( instruction* instr_ptr, int bGround, char* cmd);

//FUNCTIONS FUNCTION CALLS
void enVar(instruction* instr_ptr);
void printPrompt();
int shortRes(instruction* instr_ptr);
int pathRes(instruction* instr_ptr);
void func(instruction * instr_ptr,int commandCounter, char* tem);
int isFile(const char * path);
int isDirectory(const char * path);



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
            printTokens(&instr);

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

//IO&PIPING FUNCTIONS
void IOredirection( instruction* instr_ptr, int bGround, char* cmd)
{

    char * inputfile;
    char * outputfile;
    bool input = false;
    bool output = false;

    //char * cmd[2];

    //strcpy(cmd[0], (instr_ptr->tokens)[0]);
    // cmd[1] = NULL;
    // printf("Test 6.7\n");
    int i;
    //  printf("Print function round 2:  \n");

    // printTokens(&instr);

    for (i = 0; i < instr_ptr->numTokens-1; i++)
    {
        //printf(" current token is : %s \n", (instr_ptr->tokens)[i]);

        if ((instr_ptr->tokens)[i] != NULL)
        {


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
        my_execute(instr_ptr->tokens, bGround, cmd);
        exit(1);



    }// end of pid if statement

    else    //parent function
    {
        //waitpid(0);
    }

    if (input == true)  {   free(inputfile); }
    if (output == true) {   free (outputfile); }

}//end of function


void singlepipe(instruction * instr_ptr, int bGround, char* cmd)
{
    int status;
    instruction cmd1;
    instruction cmd2;
    cmd1.tokens = NULL;
    cmd2.numTokens = 0;
    cmd1.tokens = NULL;
    cmd2.numTokens = 0;
    char * temp1;
    char * temp2;
    char ls[] = "/bin/ls";
    printf("test print 1 \n");

    int i;
    for (i = 0; i < instr_ptr->numTokens; i++)
    {
        if ((instr_ptr->tokens)[i] != NULL)
        {
            if (strcmp((instr_ptr->tokens)[i],"|") == 0)
            {

                temp1 = (char *) calloc(strlen((instr_ptr->tokens)[i-1]), sizeof(char));
                temp2 = (char *) calloc(strlen((instr_ptr->tokens)[i+1]), sizeof(char));

                addToken(&cmd1, (instr_ptr->tokens)[i-1]);
                addNull(&cmd1);
                pathRes(&cmd1);
                printTokens(&cmd1);

                addToken(&cmd2, (instr_ptr->tokens)[i+1]);
                addNull(&cmd2);
                pathRes(&cmd2);
                printTokens(&cmd2);


                printf("test print 2\n");

            }
        }
    }
    printf("test print \n");

    int fd[2];
    pid_t p2;

    pid_t p1 = fork();
    if (p1 == 0)        // child 1 for output redirection
    {
        printf("fd[0] %d \n", fd[0]);
        printf("fd[1] %d \n", fd[1]);

        if (pipe(fd) < 0)
        {  printf("Pipe could not be initialized. \n");
        }

        printf("Piped: fd[0] %d \n", fd[0]);
        printf("fd[1] %d \n", fd[1]);

        p2 = fork();         //child 2 inside parent
        if (p2 == 0)        //child 2 for input redirection
        {
            printf("fd[1] %d \n", fd[1]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            if (execv(cmd1.tokens[0], cmd1.tokens) == -1)
            {
                printf("%s", cmd1.tokens[0]);
            }
            exit(0);
        }
        else
        {
            waitpid(p2, &status, 0);
            //wait(NULL);

            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            if (execv(cmd2.tokens[0], cmd2.tokens) < 0)
            {
                printf("Command 2 a big could not be executed. \n" );
            }
            exit(0);
        }

    }
    else                 //parent
    {
        waitpid(p1, &status, 0);
        waitpid(p2, &status, 0);
        //wait(NULL);
        //wait(NULL);
    }
    free(temp1);
    free(temp2);

    // exit(1);

}//end of single pipe

void doublepipe(instruction * instr_ptr, int bGround, char* cmd)
{
    char * cmd1[2];
    char * cmd2[2];
    char * cmd3[2];
    cmd1[1] = NULL;
    cmd2[1] = NULL;
    cmd3[1] = NULL;
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
                // waitpid(0);
                // waitpid(0);
                // waitpid(0);

            }
            else             //child #3
            {
                close(0);
                dup(5);
                close(3); close(4); close(5); close(6);
                my_execute(cmd3, bGround, cmd);
            }
        }
        else             //child #2
        {
            close(0); dup(3); close(1); dup(6); close(3); close(4); close(5);
            my_execute(cmd2, bGround, cmd);
        }

    }
    else                //child #1
    {
        close(1); dup(4); close(3); close(4); close(5); close(6);
        my_execute(cmd1, bGround, cmd);
    }


}    //end of function

//FUNCTIONS.c CALLS
void printPrompt()
{
    printf("%s", getenv("USER"));
    printf("@");
    printf("%s:", getenv("MACHINE"));
    printf("%s>", getenv("PWD"));
}

void enVar(instruction* instr_ptr)
{
    int a;

    for(a = 0; a < instr_ptr->numTokens; a++)
    {
        if((instr_ptr->tokens)[a] != NULL)
        {
            if(strcmp((instr_ptr->tokens)[a], "$HOME") == 0)
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("HOME")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("HOME"));
            }
            else if(strcmp((instr_ptr->tokens)[a], "$PATH") == 0)
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("PATH")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("PATH"));
            }
            else if(strcmp((instr_ptr->tokens)[a], "$USER") == 0)
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("USER")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("USER"));
            }
            else if(strcmp((instr_ptr->tokens)[a], "$SHELL") == 0)
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("SHELL")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("SHELL"));
            }
            else if(strcmp((instr_ptr->tokens)[a], "$PWD") == 0)
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("PWD")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("PWD"));
            }
        }
    }
}

int shortRes(instruction* instr_ptr)
{
    int a, c, d, e, f, g, h, i;
    int chk;
    int count = 0;
    int checkDelim = 0;
    int t2Check = 0;
    int t3Check = 0;
    int t4Check = 0;

    char * t1 = NULL;
    char * t2 = NULL;
    char * t3 = NULL;
    char * t4 = NULL;
    char * delim = NULL;

    for(a = 0; a < instr_ptr->numTokens; a++)
    {
        if((instr_ptr->tokens)[a] != NULL)
        {
            if(strcmp((instr_ptr->tokens)[a], ".") == 0)
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("PWD")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("PWD"));
            }
            else if(strcmp((instr_ptr->tokens)[a], "..") == 0)
            {
                for(c = strlen(getenv("PWD"))-1; c >= 0; c--)
                {
                    if(getenv("PWD")[c] == '/')
                    {
                        chk = c;
                        break;
                    }
                }

                (instr_ptr->tokens)[a] = (char *) calloc(chk, sizeof(char));

                for(d = 0; d < chk; d++)
                {
                    ((instr_ptr->tokens)[a])[d] = getenv("PWD")[d];
                }
            }
            else if(strcmp((instr_ptr->tokens)[a], "~") == 0)
            {
                (instr_ptr->tokens)[a] = (char *) calloc(strlen(getenv("HOME")), sizeof(char));
                strcpy((instr_ptr->tokens)[a], getenv("HOME"));
            }
            else if(strpbrk((instr_ptr->tokens)[a], "/") != NULL)
            {
                if(((instr_ptr->tokens)[a])[0] == '~')
                {
                    t1  = (char *) calloc(strlen(getenv("HOME")) + strlen((instr_ptr->tokens)[a]) - 1, sizeof(char));

                    for(e = 0; e < strlen(getenv("HOME")); e++)
                    {
                        t1[e] = getenv("HOME")[e];
                    }

                    for(f = strlen(getenv("HOME")); f < strlen(getenv("HOME"))+strlen((instr_ptr->tokens)[a])-1; f++)
                    {
                        t1[f] = ((instr_ptr->tokens)[a])[f-strlen(getenv("HOME"))+1];
                    }

                    (instr_ptr->tokens)[a]  = (char *) calloc(strlen(getenv("HOME")) + strlen((instr_ptr->tokens)[a]) - 1, sizeof(char));
                    strcpy((instr_ptr->tokens)[a], t1);

                    free(t1);
                }

                delim = strtok((instr_ptr->tokens)[a], "/");

                while(delim != NULL)
                {
                    if(strcmp(delim, ".") == 0)
                    {
                        checkDelim = 1;

                        delim = (char *) calloc(strlen(getenv("PWD")), sizeof(char));
                        strcpy(delim, getenv("PWD"));

                        if(t2 != NULL)
                        {
                            t3Check = 1;
                            t3 = (char *) calloc(strlen(t2), sizeof(char));

                            for(g = 0; g < strlen(t2); g++)
                                t3[g] = t2[g];


                            t2Check = 1;
                            t2 = (char *) calloc(strlen(delim)+strlen(t3)+1, sizeof(char));

                            strcat(t2, t3);
                            strcat(t2, delim);
                        }
                        else
                        {
                            t2Check = 1;
                            t2 = (char *) calloc(strlen(delim)+1, sizeof(char));
                            strcat(t2, delim);
                        }
                    }
                    else if(strcmp(delim, "..") == 0)
                    {
                        if(count != 0)
                        {
                            h = strlen(t2);
                            while(t2[h] != '/')
                                h--;

                            t4Check = 1;
                            t4 = (char *) calloc(strlen(t2), sizeof(char));
                            strcpy(t4, t2);

                            t2Check = 1;
                            t2 = (char *) calloc(h+1, sizeof(char));

                            if(t4[0] != '/')
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

                            if(strlen(t2) == 0)
                                strcat(t2, "/");

                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if(t2 != NULL)
                        {
                            t3Check = 1;
                            t3 = (char *) calloc(strlen(t2), sizeof(char));

                            for(g = 0; g < strlen(t2); g++)
                                t3[g] = t2[g];

                            t2Check = 1;

                            t2 = (char *) calloc(strlen(delim)+strlen(t3)+1, sizeof(char));

                            strcat(t2, t3);

                            if(delim[0] != '/')
                                strcat(t2, "/");

                            strcat(t2, delim);
                        }
                        else
                        {
                            t2 = (char *) calloc(strlen(delim)+1, sizeof(char));

                            if(delim[0] != '/')
                                strcat(t2, "/");

                            strcat(t2, delim);
                        }
                    }

                    delim = strtok(NULL, "/");
                    count++;
                }

                if(count != 0)
                {
                    (instr_ptr->tokens)[a] = (char *) calloc(strlen(t2), sizeof(char));
                    strcpy((instr_ptr->tokens)[a], t2);
                }

                if(t2Check == 1) free(t2); t2 = NULL;
                if(t3Check == 1) free(t3); t3 = NULL;
                if(t4Check == 1) free(t4); t4 = NULL;
                if(checkDelim == 1) free(delim); delim = NULL;

                if(isFile((instr_ptr->tokens)[a]) == 0 && isDirectory((instr_ptr->tokens)[a]) == 0)
                    return 0;

            }


        }
    }

    return 1;
}

int pathRes(instruction* instr_ptr)
{
    int a;

    char * path1;
    char * delim;
    char * t1;

    for(a = 0; a <instr_ptr->numTokens; a++)
    {
        if((instr_ptr->tokens)[a] != NULL && strpbrk((instr_ptr->tokens)[a], "/") == NULL && strcmp((instr_ptr->tokens)[a], "cd") != 0 &&
           strcmp((instr_ptr->tokens)[a], "jobs") != 0 && strcmp((instr_ptr->tokens)[a], "exit") != 0 && strcmp((instr_ptr->tokens)[a], "echo") != 0)
        {
            path1 = (char *) calloc(strlen(getenv("PATH")), sizeof(char));
            strcpy(path1, getenv("PATH"));

            delim = strtok(path1, ":");

            while(delim != NULL)
            {
                t1 = (char *) calloc(strlen(delim)+strlen((instr_ptr->tokens)[a])+1, sizeof(char));
                strcat(t1, delim);
                strcat(t1, "/");
                strcat(t1, (instr_ptr->tokens)[a]);

                if(access(t1, F_OK) != -1)
                {
                    (instr_ptr->tokens)[a] = (char *) calloc(strlen(t1), sizeof(char));
                    strcpy((instr_ptr->tokens)[a], t1);
                    return 1;
                }

                delim = strtok(NULL, ":");
            }

            if(path1 != NULL) free(path1);
            if(t1 != NULL) free(t1);
        }
    }
    return 0;
}

void func(instruction * instr_ptr, int commandCounter, char* tem)
{
    int a;
    int check = 0; // 1 ==> execute, 2 ==> I/O, 3 ==> piping
    int bGround = 0;
    int valid = 0;
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
        {    exitShell(instr_ptr, commandCounter); break;   }

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
        {
            int i = 1;
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

                bGround = 1;
            }

        }


    }//end of for loop

    //printf("%d  Path res is : \n", pathRes(instr_ptr));
    // printf("%d  check is  : \n", check);

    //---------------------------------------------------------------------------------------------------------executing regular, redirection, and piping

    if(check == 1 && pathRes(instr_ptr) == 1 ) // check if no '>', '<', or ,'|' in any token
    {
        my_execute(instr_ptr->tokens, bGround, tem);
    }
    else if(check == 2 && pathRes(instr_ptr) == 1) // check for '<', '>' to perform i/o
    {

        int i;
        for (i = 0; i < instr_ptr->numTokens-1; i++)
        {

            if ((instr_ptr->tokens)[i] != NULL)

            {
                if ((strcmp((instr_ptr->tokens)[i], "<") == 0) && (i == 0))
                {
                    printf("%s\n", "Invalid Syntax");
                    break;
                }
                else if ((strcmp((instr_ptr->tokens)[i], ">") == 0) && (i == 0))
                {
                    printf("%s\n", "Invalid Syntax");
                    break;
                }
                    //else if ((strcmp((instr_ptr->tokens)[i], "<")== 0) && (strcmp((instr_ptr->tokens)[i+1], NULL) == 0))
                else if ((strcmp((instr_ptr->tokens)[i], "<")== 0) && (i == (instr_ptr->numTokens-1)))
                {
                    printf("%s\n", "Invalid Syntax");
                    break;
                }
                    //else if ((strcmp((instr_ptr->tokens)[i], ">")== 0) && (strcmp((instr_ptr->tokens)[i+1], NULL) == 0))
                else if ((strcmp((instr_ptr->tokens)[i], ">")== 0) && (i == (instr_ptr->numTokens-1)))

                {
                    printf("%s\n", "Invalid Syntax");
                    break;
                }
                else {valid = 1; }

            }


        }// end of for loop

        if (valid == 1)
        {
            IOredirection(instr_ptr, bGround, tem);
        }
    }

    else if((check == 3) && pathRes(instr_ptr) == 1 ) // check for '|' to perform piping
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

                if ((strcmp((instr_ptr->tokens)[i], "|") == 0) && (i == 0))
                {
                    printf("%s\n", "Invalid Syntax");
                    break;
                }
                    /*  else if ((strcmp((instr_ptr->tokens)[i], "|") == 0) && ((instr_ptr->tokens)[i+1] == NULL))
                      {
                          printf("%s\n", "Invalid Syntax");
                          break;
                      }*/

                else {valid = 1;}

            }


        }// end of for loop
        if (valid == 1)
        {
            if (numofpipes == 1)
            {  singlepipe(instr_ptr, bGround, tem);	}
            if (numofpipes == 2)
            {	doublepipe(instr_ptr,bGround, tem); }

        }
    }

}

int isFile(const char * path)
{
    if(access(path, F_OK) == -1)
        return 0;

    return 1;
}

int isDirectory(const char * path)
{
    struct stat stats;

    stat(path, &stats);

    if(S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}
