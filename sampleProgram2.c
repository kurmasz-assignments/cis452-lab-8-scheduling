/**********************************************************
 *
 * sampleProgram2.c
 * CIS 452 Lab 8
 *
 * !!!NAMES!!!
 *************************************************************/

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BIG_NUM 1000000000

int main(int argc, char **argv) {

   if (argc < 2) {
      fprintf(stderr, "Program requires an integer argument.\n");
      exit(1);
   }

   cpu_set_t set;
   int joinedCPU = 0;
   CPU_ZERO(&set);
   CPU_SET(joinedCPU, &set);

   if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) < 0) {
      perror("Problem setting affinity");
   }

   long sum = 0;
   int argint = atoi(argv[1]);
   if (argint == 0) {
      printf("Running %d iterations with no yield.\n", BIG_NUM);
   } else if (argint == 1) {
      printf("Running %d iterations with yield.\n", BIG_NUM);
   } else {
      printf("Running infinite loop with no yield.\n");
   }

   /* If argint == 2, loop forever */
   if (argint == 2) {
      while (1) {
         sum += 1;
      }
   } else {
      for (long i = 0; i < BIG_NUM; i++) {
         if (argint == 1) {
            sched_yield();
         }
         sum += i;
      }
   }
}
