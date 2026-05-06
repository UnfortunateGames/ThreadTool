#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <arghandler.h>
#include <rundata.h>

static const char* HELP_MESSAGE =
    "Thread Tool\n"
    "\tA tool use to run an executable asynchronously.\n\n"

    "NOTE:\tYou have to append \"./\" to the path of the executable.\n"
    "\tIf it's a script in your system, you don't have to.\n\n"

    "Run flags:\n"
    "\tasync\t- Will run whenever freed. It tries to finish as fast as possible\n"
    "\tqueue\t- Will run all threads at once, and waits until every thread is done.\n"
    "\t\tOnly then it will continue.\n"
    "\tserial\t- Will run the executables one by one, in one thread.\n"
    "\t\tThis is a a debug feature.\n\n"

    "Other flags:\n"
    "\t-h/-help\t- Shows this message.\n"
    "\t-d/--debug\t- This will show you debug messages for debugging.\n"
    "\t-f/--force\t- Will bypass the overthreading warning. You have been warned.\n"
    "\t-t/-threads\t- This sets the max threads, this is 2 by default.\n";

int set_arguments(struct RunConfig* conf, int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "async") == 0) {
            if (conf->debugFlag)
                printf("[TTDebug] Read \"async\" run flag.\n");
            if (conf->mode != NotSet) {
                fprintf(stderr, "You have put 2 or more run flags.\n");
                return 2;
            }
            conf->mode = Async;
        } else if (strcmp(argv[i], "queue") == 0) {
            if (conf->debugFlag)
                printf("[TTDebug] Read \"queue\" run flag.\n");
            if (conf->mode != NotSet) {
                fprintf(stderr, "You have put 2 or more run flags.\n");
                return 2;
            }
            conf->mode = Queue;
        } else if (strcmp(argv[i], "serial") == 0) {
            if (conf->debugFlag)
                printf("[TTDebug] Read \"serial\" run flag.\n");
            if (conf->mode != NotSet) {
                fprintf(stderr, "You have put 2 or more run flags.\n");
                return 2;
            }
            conf->mode = Serial;
        } else if (
            strcmp(argv[i], "-h") == 0
            || strcmp(argv[i], "--help") == 0
        ) {
            printf("%s", HELP_MESSAGE);
            return 1;
        } else if (
            strcmp(argv[i], "-t") == 0
            || strcmp(argv[i], "--threads") == 0
        ) {
            if (i + 1 == argc) {
                fprintf(stderr, "No thread count found after threads flag.\n");
                return 2;
            }
            conf->threadCount = 0;
            for (int j = 0; j < strlen(argv[++i]); j++) {
                if (argv[i][j] < '0' || argv[i][j] > '9') {
                    fprintf(stderr, "Invalid number after threads flag.\n");
                    return 2;
                }
                conf->threadCount *= 10;
                conf->threadCount += argv[i][j] - '0';
            }
        } else if (
            strcmp(argv[i], "-f") == 0
            || strcmp(argv[i], "--force") == 0
        ) conf->forceFlag = true;
        else if (
            strcmp(argv[i], "-d") == 0
            || strcmp(argv[i], "--debug") == 0
        ) conf->debugFlag = true;
        else {
            if (conf->debugFlag) printf("[TTDebug] Read \"%s\" as executable.\n", argv[i]);
            if (conf->pathCount >= 32) {
                fprintf(stderr, "Inputted more executables than maximum amount.");
                return 2;
            }
            conf->pathIndexes[conf->pathCount++] = i;
        }
    }
    return 0;
}
