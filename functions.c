#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>

void enVar(instruction* instr_ptr);
void printPrompt();
void shortRes(instruction* instr_ptr); 
int pathRes(instruction* instr_ptr);

int pathRes2(char * tmp);
void func(instruction * instr_ptr);

void printPrompt()
{
	printf("%s", getenv("USER"));                                                                                   
    printf("@");
    printf("%s:", getenv("MACHINE"));
    printf("%s>", getenv("PWD"));
}

void shortRes(instruction* instr_ptr)
{
    int a, b, c, d, e, f, g, h, i;
    int chk;
    int last;
    int count = 0;
    int checkDelim = 0;
    int t2Check = 0;
    int t3Check = 0;
    int t4Check = 0;
    
    char * prevDir = getenv("PWD");
    
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
            }
        }
    }
}

int pathRes(instruction* instr_ptr)
{
	int a;
	
    instruction temp;
    
	char * path1;
	char * delim;
	char * t1;
	
	if(strcmp((instr_ptr->tokens)[0], "cd") != 0 && strcmp((instr_ptr->tokens)[0], "exit") != 0 
	&& strcmp((instr_ptr->tokens)[0], "jobs") != 0 && strcmp((instr_ptr->tokens)[0], "echo") != 0) 
	{
		path1 = (char *) calloc(strlen(getenv("PATH")), sizeof(char));
		strcpy(path1, getenv("PATH"));

		delim = strtok(path1, ":");

		while(delim != NULL)
		{
			t1 = (char *) calloc(strlen(delim)+strlen((instr_ptr->tokens)[0])+1, sizeof(char));	
			strcat(t1, delim);
			strcat(t1, "/");
			strcat(t1, (instr_ptr->tokens)[0]);
            
/*            addToken(&temp, t1);
            shortRes(&temp);
            
            printf("%s\n", (temp.tokens)[0]);
*/
			if(access(t1, F_OK) != -1)
			{
                printf("%s\n", t1);
                (instr_ptr->tokens)[0] = (char *) calloc(strlen(t1), sizeof(char));
                strcpy((instr_ptr->tokens)[0], t1);
                return 1;
			}	
			
			delim = strtok(NULL, ":");
		}
        
        if(path1 != NULL) free(path1);
        if(t1 != NULL) free(t1);
	}
    
    return 0;
}

int pathRes2(char * tmp)
{
    int a;
    
    char * path1;
    char * delim;
    char * t1;
    
    path1 = (char *) calloc(strlen(getenv("PATH")), sizeof(char));
    strcpy(path1, getenv("PATH"));
    
    delim = strtok(path1, ":");
    
    while(delim != NULL)
    {
        t1 = (char *) calloc(strlen(delim)+strlen(tmp)+1, sizeof(char));
        strcat(t1, delim);
        strcat(t1, "/");
        strcat(t1, tmp);
        
        if(access(t1, F_OK) != -1)
        {
            *tmp = (char *) calloc(strlen(t1), sizeof(char));
            strcpy(tmp, t1);
            
            printf("%s\n", tmp);
            
            if(t1 != NULL) free(t1);
            if(path1 != NULL) free(path1);
            
            return 1;
        }
        
        delim = strtok(NULL, ":");
    }
    
    if(path1 != NULL) free(path1);
    if(t1 != NULL) free(t1);
    
    return 0;
}

void func(instruction * instr_ptr)
{
    int a;
    int check = 0; // 1 ==> execute, 2 ==> I/O, 3 ==> piping, 4 ==> builtins
    int bGround = 0;
	int valid = 0; 
	int numofpipes = 0; 
    
    shortRes(instr_ptr);
    enVar(instr_ptr);
 
    for(a = 0; a < instr_ptr->numTokens; a++)
    {
	    
        if(strcmp((instr_ptr->tokens)[a], "<") == 0 || strcmp((instr_ptr->tokens)[a], ">") == 0)
	{	check = 2;	break;	}
        else if(strcmp((instr_ptr->tokens)[a], "|") == 0)
	{	check = 3;	break;	}
        else if(strcmp((instr_ptr->tokens)[a], "cd") != 0 && strcmp((instr_ptr->tokens)[a], "exit") != 0
                && strcmp((instr_ptr->tokens)[a], "jobs") != 0 && strcmp((instr_ptr->tokens)[a], "echo") != 0)
	{	check = 4;	break;	}
        else	(check != 2 && check != 3 && check != 4)
	{	 check = 1;	}
        
        if(strcmp((instr_ptr->tokens)[a], "&") == 0)
	{	
		if ((strcmp((instr_ptr->tokens)[0], "&") == 0))		//case 1: & is before the instructions and is ignored 
		{
			//ignore 
		}
		if ((a != 0)  && ((instr_ptr->tokens)[a+1] != NULL))		//case 2: & is in the middle and the syntax is invalid 
		{
			printf("%s\n", "Invalid Syntax");
		}
		else if ((instr_ptr->tokens)[a+1] == NULL)	//case 3: & is at the end of instructions and backgroudn processing happends 
		{
			bGround = 1;
		}
	}
    }
 
    if(pathRes(instr_ptr) == 1 && check == 1) // check if no '>', '<', or ,'|' in any token
    {
 		my_execute(instr_ptr, 1, bGround);
    }
    else if(pathRes(instr_ptr) == 1 && check == 2) // check for '<', '>' to perform i/o
    {
	 for (i = 0; i < instr_ptr->numTokens; i++) 
    	{
       	 	if ((instr_ptr->tokens)[i] != NULL)
		{
			if (((instr_ptr->tokens)[i] == "<") && (i == 0))
			{
			printf("%s\n", "Invalid Syntax");
				break; 
			}
			else if (((instr_ptr->tokens)[i] == ">") && (i == 0))
			{
			printf("%s\n", "Invalid Syntax");
				break; 				
			}
			else if (((instr_ptr->tokens)[i] == "<") && ((instr_ptr->tokens)[i+1] == NULL))
			{
			printf("%s\n", "Invalid Syntax");
				break; 	
			}
			else if (((instr_ptr->tokens)[i] == ">") && ((instr_ptr->tokens)[i+1] == NULL))
			{
			printf("%s\n", "Invalid Syntax");
				break; 	
			}
			else {valid = 1}; 
		
        	}

    	}// end of for loop 
	    if (valid == 1)
	    {
		    IOredirection(instr_ptr, bGround); 
	    }
    }
    else if(check == 3) // check for '|' to perform piping
    {
        for (i = 0; i < instr_ptr->numTokens; i++) 
    	{
       	 	if ((instr_ptr->tokens)[i] != NULL)
		{
			if ((instr_ptr->tokens)[i] == "|")
			{
				numofpipes++; 
			}
			
			if (((instr_ptr->tokens)[i] == "|") && (i == 0))
			{
			printf("%s\n", "Invalid Syntax");
				break; 
			}
			else if (((instr_ptr->tokens)[i] == "<") && ((instr_ptr->tokens)[i+1] == NULL))
			{
			printf("%s\n", "Invalid Syntax");
				break; 	
			}
			
			else {valid = 1}; 
		
        	}

    	}// end of for loop 
	    if (valid == 1)
	    {
		    if (numofpipes == 1)
		    {  singlepipe(isntr_ptr, bGround);	}
		    if (numofpipes == 2)
		    {	doublepipe(instr_ptr,bGround); }
			    
	    }
    }
    else if(check == 4) // check for builtins
    {
 	
    }
 
}
