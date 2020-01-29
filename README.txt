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
  • To clean the directory the shell is running in, type "make clean" into the command line.
  • This will remove the .o files and the "shell" executable.



Function Descriptions: 

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
  - After all of the commands are entered on the command line one last token is added to the instruction 
    and set as NULL

5. void IOredirection( instruction* instr_ptr, int bGround)
  - This function takes in the instruction pointer, and an integer which will equal 1 if the command should 
    be run in the background or 0 if the command should not 
  - This function first parses through the tokens to determine which will be the input file and which will be 
    the output file
  - The fork function is called and in the child process redirects the input and output only if there are 
    input and output files respectively 
  - Then the my_execute function is called to run the command and the child process is exited.  

6. void singlepipe( instruction* instr_ptr, int bGround)
  - This function takes in the instruction pointer, and an integer which will equal 1 if the command should be run in the         background or 0 if the command should not
  - First this function copies the writing function and the reading function as tokens in the two instruction variables 
    cmd1 and cmd2
  - The program forks, the pipe function is called, and program forks again
  - Inside the first child the input and output file descriptors are changed, the first function is executed, and the 
    child process is terminated
  - Again inside the seccond child the input and output file descriptors are changed, the seccond function is executed, 
    and the child process is terminated
  - The parent program then calls the wait function to wait for the child processes to finish

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
  - This function looks at the parsed instructions and then decides if the next step is one of four possibilities: I/O             redirection, piping, built ins, or regular execution
  - After this the tokens are checked for the "&" symbol to indicate background processing 
  - Then the appropriate functions are called depending on which of the four types of execution need to run  

13. int isFile(const char * path)
  - Uses access to check if c-string passed in is a file

14. int isDirectory(const char * path)
  - Uses stat to check if c-string passed in is a directory
  
15. void backgroundInit()
  - Initilizes pElement struct with NULL in all elements.

16. void pushProcess(struct ProcessElement *p)
  - pEntry is checked for a NULL.
  - If it is NULL, the entry is set to the value passed in (*p)
  - printProcessStart(pEntry[i], i); is called.
  
17. void rmProcess(int index)
  - If the index is int the range of 0 - 256, and if the index is not equal to NULL, the function
  -       deleteEntry(pEntry[index]);
  - is called.      

18. void checkQueue()
  - Loops through the whole array, if an index is NOT equal to NULL adn if 
  -       checkProcess(pEntry[i])
  - returns a value of 1, 
  -       rmProcess(i);
  - is called.
  
19. struct ProcessElement* mkProcess(int pid_1, int pid_2, char *cmd)
  - Creates a new element with:
  -       struct ProcessElement* newElement = (struct ProcessElement*)calloc(1, sizeof(struct ProcessElement));
  - and assigns the data elements pid_1,_2, and cmd to the elements of newElement. 
  
20. int checkProcess(struct ProcessElement *p)
  - Checks the pid values of the ProcessElement struct, to determine if they are valid.
  - If they are not, an error is printed.
  
21. void deleteEntry(struct ProcessElement *p)
  - Frees the *p and its data.  Frees p and p->cmd  

22. void exitCommandProcess()
  - Checks each element of the Background array.
  - If the element is not equal to NULL, it is flagged as still running.
  
23. void printProcessStart(const struct ProcessElement *p, int pos)
  - If the element is not equal to NULL, the position and pid number are printed.
  
24. void my_execute(char **argument, int background, char* cmd)
  - Forks and runs a child process on the command passed in.
  - If the process is flagged as a background process, a wait pid WNOHANG is called and 
  -       pushProcess(mkProcess(pid, -1, cmd));
  - is called.
  
25. void exitShell(char **cmd, int commandCounter)
  - Calls: 
  -       exitCommandProcess();
  - and prints the exit message, along with the commands issued.
  
26. void changeDirectory(const char* dir)
  - Makes a new path with chdir(dir) and then sets the environment variable to the new working directory, if new path was 0.

27. void echo(char *string)
  - If the first element of the string, the path variable is expanded, if valid. 
  - Otherwise, the string that was passed in is printed.

28. void jobs(const struct ProcessElement *p, int pos)
  - If background process is not NULL, the active jobs are printed.

29. void freedom(char **theArray)
  - Frees all of the allocated memory.
  
Known Bugs/Unfinished Portions: 
-GUYS MAKE SURE TO INFORM THEM OF 1. WHEN IT HAPPENDS.  2. WHY IT HAPPENDS.  3.  HOW WE ATTEMPTED TO FIX IT
1. When changing directory into the subdirectory the prompt does not print out whole working directory and 
   just the directory it is currently in
    - It is happening because there is no resolution of the directory name when the directory is changed
    - Tried to solve it by putting a shortRes function before the directory is changed but getenv("PWD") 
      only returns the name of the directory it is in and not the previous directories seperated by '/'
2.  Bug: Background processing prints "junk" after a job finishes in the [pid number] section. Occurs in RUNTIME.
      - Example input:
          ls &
        Output:
          [0]     [17155]
          name@linprog1.cs.fsu.edu:/home/majors/name/cop4610/file>/bin/ls: cannot access &: No such file or directory
        Input:
          ls  
        Output:
          executionProcessing.c  executionProcessing.o  main.o    shell
          executionProcessing.h  main.c                 makefile
          [0]+    [H??H9?u?H?[]A\A]A^A_Ðf.?]
          
     This bug first appeared when the entire program was put together and tested as a single unit and is probably. 
     a bad memory access, somewhere. Attempts to fix the bug included combining the command tokens into a single 
     string (so it could all be printed as one string of [/bin/ls &/], for example), removing the '&'
     character from the token list, if it occurred, and crying (just kidding).
        
3.
4.
5.
6.
7.
8.
9.



Special Considerations: 

• The "Shell-ception" extra credit was completed.



























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
