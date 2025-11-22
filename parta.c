#include "parta.h"
#include <stdlib.h>
#include <stdio.h>
struct pcb* init_procs(int* bursts, int blen) {
    struct pcb* pcbs = malloc(sizeof(struct pcb) * blen);
    if (pcbs == NULL) {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }
    for (int i = 0; i < blen; i++) {
        pcbs[i].pid = i;
        pcbs[i].burst_left = bursts[i];
        pcbs[i].wait = 0;
    }
    return pcbs;
}

/**
 * TODO: Describe what the function does
 */
void printall(struct pcb* procs, int plen) {
    // Feel free to print however you want
    for (int i = 0; i < plen; i++) {
        printf("PID: %d, Burst Left: %d, Wait: %d\n", procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
}

/**
 * TODO: Describe what the function does
 * @param procs The array of process control blocks
 * @param plen The length of the procs array
 * @param current The index of the currently running process
 * @param amount The amount of time to run the current process
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    if (current < 0 || current >= plen || amount <= 0) {
        return;
    }
    int actual_run = procs[current].burst_left;
    if (actual_run > amount) {
        actual_run = amount;
    }
    if (procs[current].burst_left > 0) {
        procs[current].burst_left -= actual_run;
        if (procs[current].burst_left < 0) {
            procs[current].burst_left = 0;
        }
    }
    for (int i = 0; i < plen; i++) {
        if (i == current) continue;
        if (procs[i].burst_left > 0) {
            procs[i].wait += actual_run;
        }
    }
}

/**
 * TODO: Describe what the function does
 * @param procs The array of process control blocks
 * @param plen The length of the procs array
 * @return 0 on success
 * 
 */
int fcfs_run(struct pcb* procs, int plen) {
    // TODO: Complete and document
    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left > 0) {
            run_proc(procs, plen, i, procs[i].burst_left);
        }
    }
    return 0;
}

/**
 * TODO: Describe what the function does
 */
int rr_next(int current, struct pcb* procs, int plen) {
    // TODO: Complete and document
    int start = (current + 1) % plen;
    for (int i = 0; i < plen; i++) {
        int idx = (start + i) % plen;
        if (procs[idx].burst_left > 0) {
            return idx;
        }
    }
    return -1;
}
/**
 * TODO: Describe what the function does
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    // Run processes in round-robin order using `quantum` time slices.
    // Returns the total time taken to finish all processes (or 0 if none).
    if (procs == NULL || plen <= 0 || quantum <= 0) {
        return 0;
    }

    int total_time = 0;

    // Start by finding the first runnable process. Calling rr_next with
    // (plen - 1) causes it to start searching at index 0.
    int current = rr_next(plen - 1, procs, plen);
    while (current != -1) {
        int run_amount = procs[current].burst_left;
        if (run_amount > quantum) run_amount = quantum;

        run_proc(procs, plen, current, run_amount);
        total_time += run_amount;

        // Find the next process after `current` that still has work.
        current = rr_next(current, procs, plen);
    }
        return total_time;
}