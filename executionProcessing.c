//
// Created by Scott Forn on 1/15/20.
//

#include "executionProcessing.h"

/*
  ___          _                             _
 | _ ) __ _ __| |____ _ _ _ ___ _  _ _ _  __| |
 | _ \/ _` / _| / / _` | '_/ _ \ || | ' \/ _` |
 |___/\__,_\__|_\_\__, |_| \___/\_,_|_||_\__,_|
    | _ \_ _ ___  |___/_ _____(_)_ _  __ _
    |  _/ '_/ _ \/ _/ -_|_-<_-< | ' \/ _` |
    |_| |_| \___/\__\___/__/__/_|_||_\__, |
                                     |___/
 */

struct ProcessElement* pEntry[];

//Initialize the background queue (size 256 for processing space)
void backgroundInit() 
{
    int i;
    for (i = 0; i < SIZEOFQUEUE; i++)
    {
        pEntry[i] = NULL;
    }
}

//Adds the newest backgrond command to the end of the list
void pushProcess(struct ProcessElement *p)
{
    int i;

    for (i = 0; i < SIZEOFQUEUE; i++)
    {
        if (pEntry[i] == NULL)
        {
            pEntry[i] = p;
            //Prints the first process that's being executed.
            printProcessStart(pEntry[i], i);
            break;
        }
    }
}

void rmProcess(int index)
{
    //Finds index inside of the queue.
    if (index >= 0 && index < SIZEOFQUEUE)
    {
        if (pEntry[index] != NULL)
        {
            if (pEntry[index] != NULL)
            {
                printf("[%i]+\t[%s]\n", index, pEntry[index]->cmd);
            }
            //Call delete function
            deleteEntry(pEntry[index]);
            //Null out pEntry
            pEntry[index] = NULL;
        }
    }
}

void checkQueue()     //Checks the status of queue i.e. empty or not
{
    int i;
    for (i = 0; i < SIZEOFQUEUE; i++)
    {
        if (pEntry[i] != NULL)
        {
            if (checkProcess(pEntry[i]))
            {
                rmProcess(i);
            }
        }
    }
}

//Creates a new process
struct ProcessElement* mkProcess(int pid_1, int pid_2, char *cmd)
{
    //Allocate memory and create a new element
    struct ProcessElement* newElement = (struct ProcessElement*)calloc(1, sizeof(struct ProcessElement));
    newElement->pid_1 = pid_1;    //Assign newElement data to data that was passed in.
    newElement->pid_2 = pid_2;    //See above.
    newElement->cmd = (char*)calloc(strlen(cmd)+1,sizeof(char));    //Memeory alloc
    strcpy(newElement->cmd, cmd);
    return newElement;
}


int checkProcess(struct ProcessElement *p)    //Checks the staus of a process i.e. completed or not
{
    int currentState;
    int pidRetrun;
    if (p->pid_2 != -1)
    {
        //pid process from lecture slides
        pidRetrun = waitpid(p->pid_2, &currentState, WNOHANG);
        if (pidRetrun == 0)
        {
            return 0;
        }
        else if (pidRetrun == -1)
        {
            printf("Error in child process: %s\n", p->cmd);
        }

    }
    pidRetrun = waitpid(p->pid_1, &currentState, WNOHANG);
    if (pidRetrun == 0)
    {
        return 0;
    }
    else if (pidRetrun == -1)
    {
        printf("Error in child process: %s\n", p->cmd);
    }
    else
    {
        return 1;
    }
    return 1;
}

void deleteEntry(struct ProcessElement *p)
{
    free(p->cmd);   //free data
    free(p);        //free the struct
}

void exitCommandProcess()
{
    int running = 1;
    while(running == 1)
    {
        int queueStep = 0;
        checkQueue();     //Check the staus of the queue
        for (queueStep = 0; queueStep < SIZEOFQUEUE; queueStep++)
        {
            if (pEntry[queueStep] != NULL)    //If the program is running let it run.
            {
                running = 1;
                break;
            }
        }
        if (queueStep == SIZEOFQUEUE)     //If the background processes are done, tell it
        {
            running = 0;
        }
    }
}

