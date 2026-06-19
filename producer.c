#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>

volatile sig_atomic_t running = 1;
volatile sig_atomic_t count = 0;

//SIGNAL HANDLERS
void handle_sigint(int sig) {
    running = 0;
    printf("\n[Producer] SIGINT received → shutting down gracefully...\n");
}

void handle_sigusr1(int sig) {
    printf("\n[Producer] SIGUSR1 → items sent so far: %d\n", count);
}

//MAIN
int main(int argc, char *argv[]) {
    char *filename = NULL;
    int buffer_size = 4096;

    int opt;

    /* Parse command-line args */
    while ((opt = getopt(argc, argv, "f:b:")) != -1) {
        switch (opt) {
            case 'f':
                filename = optarg;
                break;
            case 'b':
                buffer_size = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-f file] [-b size]\n", argv[0]);
                exit(1);
        }
    }

    // SETUP SIGNALS
    struct sigaction sa_int, sa_usr;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    sa_usr.sa_handler = handle_sigusr1;
    sigemptyset(&sa_usr.sa_mask);
    sa_usr.sa_flags = 0;
    sigaction(SIGUSR1, &sa_usr, NULL);

    // INPUT SOURCE
    FILE *input = stdin;

    if (filename != NULL) {
        input = fopen(filename, "r");
        if (!input) {
            perror("Error opening file");
            return 1;
        }
    }

    char buffer[buffer_size];

    // MAIN LOOP
    while (running && fgets(buffer, buffer_size, input)) {
        write(STDOUT_FILENO, buffer, strlen(buffer));
        count++;
    }

    //CLEANUP
    if (filename != NULL) {
        fclose(input);
    }

    printf("\n[Producer] Final count: %d\n", count);

    return 0;
}
