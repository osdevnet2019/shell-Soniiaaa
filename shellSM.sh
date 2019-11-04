#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


char *fgetstr(char *string, int n, FILE *stream)
{
	char *result;
	result = fgets(string, n, stream);
	if(!result)
		return result;
	if(string[strlen(string) -1] == '\n')
		string[strlen(string) - 1] = 0;
	
	return(string);

}

int main()
{	
	char line[1000];
	char *token;

	char *args[10];


  while(1)
  {

	printf("\n$ ");
	fgetstr(line, 1000, stdin);
	char *s = "&";
	int bg = 0;
	args[0] = strtok(line, " ");
	int i = 1;

	do
	{
		token = strtok(NULL, " ");
		args[i] = token;
		i++;
	} while(token !=NULL);


	if(!strcmp(args[i-2], s))
	{
		args[i-2] = NULL;
		bg = 1;
	}


	fflush(NULL);
	pid_t pid;
	pid = fork();
	
	if(pid < 0)
	{
		printf("Forking process failed\n");
	}
	if(pid != 0)
	{
           printf("this is the parent process with id %d\n", (int)getpid());
	   if(bg)
	   {
	   	printf("Waiting");
		wait(NULL);
           }

	}
	else
	{
	   printf("this is the child process with id %d\n", (int)getpid());
	   if(execvp(args[0], args) == -1)
		printf("execvp FAILED");
	   
	   execvp(args[0], args);
	}
  }
  return 0;
}
