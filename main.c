
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PIPE 1
#define COMMA 2

int	is_limit(char *str)
{
	if (strcmp(str, "|") == 0)
		return PIPE;
	
	if (strcmp(str, ";") == 0)
		return COMMA;
	
	return 0;
}

int	is_pipe(char *str) {
	if (strcmp(str, "|") == 0)
		return 1;
	return 0;
}

int	is_comma(char *str) {
	if (strcmp(str, ";") == 0)
		return 1;
	return 0;
}

int	main(int argc, char **argv, char **envp)
{
	int	begin = 1;
	int	end = 1;
	int	last = 0;
	//int	fd[2] = { STDIN_FILENO, STDOUT_FILENO };

	while (!last && argv[begin])
	{
		end = begin;

		while(argv[end] && !is_limit(argv[end]))
			end++;

		//printf("\n end[%d] \n", end);
		
		if (argv[end] == NULL)
			last = 1;
		else
		{
		/*
			if (is_pipe(argv[end]))
				pipe(fd);
				*/

			argv[end] = NULL;
		}

		int pid = fork();

		if (pid == 0)
		{
			execve(argv[begin], argv + begin , envp);
			exit(2);
		}

		waitpid(pid, NULL, 0);

		begin += (end - begin + 1);
		//printf("\nnew begin ->%d \n", begin);
	}

}
