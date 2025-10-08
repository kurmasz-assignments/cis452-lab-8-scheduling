CIS 452 Lab 8:  Process Scheduling in Linux
--------------------------------------------

### Overview

The goal of this lab is to explore process scheduling in Linux and to explore
system calls that allow users to modfiy scheduling.

### Processor Affinity

As discussed in class,
an operating system may give users the option of requesting CPU affinity --
specifying a specific processor or subset of processors that a thread should
run on.
One reason this may be helpful is to make better use of the cache.
However, a process will not necessarily run faster simply because it has
affinity for a particular CPU.

We can use system calls and the `top` process to observe CPU affinity in action.
Sample Program 1 is designed to accept a single Boolean argument (`0` or `1`)
determining whether to set CPU affinity.
The program then forks a number of children
and finally performs some useless work to keep the processor(s) busy long
enough for us to observe interesting trends.

* Determine the system call that allows you to set the affinity of a process
  to a specifc CPU.
  Modify the sample program below to incorporate the correct call.
  **Be sure to include the line you used in your writeup.**

*Sample Program 1*

```c
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
```

1. Depending on whether or not affinity is set,
   is the program faster, slower, or about the same?

2. Approximately how large is the difference?  (Two times? Ten times?)

3. Explain **why** you observed the performance that you did.

4. Explain how you can use the `top` program to verify that your affinity request has been honored. 

5. Predict what will happen if you add a second CPU to `set`.

6. Add a second CPU to `set` and verify.  (Include a screen shot of `top`.)


### Yielding

Determine the purpose of the system call `sched_yield` --
you can review `sched(7)`, `sched_yield(2)`, or any other source you prefer.
Then, consider Sample Program 2 below.
Once again, the primary purpose of the program is to perform meaningless work
that lets us observe scheduling behavior.

*Sample Program 2*

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#define BIG_NUM 50000000

int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "Program requires an integer argument.\n");
        exit(1);
    }

    long sum = 0;
    int argint = atoi(argv[1]);
    for (long i = 0; i < BIG_NUM; i++) {
        if (argint > 0) {
            sched_yield();
        }
        sum += i;
    }

}
```

* Run the program with `0` as a parameter so that the yield does not occur.
* Now, run the program with `1` as a parameter.  Verify that calling `yield` 
  slows the program by a factor of at least 10. (Think about how you can 
  verify this without waiting for the program to terminate. )


7. Explain how you verified that the slowdown was at least 10x.

8. Run the program with `1` as a parameter again.  This time have `top` open in a different 
window.  What percentage of the CPU does the program use?

9. Why does the program take so long to run, even though it's access to the CPU does not appear limited? 
   Hint: Reduce `BIG_NUM` by a factor of 10, then use `time` to measure the time needed to run the process with yields.

10. With `top` open, run the program with `0` in one terminal and with `1` in another. What percentage of the CPU does the 
yielding process get? Is that what you expected?  Or does the value seem either too high or too low?  Why?

11. How would you modify the program so that it still defers to other processes by calling `sched_yield`, but isn't absurdly slow 
(compared to the non-yielding version)?



### Experimenting with Policies

Study the `man` page `man 7 sched`.
Focus on the sections titled
`API summary`, `Scheduling policies`,  `SCHED_OTHER` (you do not need to read the part about `nice` values),
`SCHED_IDLE`, and `SCHED_BATCH`.

* Design and run an experiment to demonstrate the relative priorities of processes
with these three scheduling policies. It is **not** enough simply to look up the answer in the `man` pages --
  you must write code that confirms the expected behavior.

Your experiment can be heavily based on code in the previous parts of the lab.
Overall, the experiment should focus on
(1) keeping the CPU busy, and
(2) running processes with different scheduling policies.

A word of caution:
the experiment works only if processes with multiple types of scheduling
*are running on the processor at the same time*.

12. Your writeup should include your source code,
    a description of how you ran the program(s),
    and any relevant screenshots.
    Your description should be clear enough that I can repeat your experiment
    if I choose to do so.
