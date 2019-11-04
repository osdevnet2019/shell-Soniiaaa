#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

char *fgetstr(char *str, int c, FILE *stream)
{
	char *res;
	res = fgets(str, c, stream);
	if(!res)
		return res;
	if(str[strlen(str) -1] == '\n')
		str[strlen(str) - 1] = 0;
	
	return(str);
}

int main()
{	
	char ln[1000];
	char *tok;
	char *args[10];
  while(1)
  {
	printf("\n$ ");
	fgetstr(ln, 1000, stdin);
	char *s = "&";
	int kh = 0;
	args[0] = strtok(ln, " ");
	int i = 1;
	do
	{
		tok = strtok(NULL, " ");
		args[i] = tok;
		i++;
	} 
	while(tok !=NULL);

	if(!strcmp(args[i-2], s))
	{
		args[i-2] = NULL;
		kh = 1;
	}
	
	fflush(NULL);
	pid_t pid;
	pid = fork();
	
	if(pid < 0)
	{
		printf("Forking failed\n");
	}
	if(pid != 0)
	{
           printf("Parent ID %d\n", (int)getpid());
	   if(kh)
	   {
	   	printf("Waiting");
		wait(NULL);
       }
	}
	else
	{
	   printf("Child ID %d\n", (int)getpid());
	   if(execvp(args[0], args) == -1)
		printf("execvp FAILED");
	   execvp(args[0], args);
	}
  }
  return 0;
}
