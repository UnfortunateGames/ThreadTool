#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/wait.h>

#include <rundata.h>

int fetch_index(struct ThreadConfig* conf) {
    int out;
    if (conf->debugFlag) printf("[TTDebug] Locking mutex.\n");
    pthread_mutex_lock(&conf->lock);
    out = conf->sharedIndex;
    conf->sharedIndex++;
    if (conf->debugFlag) printf("[TTDebug] Unlocking mutex.\n");
    pthread_mutex_unlock(&conf->lock);
    return out;
}

void* async_thread(void* conf) {
    struct ThreadConfig* pConf = (struct ThreadConfig*) conf;
    int uniqueIndex, rvalue = 0;
    pid_t pid;
    while (true) {
        if (pConf->debugFlag) printf("[TTDebug] Fetching index.\n");
        uniqueIndex = fetch_index(pConf);
        if (pConf-> dataSize < uniqueIndex) break;
        if (pConf->debugFlag) printf("[TTDebug] Forking thread.\n");
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed with return error: %d\n", (int) pid);
            rvalue = 1;
            return (void*)(intptr_t) rvalue;
        } else if (pid == 0) {
            if (pConf->debugFlag)
                printf("[TTDebug] Executing \"%s\".", pConf->workArray[uniqueIndex]);
            execlp(pConf->workArray[uniqueIndex], pConf->workArray[uniqueIndex], NULL);
            break;
        }
        waitpid(pid, NULL, 0);
    }
    if (pConf->debugFlag && pid != 0) printf("[TTDebug] Thread exitted.\n");
    return (void*)(intptr_t) rvalue;
}

void* default_thread(void* conf) {
    struct ThreadConfig* pConf = (struct ThreadConfig*) conf;
    if (pConf->debugFlag) printf("[TTDebug] Fetching index.\n");
    int uniqueIndex = fetch_index(pConf);
    int rvalue = 0;
    if (pConf->dataSize < uniqueIndex) {
        rvalue = 2;
        return (void*)(intptr_t) rvalue;
    } if (pConf->debugFlag) printf("[TTDebug] Forking thread.\n");
    pid_t pid = fork();
    if (pid < 0) {
        rvalue = 1;
        return (void*)(intptr_t) rvalue;
    } else if (pid == 0) {
        if (pConf->debugFlag)
            printf("[TTDebug] Executing \"%s\".\n", pConf->workArray[uniqueIndex]);
        execlp(pConf->workArray[uniqueIndex], pConf->workArray[uniqueIndex], NULL);
        return (void*)(intptr_t) rvalue;
    }
    waitpid(pid, NULL, 0);
    return (void*)(intptr_t) rvalue;
}
