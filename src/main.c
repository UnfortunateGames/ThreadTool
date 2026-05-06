#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#include <rundata.h>
#include <arghandler.h>
#include <threads.h>

int main(int argc, char** argv) {
    struct RunConfig config = {
        NotSet,
        2, 0,
        {0},
        false, false
    };
    int maxThreads = get_nprocs();
    int rvalue = set_arguments(&config, argc, argv);
    if (rvalue == 2) return rvalue;
    else if (rvalue == 1) return 0;
    if (config.mode != Serial) {
        if (config.threadCount > maxThreads * 2) {
            if (!config.forceFlag) {
                fprintf(stderr, "[ThTool] Thread count is higher than double core count!\n");
                fprintf(stderr, "[ThTool] Are you sure what you're doing is worth it?\n");
                return 2;
            } printf("[ThTool] You have been warned! Forcing...\n");
        }
    } else if (config.threadCount > maxThreads)
        printf("[ThTool] Threads more than core count, performance will drop.\n");
    if (config.mode == NotSet) {
        fprintf(
            stderr,
            "[ThTool] You haven't passed a run flag, you may use \"-h\" or \"--help\".\n"
        );
        return 2;
    } if (config.pathCount == 0) {
        fprintf(stderr, "[ThTool] You haven't passed an executable.\n");
        return 2;
    }
    if (config.mode == Serial) config.threadCount = 1;
    rvalue = run_threads(config, argv);
    if (rvalue != 0) {
        fprintf(stderr, "An error has occurred. Error code: %d\n", rvalue);
        fprintf(stderr, "Please report this bug.\n");
        return 1;
    }
    return 0;
}
