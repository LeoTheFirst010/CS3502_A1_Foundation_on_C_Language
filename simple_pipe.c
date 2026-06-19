#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[100];
    char *message = "Hello from parent!";

    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // CHILD
        close(pipefd[1]); // close write end

        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        close(pipefd[0]);
    } else {
        // PARENT
        close(pipefd[0]); // close read end

        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);

        wait(NULL);
    }

    return 0;
}
