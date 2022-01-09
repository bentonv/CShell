/*! \class minix 
 * \brief A program for a simple Minix shell
 *  \brief This provides a simple interface for Minix
 * 	\author  Benton Valenzuela 
 *	\version 1.0
 * 	\date   13.3.06
 */
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
/* #include "sh.h"*/
/*! \class minix 
 * \brief A program for a simple Minix shell
 *  \brief This provides a simple interface for Minix
 * 	\author  Benton Valenzuela 
 *	\version 1.0
 * 	\date   13.3.06
 */
/**!
 * \def 
 *  
 *since C has no boolean type
 * 
 * */
#define TRUE 1


struct op{
int type;
char** words;

};
/* \struct built in. A simple command list*/
struct builtin{
    char* command;
    int (*f)(struct op* t);
    };



/*Prototypes I need one extra*/
char* lookupPath(char **argv,  char **dirs,char array[]);
int parseCommand(char* cLine, struct command_t* cmd);
int parsePath(char *dirs[])
/*int parsePath(char **);*/
void printPrompt(void);
void readCommand(char *buffer);
/*int setstatus(register int s);
int getn (char* as);
void leave(void); */
/*
static struct builtin builtin[] = {
    "cd",     dochdir,
    "exit",   doexit,
    0,
};
*/
int main(void)
{
   
   char* pathv[MAX_PATHS];
   char commandLine[LINE_LENGTH];
   struct command_t command;
   pid_t pid; 
   
   /*initialize command line to something.*/
   /*Shell initialization*/
   
   parsePath(pathv); /*Get directory paths from PATH*/
   
   while(TRUE)  /*since C does not have a boolean type*/
   {
      printPrompt();
      /*Read the command line and parse it*/
      readCommand(commandLine);
      
      parseCommand(commandLine, &command);
     /* printf("%s",command.argv[0]);*/
      if(!strcmp(command.argv[0],"cd"))
	chdir(command.argv[1]);
      else if(!strcmp(command.argv[0],"exit"))
	exit(0);
      /*Get the full pathname for the file*/
      else
      {
      command.name = lookupPath(command.argv,pathv,command.name);
      printf("%s\n",command.name); 
      if(command.name == NULL)
      {
      printf("Error: command does not exist.");
      /*report error*/
      continue;
      }
   /*Create child and execute the command*/
   
   switch (pid = fork())
   {
      case -1:printf("fork error");break;
      case 0:execv(command.name, command.argv);
      exit(0);break;
      default:wait(NULL);break;
   } 
      } 
   
   /*Wait for th each child to terminate*/
   
   }
}
void printPrompt(void) 
{
/** Build the prompt string to have the machine name,
 * current directory, or other desired information.
 * 
 * @see printPrompt:
 */
 /* char promptString;
  promptString = ' #';*/
  printf("bsh #");
}
void readCommand(char* buffer) {
/** This code uses any set of I/O functions, such as those in the
 * stdio library to read the entire command line into the buffer.
 * 
 * @param: character pointer
 * @return: void
 * @see: readCommand()
 */
  fgets(buffer,LINE_LENGTH,stdin);/*I may not want to use this function*/
}

int parsePath(char *dirs[]) 
{
/** This function reads the PATH variable for this
 * environment, then builds an array, dirs[], of the
 * directories in PATH
 * @param: character pointer for accessing directories in PATH
 * @return: integer type
 * @see: parsePath()
 */
  char *pathEnvVar;
  char *thePath;
  int i;

  for (i=0; i < MAX_PATHS; i++)
    dirs[i] = NULL;
  
  pathEnvVar = (char *) getenv("PATH");
  thePath = (char *) malloc(strlen(pathEnvVar) + 1);
  strcpy(thePath, pathEnvVar);
  
  dirs[0] = strtok(thePath,":");
  i = 1;
  while((dirs[i] = strtok(NULL,":"))!=NULL)
  {
   i++;    
  }
/* Loop to parse thePath. Look for a ":"
 * delimiter between each path name.
 */
  
}
char *lookupPath(char **argv, char **dirs,char array[]) {
/** This function searches the directories identified by the dir
 * argument to see if argv[0] (the file name) appears there.
 * Allocate a new string, place the full path name in it, then
 * return the string.
 * @param: char **argv - stands for argument vector
 * @param: char **dirs - for looking up directories eventually
 * @param char array  - to handle the length of the path
 * @return: char
 * @see: lookupPath()
 */
  char *result;
  char pName[MAX_PATH_LENGTH];
  int i;

/* Check to see if file name is already an absolute path*/
  if (*argv[0] == '/') 
  {
    return (argv[0]);
  }
/*
Look in PATH directories.
 Use access() to see if the file is in a dir.*/
  for (i = 0; i  <  MAX_PATHS && dirs[i] !=NULL ; i++) 
  {
     strcpy(pName,dirs[i]);
     strcat(pName,"/");
     strcat(pName,argv[0]);

     if(access(pName,F_OK)!=0)  /*int access(const char* path, int amode); */
     continue;
     else  if(access(pName,X_OK)!=0)
     continue;
     else 
     {
     strcpy(array,pName);
     return array;
     }
   }

/* File name not found in any path variable*/
  
  fprintf(stderr, "%s: command not found\n", argv[0]);
  return NULL;
}

int parseCommand(char* cLine, struct command_t* cmd)
/**
 * essentially makes sense of the command that is entered
 * such as ls 
 * @param : cLine is to be split up with a string tokenizer
 * @param : declares the struct from minishell.h and names names it cmd
 * @return: type integer
 * @see parseCommand()
 * */
{
   int argc;
   char** clPtr;
   /*init*/
   clPtr = &cLine;
   argc = 0;
  /* cmd -> argv[argc] = (char*) malloc(MAX_ARG_LENGTH);*/
   /*fill argv[]*/
   
   
   cmd -> argv[0] = strtok(cLine,WHITESPACE);
   while((cmd -> argv[++argc] = strtok(NULL,WHITESPACE)) != NULL)
   {
     /* cmd->argv[++argc] = (char* ) malloc(MAX_ARG_LENGTH);*/   
   }
   /*set the command name and argc*/
   cmd->argc = argc-1;
   cmd->name = (char*) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name,cmd->argv[0]);
   return 1;

}
