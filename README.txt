# COP4610_project_1
README

Team Members and Responsibilities:
  Conrad Horn:
  • Prompt
  • Environmental Variables
  • Path Resolution
  • Shortcut Resuolution 
  
  Justin Bomwell:
  • I/O Redirection
  • Pipes
  • Implementation of all functions in main using func()

  Scott Forn:
  • Execution
  • Background Processes
  • Built Ins
  
  
  
Tar Archive Contents:
  main.c:
  • Prints prompt
  • Parses instructions
  • Shorcut Resolution function
  • Resolves Environmental Variables
  • Path Resolution function
  • Input/Output Redirection function
  • Piping functions
  • Function to implement the other functions and runs shell in main
  
  executionProcessing.h:
  • Declare Normal Execute function
  • Declare Background Processesing Execution
  • Declare Built Ins
  
  executionProcessing.c:
  • Define Normal Execute function
  • Define Background Processesing Execution
  • Define Built Ins
  
  makefile:
  • compile main.c, executionProcessing.h & executionProcessing.c
  
  
  
Compilation Instructions:
  • To compile the project, move to the working directory that contains our opened .tar file
  • Type "make" into the command line 
◦ Any completed extra credit must be documented in the README to receive credit


Function Descriptions: 

0. void example function (int x)

  Example paragraph. Write your stuff here. Include what file to find your function on.  Also include when each file is called, its use, what it takes in and returns, and why you di each part. Try not to make multiple paragraphs and to keep everything within one indentation like i am doing here.  

1. void addToken(instruction* instr_ptr, char* tok)

2. void printTokens(instruction* instr_ptr)

3. void clearInstruction(instruction* instr_ptr)

4. void addNull(instruction* instr_ptr)

5. void IOredirection( instruction* instr_ptr, int bGround)

void singlepipe( instruction* instr_ptr, int bGround)

void doublepipe( instruction* instr_ptr, int bGround)

void enVar(instruction* instr_ptr)

void printPrompt()

int shortRes(instruction* instr_ptr)

int pathRes(instruction* instr_ptr)

void func(instruction * instr_ptr,int commandCounter)

int isFile(const char * path)

int isDirectory(const char * path)


Known Bugs/Unfinished Portions: 
-GUYS MAKE SURE TO INFORM THEM OF 1. WHEN IT HAPPENDS.  2. WHY IT HAPPENDS.  3.  HOW WE ATTEMPTED TO FIX IT
1. 
2.
3.
4.
5.
6.
7.
8.
9.



Special Considerations: 





























Currently Known Bugs:
Your README will contain a list of all known bugs in your program. For each bug, include a brief
description of when it is occurring (compile, link, or runtime), when the bug first showed up, the
symptoms the bug is giving off, and how you attempted to fix it. Undocumented bugs will incur a
greater penalty.
Demonstrating that you understand why a program has a bug, even if you are unsure of how to fix it,
shows that you are thorough. An undocumented bug shows that you do not understand the program, or
that you tried to hide a flaw. Undocumented bugs only slow down future development.
Known bugs and unfinished portions of the project
• Special considerations or anything I should know when grading your solution
◦ Any completed extra credit must be documented in the README to receive credit



GIT Commit Log
• Submit a screenshot of the commits made to the repository
• All groups must use a private GIT repository
• All group members must actively commit his/her contributions to the repository
• If the commit log / division of labor indicates a group member does not contribute equally,
deductions will be made for that group member
