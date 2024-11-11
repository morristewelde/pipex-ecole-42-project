#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

// DUP
/*int	main()
{
	int fd;

	fd = open("test.txt", O_RDWR | O_TRUNC);
	int fd_cp = dup(fd);
	int fd_cp2 = dup(fd_cp);
	
	printf("%d, %d, %d\n", fd, fd_cp, fd_cp2);

	write(fd, "111\n", 4);
	write(fd_cp, "222\n", 4);
	write(fd_cp2, "333\n", 4);
	
	close(fd);
	close(fd_cp);
	return (0);
}*/

// DUP2
/*int	main()
{
	int fd;

	fd = open("morris2.txt", O_WRONLY | O_CREAT, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	printf("this is printed in morris.txt\n");
	return (0);
}*/

//ACCESS
/*int	main(int ac, char **av)
{
	(void)ac;
	char path[255];

    snprintf(path, sizeof(path), "../../../../bin/%s", av[1]);

	if (access(path, F_OK) != -1)   // or (access("morris.txt", F_OK) chaecks is morris.txt exists in the current directory
		printf("the file exists\n");
	else
		printf("Is not there\n");
	return (0);
}*/


//EXECVE  ... EXECVP With error of permision b/c Xorg is prohibited in the pc of 42
/*int main()
{
	char *av[3];

	av[0] = "xorg";
	av[1] = "X";
	av[2] = NULL;

	execve("/usr/lib/xorg/Xorg.wrap", av, NULL);
	// execvp("/bin/ls", av); it will do the same thing as execve exept here we dont need the full path
	printf("This line will not be done.\n");
	return (0);
}*/

//EXECVP  ... EXECVP which works
int main()
{
	char *av[3];

	av[0] = "ls";
	av[1] = "-l";
	av[2] = NULL;

	execve("/usr/bin/ls", av, NULL);
	// execvp("/bin/ls", av); it will do the same thing as execve exept here we dont need the full path
	printf("This line will not be done.\n");
	return (0);
}

//FORK
/*int main(int ac, char **av)
{
	int fd[2];
	if (pipe(fd) == -1){
		printf("pipe error\n");
		return (1);
	}
	int id = fork();
	if (id == -1)
		return 1;
	if (id == 0) {
		close(fd[0]);
		int x;
		printf("give me a number: ");
		scanf("%d", &x);
		if (write(fd[1], &x, sizeof(int)) == -1)
			return 2;
		close(fd[1]);
	} else {
		close(fd[1]);
		int y;
		if (read(fd[0], &y, sizeof(int)) == -1)
			return 3;
		close(fd[0]);
		y *= 3;
		printf("get this from my child %d \n", y);
	}
	return (0);
}*/

//PIPE
/*int main(int ac, char **av)
{
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int arrsize = sizeof(arr) / sizeof(int);
	int	start, end;
	int fd[2];
	pipe(fd);
	int id = fork();
	if (id == 0)
	{
		close(fd[0]);
		start = 0;
		end = arrsize / 3;
		int sum1 = 0;
		for (int i = start; i < end; i++)
		{
			sum1 += arr[i];
		}
		write(fd[1], &sum1, sizeof(int));
		close(fd[1]);
		printf("%d\n", sum1);
	}
	else {
		close(fd[1]);
		start = arrsize / 2;
		end = arrsize;
		int sum2 = 0;
		for (int i = start; i < end; i++)
		{
			sum2 += arr[i];
		}
		int totalsum;
		read(fd[0], &totalsum, arrsize / 2);
		close(fd[0]);
		totalsum += sum2;
		wait(NULL);
		printf("%d\n", sum2);
		printf("%d\n", totalsum);
	}
	return (0);
}*/

/*int main(int ac, char **av)
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int arrsize = sizeof(arr) / sizeof(int);
    int fd[2];
    pipe(fd);

    int id1 = fork();
    if (id1 == 0) // First child process
    {
        close(fd[0]);
        int start = 0;
        int end = arrsize / 3;
        int sum1 = 0;

        for (int i = start; i < end; i++)
        {
            sum1 += arr[i];
        }
        write(fd[1], &sum1, sizeof(int));
        close(fd[1]);
        printf("Sum1 (Process 1): %d\n", sum1);
        return 0;
    }

    int id2 = fork();
    if (id2 == 0) // Second child process
    {
        close(fd[0]); // Close reading end
        int start = arrsize / 3;
        int end = 2 * arrsize / 3;
        int sum2 = 0;

        for (int i = start; i < end; i++)
        {
            sum2 += arr[i];
        }
        write(fd[1], &sum2, sizeof(int));
        close(fd[1]);
        printf("Sum2 (Process 2): %d\n", sum2);
        return 0;
    }

    close(fd[1]);
    int start = 2 * arrsize / 3;
    int end = arrsize;
    int sum3 = 0;

    for (int i = start; i < end; i++)
    {
        sum3 += arr[i];
    }

    int sum1, sum2;
    read(fd[0], &sum1, sizeof(int));
    read(fd[0], &sum2, sizeof(int));
    close(fd[0]);

    int totalsum = sum1 + sum2 + sum3;
    int wait1 = wait(NULL);
    int wait2 = wait(NULL);

    printf("Sum3 (Parent Process): %d\n", sum3);
    printf("Total Sum: %d\n", totalsum);

	printf("was waiting for %d\n", wait1);
	printf("was waiting for %d\n", wait2);

    return 0;
}*/

/*int main(int argc, char *argv[])
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
		printf("I am the child process.\n");
		sleep(2);
		exit(0);
	}
	else
	{
		wait(NULL);
		printf("I am the parent process.\n");
		printf("Child process terminated after a 2s delay.\n");
	}

	return (0);
}*/

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

int demain()
{
	fprintf(stdout, "text from fprintf\n");
	return 0;
}