//Print Background Array position \t pid asscociated with the command. 
void printProcessStart(const struct ProcessElement *p, int pos)
{
    if (p != NULL)
    {
        if (p->pid_2 != -1)
        {
            printf("[%i]\t[%i][%i]\n", pos, p->pid_1, p->pid_2);
        }
        else
        {
            printf("[%i]\t[%i]\n", pos, p->pid_1);
        }
    }
}



/*
  ___                 _   _
 | __|_ _____ __ _  _| |_(_)___ _ _
 | _|\ \ / -_) _| || |  _| / _ \ ' \
 |___/_\_\___\__|\_,_|\__|_\___/_||_|
 */

void my_execute(char **argument, int background, char* cmd)
{
    int backgroundCheck = background;//contains(cmd, "&");
    /*int inputSymbol = contains(cmd, "<");
    int outputSymbol = contains(cmd, ">");
    int pipe_count = countOf(cmd, "|");*/
    int status;
    pid_t pid = fork();

    if (pid == -1)
    {
        //Error
        printf("Fork Trouble");
        exit(1);
    }
    else if (pid == 0)
    {
        //Child
        execv(argument[0], argument);
        printf("Problem executing %s\n", argument[0]);

        //
        exit(1);
    }
    else
    {
        if (backgroundCheck == -1)
        {
            waitpid(pid, &status, WNOHANG);
            pushProcess(mkProcess(pid, -1, cmd));
        }
        else
        {
            //Parent
            waitpid(pid, &status, 0);
        }
    }
    
}


/*
 ___      _ _ _     _
| _ )_  _(_) | |_  (_)_ _  ___
| _ \ || | | |  _| | | ' \(_-<
|___/\_,_|_|_|\__| |_|_||_/__/
*/

void exitShell(char **cmd, int commandCounter)
{
    exitCommandProcess();   //Wait until background is done.
    //freedom(cmd);
    printf("\nExiting Now!\n    Commands Issued: %u\n", commandCounter);
    exit(0);    //Exit the shell
}

void changeDirectory(const char* dir)   //Take in the modified directory string
{
    int newPath = chdir(dir);
    if (newPath == 0)   //If chdir returns 0, setenv
    {
        setenv("PWD", dir, 1);    //make "PWD" the current directory.
        //printf("%s>", getenv("PWD"));
    }
    else
    {
        printf("No directory found\n");
    }
}

void echo(char *string)
{

    if(string[0] != '$')  //If first is not $, just print the string.
    {
        printf("%s",string);
    }
    else if(string[0] == '$')  //If $USER
    {
        if(getenv(string))
        {
            int i = 1;
            while(string[i] != '\0')  //Print chars EXCEPT $
            {
                printf("%c", string[i]);
                i++;
            }
        }
        else                //Error
        {
            printf("Error. command does not exist.");
        }
    }
}


//Print active jobs
void jobs(const struct ProcessElement *p, int pos)//char* string)
{
    if (p != NULL)    //If not empty
    {
        if (p->pid_2 != -1)
        {
            printf("[%i]+ [%i][%i]\n", pos, p->pid_1, p->pid_2);
        }
        else
        {
            printf("[%i]+ [%i] [%s]\n", pos, p->pid_1, p->cmd);
        }
    }
    //int CMDPosition, CMDPID;
    //printf("[%d]+ %d %s", CMDPosition,CMDPID,string);
}

/*
 _   _ _   _ _ _ _   _
| | | | |_(_) (_) |_(_) ___  ___
| | | | __| | | | __| |/ _ \/ __|
| |_| | |_| | | | |_| |  __/\__ \
 \___/ \__|_|_|_|\__|_|\___||___/
*/


void freedom(char **theArray)     //Free up all memory
{
    int itr = 0;

    while (theArray[itr] != NULL)
    {
        free(theArray[itr]);
        itr++;
    }
    //free(theArray);
}
