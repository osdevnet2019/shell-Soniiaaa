#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define Buffer_Size_Token 64
#define Buffer_Size 1024
#define Delimiters " \t\r\n\a"

int exit_shell(char **ar);
int cd_shell(char **ar);

char *command_default_command[] = { "cd", "exit"};

int (*function[]) (char **) = { &cd_shell, &exit_shell};

int default_num() 
{  
	return sizeof(command_default_command) / sizeof(char *);
}

int exit_shell(char **ar)
{  
	return 0;
}

int cd_shell(char **ar)
{
  if (ar[1] == NULL) 
  {
    fprintf(stderr, "the expected argument for \"cd\"\n");
  } 
  else 
  {
    if (chdir(ar[1]) != 0) 
	{
      perror("error shell");
    }
  }
  return 1;
}

int begin(char **ar)
{
  pid_t pdd;
  int sti;
  
  pdd = fork();
  if (pdd == 0) 
  {
    int i=0;
    while(ar[i] != NULL)
      i++;
    if(i-1 >= 0 && strcmp("&", ar[i-1]) == 0)
	{
     ar[i-1] = NULL;
    }

    if (execvp(ar[0], ar) == -1) 
	{
      perror("error shell");
    }
    exit(EXIT_FAILURE);
  } 
  else if (pdd < 0) 
  {
    perror("lsh");
  } 
  else 
  {
    do 
	{
		int i=0;
		while(ar[i] != NULL)
		i++;
		if(i-1 >= 0 && strcmp("&", ar[i-1]) == 0)
		{
			ar[i-1] = NULL;
		}
		else
		{
			waitpid(pdd, &sti, WUNTRACED); 
		}    
   } 
   while (!WIFEXITED(sti) && !WIFSIGNALED(sti));
  }
  return 1;
}


int execute(char **ar)
{
  int j;
  if (ar[0] == NULL) 
  {
    return 1;
  }
  for (j = 0; j < default_num(); j++) 
  {
    if (strcmp(ar[0], command_default_command[j]) == 0) 
	{
      return (*function[j])(ar);
    }
  }
  return begin(ar);
}

char *read_line(void)
{
  int bs = Buffer_Size;
  int k = 0;
  char *bff = malloc(sizeof(char) * bs);
  int c;

  if (!bff) 
  {
    fprintf(stderr, "allocation failed\n");
    exit(EXIT_FAILURE);
  }

  while (1) 
  {
    c = getchar();
    if (c == EOF) 
	{
      exit(EXIT_SUCCESS);
    } 
	else 
		if (c == '\n') 
		{
		  bff[k] = '\0';
		  return bff;
		} 
		else 
		{
		  bff[k] = c;
		}
    k++;

    if (k >= bs) 
	{
      bs += Buffer_Size;
      bff = realloc(bff, bs);
      if (!bff) 
	  {
        fprintf(stderr, "allocation failed\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **tokenize(char *l)
{
  int bs = Buffer_Size_Token;
  int k = 0;
  char **tks = malloc(bs * sizeof(char*));
  char *tkn, **bck;

  if (!tks) 
  {
    fprintf(stderr, "allocation failed\n");
    exit(EXIT_FAILURE);
  }

  tkn = strtok(l, Delimiters);
  while (tkn != NULL) 
  {
    tks[k] = tkn;
    k++;

    if (k >= bs) 
	{
      bs += Buffer_Size_Token;
      bck = tks;
      tks = realloc(tks, bs * sizeof(char*));
      if (!tks) 
	  {
		free(bck);
        fprintf(stderr, "allocation failed\n");
        exit(EXIT_FAILURE);
      }
    }
    tkn = strtok(NULL, Delimiters);
  }
  tks[k] = NULL;
  return tks;
}

int main(int argc, char **argv)
{
  char *ln;
  char **ar;
  int sti;
  char hh[256];
  do 
  {
    if (getcwd(hh, sizeof(hh)) != NULL)
          printf("%s", hh);
    if(geteuid() == 0)
        printf("$ ");
    else
        printf("# ");
    ln = read_line();
    ar = tokenize(ln);
    sti = execute(ar);
    free(ln);
    free(ar);
  } 
  while (sti);
  return EXIT_SUCCESS;
}

