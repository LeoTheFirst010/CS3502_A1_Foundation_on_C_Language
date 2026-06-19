#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipe1[2]; // parent → child
    int pipe2[2]; // child → parent

    pipe(pipe1);
    pipe(pipe2);

    pid_t pid = fork();

    if (pid == 0) {
        // CHILD

        close(pipe1[1]);
        close(pipe2[0]);

        char msg[100];

        read(pipe1[0], msg, sizeof(msg));
        printf("Child got: %s\n", msg);

        char reply[] = "Hello parent, message received!";
        write(pipe2[1], reply, strlen(reply) + 1);

        close(pipe1[0]);
        close(pipe2[1]);

    } else {
        // PARENT

        close(pipe1[0]);
        close(pipe2[1]);

        char msg[] = "Hello child!";

        write(pipe1[1], msg, strlen(msg) + 1);

        char response[100];
        read(pipe2[0], response, sizeof(response));

        printf("Parent got: %s\n", response);

        close(pipe1[1]);
        close(pipe2[0]);

        wait(NULL);
    }

    return 0;
}
