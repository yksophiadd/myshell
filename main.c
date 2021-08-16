#include <stdio.h>
#include <string.h>

int main() {
    char *line = NULL;
    char *token = NULL, *program = NULL;
    size_t len = 0;
    int ret;
    printf("> ");
    while ((ret = getline(&line, &len, stdin)) != -1) {
        
        token = strtok(line, " ");
        printf("CMD: %s\n", token);
        if (!strncmp(line, "exit", 4)) {
            break;
        } else if (!strncmp(token, "echo", 4)) {
            while ((token = strtok(NULL, "")) != NULL) {
                printf("%s", token);
            }
            printf("\n");
        }
        printf("> ");
    }
    printf("\n");
    return 0;
}
