#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void do_wait_child_proc(pid_t *wait_list, int *cnt)
{
    int i, j, ret;
    int wstatus;
    for (i = 0; i < *cnt; i++) {
        ret = waitpid(wait_list[i], &wstatus, WNOHANG);
        if (ret != 0 && WIFEXITED(wstatus)) {
            printf("[%d] exited\n", wait_list[i]);
            for (j = i + 1; j < *cnt; j++) {
                wait_list[j-1] = wait_list[j];
            }
            i--;
            (*cnt)--;
        }
    }
}

int main()
{
    char *line = NULL;
    char *token = NULL, *program = NULL;
    char **child_argv = NULL, **new_argv;
    void *tmp;
    size_t len = 0;
    int wstatus, ret, i;
    int child_argc = 0, argv_cap = 0;
    int wait_list_cap = 0, wait_list_cnt = 0;
    pid_t pid;
    pid_t *wait_list, *new_wait_list;

    // Initialize argv_cap of child_argv
    argv_cap = 5;
    child_argv = (char **) malloc(sizeof(char*) * argv_cap);

    // Initialize wait_list
    wait_list_cap = 5;
    wait_list = (pid_t *) malloc(sizeof(pid_t) * wait_list_cap);

    printf("> ");
    while ((ret = getline(&line, &len, stdin)) != -1) {
        token = strtok(line, " \t\n");

        if (!strncmp(line, "exit", 4)) {
            break;
        } else if (!token) {
            printf("> ");
            continue;
        } else if (!strncmp(token, "echo", 4)) {
            while ((token = strtok(NULL, "\n")) != NULL) {
                printf("%s", token);
            }
            printf("\n");
        } else {
            // Parse arguments
            child_argc = 0;
            while (token != NULL) {
                if (child_argc + 1 == argv_cap) {
                    new_argv = (char **) malloc(sizeof(char*) * (argv_cap * 2));
                    tmp = (void *)child_argv;
                    memcpy(new_argv, child_argv, sizeof(char*) * argv_cap);
                    child_argv = new_argv;
                    free(tmp);
                    argv_cap *= 2;
                }
                child_argv[child_argc++] = strdup(token);

                token = strtok(NULL, " \t\n");
            }
            child_argv[child_argc] = NULL;

            pid = fork();
            if (pid == 0) {
                if ((strlen(child_argv[child_argc - 1]) == 1)
                    && (!strncmp(child_argv[child_argc - 1], "&", 1))) {
                    free(child_argv[child_argc - 1]);
                    child_argv[child_argc - 1] = NULL;
                }
                execvp(child_argv[0], child_argv);
                for (i = 0; i < child_argc; i++) {
                    free(child_argv[i]);
                    child_argv[i] = NULL;
                }
                free(child_argv);
                return 0;
            } else if (pid > 0) {
                if ((strlen(child_argv[child_argc - 1]) == 1)
                    && (!strncmp(child_argv[child_argc - 1], "&", 1))) {
                    ret = waitpid(pid, &wstatus, WNOHANG);
                    if (ret == 0) {
                        // pid status no change
                        if (wait_list_cnt == wait_list_cap) {
                            // Extend wait_list size
                            new_wait_list = (pid_t *) malloc(sizeof(pid_t) * wait_list_cap * 2);
                            tmp = wait_list;
                            memcpy(new_wait_list, wait_list, sizeof(pid_t) * wait_list_cap);
                            wait_list = new_wait_list;
                            free(tmp);
                            wait_list_cap *= 2;
                        }
                        wait_list[wait_list_cnt++] = pid;
                    } else if (ret < 0) {
                        printf("Wait pid error\n");
                    } else if (WIFEXITED(wstatus)) {
                        printf("[%d] exited\n", ret);
                    } else {
                        printf("[%d] signaled\n", ret);
                    }
                } else {
                    ret = waitpid(pid, &wstatus, 0);
                }
            } else {
                printf("FORK FAILED\n");
            }
            for (i = 0 ; i < child_argc; i++) {
                free(child_argv[i]);
            }
        }
        free(line);
        line = NULL;
        len = 0;
        // Check if waiting child exited
        if (wait_list_cnt > 0) {
            do_wait_child_proc(wait_list, &wait_list_cnt);
        }
        printf("> ");
    } // end of getline
    while (wait_list_cnt) {
        do_wait_child_proc(wait_list, &wait_list_cnt);
    }
    printf("\n");
    free(child_argv);
    free(wait_list);
    return 0;
}
