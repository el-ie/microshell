#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PIPE 42
#define COMMA 43
#define END 44
#define CMD 45
#define UNKNOWN 46
#define UNUSED -1

int	get_type(char *str)
{
	if (!str)
		return END;

	if (strcmp(str, "|") == 0)
		return PIPE;
	
	if (strcmp(str, ";") == 0)
		return COMMA;
	
	return CMD;
}

int	main(int argc, char **argv, char **envp)
{
	int	begin = 1;
	int	end = 1;
	int	last = UNKNOWN;
	int	next = UNKNOWN;
	int	fd[2] = { UNUSED, UNUSED };
	int	last_pipe = UNUSED;

	while (next != END && argv[begin])
	{
		end = begin;

		if (last == PIPE)
			last_pipe = fd[0];
		else {
			last_pipe = UNUSED;
			//fd[0] = UNUSED;
			//fd[1] = UNUSED;
			}

		while (get_type(argv[end]) == CMD)
			end++;

		next = get_type(argv[end]);

		//printf("\n end[%d] \n", end);
		//printf("\n next = %d \n", next);
		
		argv[end] = NULL;
		
		if (next == PIPE)
			pipe(fd);

		int pid = fork();

		if (pid == 0)
		{
			
			if (last_pipe != UNUSED) {
				dup2(last_pipe, STDIN_FILENO);
				close(last_pipe);
			}
			if (next == PIPE) {
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
				close(fd[0]);
			}

			execve(argv[begin], argv + begin , envp);
			exit(2);
		}

		if (last_pipe != UNUSED)
			close(last_pipe);

		if (next == PIPE)
			close (fd[1]);

		begin += (end - begin + 1);

		last = next;
		//printf("\nnew begin ->%d \n", begin);
	}

	wait(NULL);
	
}
