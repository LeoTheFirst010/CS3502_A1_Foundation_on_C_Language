#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    char buffer[4096];

    printf("Consumer started...\n");

    while (fgets(buffer, sizeof(buffer), stdin)) {
        printf("Consumed: %s", buffer);
    }

    printf("Consumer finished.\n");
    return 0;
}
