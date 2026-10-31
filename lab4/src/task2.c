#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        _exit(117); 
    }

    printf("Parent PID: %ld, child PID: %ld\n",
           (long)getpid(), (long)pid);

    puts("Check ps in another terminal, then press Enter.");
    getchar();

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return 1;
    }

    if (WIFEXITED(status)) {
        printf("Child exit code: %d\n", WEXITSTATUS(status));
    }

    getchar();

    return 0;
}