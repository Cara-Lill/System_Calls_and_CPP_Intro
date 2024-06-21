#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

int main()
{
    int pid, ret_exec, status;
    pid = fork(); // attempt to create a child process

    switch(pid) {

        case -1:
        /* Failed fork */
        printf("Error\n");  // display error message
        exit(1);

        case 0:
        /* Child process*/
        // replace the current process with the 'ps -A' command
        // listing current running processes
        ret_exec = execl("/bin/ps", "ps", "-A", NULL);
        if (ret_exec == -1) {
            printf("Error executing exec\n");
            exit(1);
        }

        default:
        /* Parent Process */
        pid = wait(&status); // returns child process id, and stores the termination status

        // check that the child process terminated correctly
        // then print out the process ids and termination status
        if (WIFEXITED(status)) {
            printf("Parent Process ID: %d\n", getpid());
            printf("Child Process ID: %d\n", pid);
            printf("Termination Status: %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
