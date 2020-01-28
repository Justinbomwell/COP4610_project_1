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
  - Take in struct instruction and c-string command from the command line
  - Make space for an additional token in the instruction
  - place the c-string tok into the new token that was allocated

2. void printTokens(instruction* instr_ptr)
  - print each individual token on a new line from the instruction 

3. void clearInstruction(instruction* instr_ptr)
  - Go through the instructions individual tokens and free their memory
  - Free the actual instruction 
  - Set the instruction to NULL and numTokens of the instruction to 0

4. void addNull(instruction* instr_ptr)
  - After all of the commands are entered on the command line one last token is added to the instruction and set as NULL

5. void IOredirection( instruction* instr_ptr, int bGround)

6. void singlepipe( instruction* instr_ptr, int bGround)

7. void doublepipe( instruction* instr_ptr, int bGround)

8. void enVar(instruction* instr_ptr)
  - For loops through tokens of instruction
  - Checks to see for environmental variables such as "$HOME"
    - expands environmental variables using getenv

9. void printPrompt()
  - Prints the user, machine and working directory before the user enters anything on the command line

10. int shortRes(instruction* instr_ptr)
  - For loops through each token of the instruction
  - Checks for a token that equals '.', "..", or '~'
    - In the case for '.' 
      - Reallocate new size of token to equal the string length of the working directory
      - Copy working directory string over to the token previously holding '.' string
    - In the case for ".."
      - Get the location of the second to last '/' in the working directory string
      - Reallocate the ".." string to equal the size of the position of the last '/' in the working directory
      - Copy the working directory into the token previously holding ".." until the parent directory
    - In the case for '~'
      - Reallocate new size of token to equal the string length of the home directory
      - Copy home directory string over to the token previously holding '~'
  - Checks for a token containing '/' anywhere in the string
    - Checks if first character of the token is '~'
      - Allocate new memory for size of home directory string + Original token string length -1 because removing '~'
      - Replace '~' with the home directory and then concatenate the rest of the string onto the end
    - Tokenize the instruction token string with the delimiter being '/' in a while loop until no more tokens
      - If new token equals '.' 
        - Make the new token the size of the working directory 
        - make new token previously containg '.' into the working directory
        - Concatenate the new token to the original token then concatenate a '/' onto the end
      - If new token equals ".."
        - Make sure it is not the first token from the original token. If so break the loop
        - If it isnt the first token then find where the second to last '/' is in the original token
        - Take off all the characters of the token after the second to last '/' in the string
  - Check if the tokens resolved for shorcuts are either files or directories
  - If not file or directory return 0, If it is a file or directory then return 1

11. int pathRes(instruction* instr_ptr)
  - For loop through all tokens of the instruction
  - If token not a builtin or does not contain a '/' go resolve its path 
    - Tokenize the path environmental variable with the delimeter ':' using a while loop until done
    - concatenate the token from the instruction onto the tokenized path segments and check if it is not a file
    - if not a file return 0
    - if all instruction tokens running through pathRes are a file then returns 1
    
12. void func(instruction * instr_ptr,int commandCounter)

13. int isFile(const char * path)
  - Uses access to check if c-string passed in is a file

14. int isDirectory(const char * path)
  - Uses stat to check if c-string passed in is a directory


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
