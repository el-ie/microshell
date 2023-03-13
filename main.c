
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define UNKNOWN 40

#define PIPE	41
#define COMMA	42
#define END	43
#define CONTENT	44

int	ft_strlen(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return i;
}

int	get_type(char *str)
{
	if (str == NULL)
		return END;
	
	if (strcmp(str, "|") == 0)
		return PIPE;
	
	if (strcmp(str, ";") == 0)
		return COMMA;
	
	return CONTENT;
}

int	main(int argc, char **argv, char **envp)
{
	int	begin = 1;
	int	end = 1;
	int	type = UNKNOWN;
	int	last_type = UNKNOWN;
	int	fd[2] = { UNKNOWN, UNKNOWN };
	int	old_pipe = UNKNOWN;

	int	pid;

	if (argc < 2) {
		return 1;
	}

	while (argv[begin] && type != END)
	{
		while (argv[end] && get_type(argv[end]) == CONTENT)
			end++;

		type = get_type(argv[end]);

		if (argv[end] == NULL)
			type = END;

		//securisation segfault + passer les points de controle seuls
		if (begin == end) {
			begin++;
			end = begin;
			continue;
		}

		argv[end] = NULL;

		if (type == PIPE) {
			pipe(fd);
		}

		if (strcmp(argv[begin], "cd") == 0)
		{
			if ( (!argv[begin + 1])
			||	(get_type(argv[begin + 1]) != CONTENT)
			||	(get_type(argv[begin + 2]) == CONTENT) )
			{
				write(2, "error: cd: bad arguments\n", 25);
			}
			else
			{
				if (chdir(argv[begin + 1]) == -1)
				{
					write(2, "error: cd: cannot change directory to ", 38);
					write(2, argv[begin + 1], ft_strlen(argv[begin + 1]));
					write(2, "\n", 1);
				}
			}
		}
		else
		{
			pid = fork();

			if (pid == 0)
			{
				if (last_type == PIPE) {
					dup2(old_pipe, STDIN_FILENO);
					close(old_pipe);
				}
				if (type == PIPE) {
					close(fd[0]);
					dup2(fd[1], STDOUT_FILENO);
					close(fd[1]);
				}

				execve(argv[begin], argv + begin, envp);
				
				if (last_type == PIPE)
					close (STDIN_FILENO);
				if (type == PIPE) {
					close (fd[0]);
					close (fd[1]);
				}
				write(2, "error: cannot execute ", 22);
				write(2, argv[begin], ft_strlen(argv[begin]));
				write(2, "\n", 1);
				exit (2);
			}
		}


		if (last_type == PIPE)
			close(old_pipe);

		if (type == PIPE) {
			close (fd[1]);
			old_pipe = fd[0];
		}

		while(waitpid(-1, NULL, WUNTRACED) != -1)
			;

		last_type = type;
		begin = end + 1;
		end = begin;

	}

}

