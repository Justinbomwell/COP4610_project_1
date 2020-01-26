//
// Created by Scott Forn on 1/15/20.
//

#ifndef PROJECT_1_MYSHELL_SHORTCUTANDPROCESSRESOLUTION_H
#define PROJECT_1_MYSHELL_SHORTCUTANDPROCESSRESOLUTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>



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

struct ProcessElement
{
    int pid_1;
    int pid_2;  //If redirection, otherwise = -1
    char* cmd;  //process command
};

// global process queue
#define SIZEOFQUEUE 256
struct ProcessElement* pEntry[SIZEOFQUEUE];

void backgroundInit();
void pushProcess(struct ProcessElement *p);
void rmProcess(int index);
void checkQueue();
struct ProcessElement* mkProcess(int pid_1, int pid_2, char *cmd);
int checkProcess(struct ProcessElement *p);
void deleteEntry(struct ProcessElement *p);
void exitCommandProcess();
void printProcessStart(const struct ProcessElement *p, int pos);


/*
  ___                 _   _
 | __|_ _____ __ _  _| |_(_)___ _ _
 | _|\ \ / -_) _| || |  _| / _ \ ' \
 |___/_\_\___\__|\_,_|\__|_\___/_||_|

 */

char* PathEnvBuildPath(char* str);
void my_execute(char **cmd, int isValid, int pipes, int fileRedirects);
void my_executeNormal(char **cmd, int background);
char** my_executePipes(char **cmd, int background, int symbol);
char** my_executeInput(char **cmd, int background, int symbol);
char** my_executeOutput(char **cmd, int background, int symbol);
char* DeleteCharacter(char* line, size_t start, size_t end);
/*
  ___      _ _ _     _
 | _ )_  _(_) | |_  (_)_ _  ___
 | _ \ || | | |  _| | | ' \(_-<
 |___/\_,_|_|_|\__| |_|_||_/__/

 */
void exitShell(char **cmd, int commandCounter);
void changeDirectory(const char* dir);
void echo(char* string);
void jobs(const struct ProcessElement *p, int pos);

/*
 _   _ _   _ _ _ _   _
| | | | |_(_) (_) |_(_) ___  ___
| | | | __| | | | __| |/ _ \/ __|
| |_| | |_| | | | |_| |  __/\__ \
 \___/ \__|_|_|_|\__|_|\___||___/
*/

void freedom(char **theArray);
int contains(char** cmd, const char* theString);
int countOf(char** cmd, const char* theString);


#endif //PROJECT_1_MYSHELL_SHORTCUTANDPROCESSRESOLUTION_H
