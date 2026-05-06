#pragma once
#include <pthread.h>
#include <stdbool.h>

enum RunMode {
    NotSet,
    Async,
    Queue,
    Serial
};

enum ThreadState {
    Available,
    Awaiting,
    Running,
    Finished
};

struct RunConfig {
    enum RunMode mode;
    int threadCount;
    int pathCount;
    int pathIndexes[32];
    bool forceFlag;
    bool debugFlag;
};

struct ThreadConfig {
    char** workArray;
    int dataSize;
    int sharedIndex;
    pthread_mutex_t lock;
    bool debugFlag;
};

int fetch_index(struct ThreadConfig* conf);

void* async_thread(void* conf);

void* default_thread(void* conf);
