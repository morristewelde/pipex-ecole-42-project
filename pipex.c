#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int demain(int ac, char **av, char **envp)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if (pid == 0)
	{
		sleep(2);
		printf("\x1b[33m" "I am the child process.\n" "\x1b[0m");
		exit(0);
	}
	else
	{
		printf("I am the parent process.\n");
		wait(NULL);
		printf("Child process terminated after a 2s delay.\n");
	}
	return (0);
}

// #include <errno.h>
/*int main() {
    // Open morris2.txt for reading
    int fd2 = open("morris2.txt", O_RDONLY);
    int fd = open("morris.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(fd2, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);

    // execl("/usr/bin/cat", "cat", NULL);
	char *av[2] = {"cat", NULL};
	execve("/bin/cat", av, NULL);

	perror("ERROR ON CAT");

    close(fd2);
    close(fd);
    return 1;
}*/

# define RED "\x1b[31m"


int main()
{
	int rem = remove("example.txt");
	
	if (rem == 0)
		printf("deleted succesfully\n");
	else 
	{
		char *buf = strerror(errno);
		printf("ailed to remove : %s", buf);
	}
	return (0);
}


