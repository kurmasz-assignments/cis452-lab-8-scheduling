/**********************************************************
 *
 * sampleProgram1.c
 * CIS 452 Lab 8
 *
 * !!!NAMES!!!
 *************************************************************/
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NLOOPS 500000000
#define NUM_PROCS 8

int main(int argc, char *argv[])
{
    cpu_set_t set;
    int joinedCPU = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s {0|1}\n", argv[0]);
        exit(1);
    }

    CPU_ZERO(&set);
    CPU_SET(joinedCPU, &set);

    if (atoi(argv[1]) != 0) {

        // ADD LINE HERE TO SET AFFINITY
    }

    pid_t pid;
    for (int i = 0; i < NUM_PROCS - 1; i++) {
        if ((pid = fork()) < 0) {
            perror("Problem forking");
            exit(1);
        } else if (!pid) {
            break;
        }
    }

    long sum = 0;
    for (long j = 0; j < NLOOPS; j++) {
        sum += j;  // meaningless work
    }

    return 0;
}