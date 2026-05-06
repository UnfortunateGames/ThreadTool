#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include <threads.h>
#include <rundata.h>

int run_threads(struct RunConfig conf, char** argv) {
    char* workArray[conf.pathCount];
    pthread_t threadArray[conf.threadCount];
    int rvalue;
    struct ThreadConfig thConf = {
        .workArray = workArray,
        .dataSize = conf.pathCount,
        .sharedIndex = 0,
        .lock = PTHREAD_MUTEX_INITIALIZER,
        .debugFlag = conf.debugFlag
    };
    if (conf.debugFlag) printf("[ThTool] Initializing work array.\n");
    for (int i = 0; i < conf.pathCount; i++)
        workArray[i] = argv[conf.pathIndexes[i]];
    switch (conf.mode) {
        case Async:
            if (conf.debugFlag) printf("[ThTool] Async initialize.\n");
            for (int i = 0; i < conf.threadCount; i++) {
                rvalue = pthread_create(
                    &threadArray[i], NULL, async_thread, (void*) &thConf
                );
                if (rvalue != 0) return rvalue;
            }
            break;
        case Queue:
            if (conf.debugFlag) printf("[ThTool] Queue initialize.\n");
            for (int i = 0; i < conf.threadCount; i++) {
                rvalue = pthread_create(
                    &threadArray[i], NULL, default_thread, (void*) &thConf
                );
                if (rvalue != 0) return rvalue;
            }
            break;
        case Serial:
            if (conf.debugFlag)
                printf("[ThTool] Serial initialize, only one thread will be made.\n");
            rvalue = pthread_create(
                &threadArray[0], NULL, default_thread, (void*) &thConf
            );
            break;
        case NotSet:
            printf("[ThTool] If you're seeing this, I'm an idiot.");
            return 0;
    }
    if (conf.debugFlag) printf("[ThTool] Initialize finished.\n");
    if (rvalue != 0) return rvalue;
    if (conf.mode == Async)
        for (int i = 0; i < conf.threadCount; i++) {
            rvalue = pthread_join(threadArray[i], NULL);
            if (rvalue != 0) return rvalue;
        }
    else {
        bool error, finished = false;
        int mainReturn = 0;
        void* returnBuffer;
        while (true) {
            for (int i = 0; i < conf.threadCount; i++) {
                rvalue = pthread_join(threadArray[i], &returnBuffer);
                if (rvalue != 0) {
                    error = true;
                    mainReturn = rvalue;
                }
                rvalue = (int)(intptr_t) returnBuffer;
                if (rvalue == 1) error = true;
                else if (rvalue == 2) finished = true;
            }
            if (error) {
                pthread_mutex_destroy(&thConf.lock);
                if (mainReturn != 0) return mainReturn;
                return 1;
            } else if (finished) break;
            for (int i = 0; i < conf.pathCount; i++) {
                rvalue = pthread_create(
                    &threadArray[i], NULL, default_thread, (void*) &thConf
                );
            }
        }
    }
    if (conf.debugFlag) printf("[TTDebug] Run exitted.\n");
    pthread_mutex_destroy(&thConf.lock);
    return 0;
}
