#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *line = NULL;
    char *token = NULL, *program = NULL;
    char **child_argv = NULL;
    char **new_argv, **tmp;
    size_t len = 0;
    int wstatus, ret;
    int child_argc = 0, capacity = 0;
    pid_t pid;

    printf("> ");
    while ((ret = getline(&line, &len, stdin)) != -1) {
        token = strtok(line, " \t\n");
        printf("CMD: %s\n", token);

        if (!strncmp(line, "exit", 4)) {
            break;
        } else if (!strncmp(token, "echo", 4)) {
            while ((token = strtok(NULL, "")) != NULL) {
                printf("%s", token);
            }
            printf("\n");
        } else {
            pid = fork();
            if (pid == 0) {
                capacity = 5;
                child_argc = 0;
                child_argv = (char **) malloc(sizeof(char*) * capacity);
                while (token != NULL) {
                    if (child_argc + 2 > capacity) {
                        new_argv = (char **) malloc(sizeof(char*) * (capacity * 2));
                        tmp = child_argv;
                        memcpy(new_argv, child_argv, sizeof(char*) * capacity);
                        child_argv = new_argv;
                        free(tmp);
                        capacity *= 2;
                    }
                    child_argv[child_argc++] = strdup(token);

                    token = strtok(NULL, " \t\n");
                }
                execvp(child_argv[0], child_argv);
                return 0;
            } else if (pid > 0) {
                printf("Wait child %d...\n", pid);
                ret = waitpid(pid, &wstatus, 0);
                printf("Wait child %d back\n", ret);
            } else {
                printf("FORK FAILED\n");
            }
        }
        printf("> ");
    }
    printf("\n");
    return 0;
}
