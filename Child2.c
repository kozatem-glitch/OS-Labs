#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void error(const char* msg, size_t len)
{
	write(STDERR_FILENO, msg, len);
	exit(EXIT_FAILURE);
}
int main()
{
	char buf[4096];
	bool prev_is_space = false, is_space = false;
	ssize_t i = 0, n = 0, write_pos = 0, w = 0, written = 0;
	while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
	{
		write_pos = 0;
		for (i = 0; i < n; i++)
		{
			is_space = (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n' || buf[i] == '\r');
			if (is_space)
			{
				if (!prev_is_space)
				{		
					buf[write_pos++] = buf[i];
				}
				prev_is_space = true;
			}
			else
			{
				buf[write_pos++] = buf[i];
				prev_is_space = false;
			}
		}
		written = 0;
		while (written < write_pos) 
		{
    			w = write(STDOUT_FILENO, buf + written, write_pos - written);
    			if (w <= 0) 
			{
        			const char msg[] = "Write to stdout failed\n";
        			error(msg, sizeof(msg) - 1);
    			}
    			written += w;
		}
	}

	if (n == -1)
	{
		const char msg[] = "Read from stdin failed\n";
		error(msg, sizeof(msg) - 1);
	}
		
	return 0;
}