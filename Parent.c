#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>

void error(const char* msg, size_t len)
{
        write(STDERR_FILENO, msg, len);
        exit(EXIT_FAILURE);
}
int main()
{
	int pipe1[2];
	int pipe2[2];
	int pipe3[2];

	if (pipe(pipe1) == -1 || pipe(pipe2) == -1 || pipe(pipe3) == -1)
	{
		const char msg[] = "Pipe failed\n";
		error(msg, sizeof(msg) - 1);
	}

	pid_t pid1 = fork();
	if (pid1 == -1)
	{
		const char msg[] = "Fork failed\n";
		error(msg, sizeof(msg) - 1);
	}

	if (pid1 == 0)
	{
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe3[0]);
		close(pipe3[1]);

		dup2(pipe1[0], STDIN_FILENO);
		dup2(pipe2[1], STDOUT_FILENO);
		close(pipe1[0]);
		close(pipe2[1]);

		execl("./child1", "child1", NULL);

		const char msg[] = "Execl failed\n";
		error(msg, sizeof(msg) - 1);
	}	
	
	pid_t pid2 = fork();
	if (pid2 == -1)
	{
		const char msg[] = "Fork failed\n";
		error(msg, sizeof(msg) - 1);
	}
	if (pid2 == 0)
	{
		close(pipe2[1]);
		close(pipe3[0]);
		close(pipe1[0]);
		close(pipe1[1]);

		dup2(pipe2[0], STDIN_FILENO);
		dup2(pipe3[1], STDOUT_FILENO);
		close(pipe2[0]);
		close(pipe3[1]);

		execl("./child2", "child2", NULL);

		const char msg[] = "Execl failed\n";
		error(msg, sizeof(msg) - 1);
	}	

	close(pipe3[1]);
	close(pipe1[0]);
	close(pipe2[0]);
	close(pipe2[1]);

	char buf[4096];
	ssize_t n, written, w;
	while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) 
	{
    		written = 0;
    		while (written < n) 
		{
        		w = write(pipe1[1], buf + written, n - written);
        		if (w <= 0) 
			{
            			const char msg[] = "Write to pipe1 failed\n";
				error(msg, sizeof(msg) - 1);
        		}
        		written += w;
    		}
	}
	close(pipe1[1]);

	while ((n = read(pipe3[0], buf, sizeof(buf))) > 0)
	{
		written = 0;
    		while (written < n) 
		{
        		w = write(STDOUT_FILENO, buf + written, n - written);
        		if (w <= 0) 
			{
            			const char msg[] = "Write to stdout failed\n";
            			error(msg, sizeof(msg) - 1);
        		}
        		written += w;
    		}

	}
	close(pipe3[0]);
	
	wait(NULL);
	wait(NULL);

	return 0;
}
