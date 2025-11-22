#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Expected usage:
    // parta_main fcfs <burst...>
    // parta_main rr <quantum> <burst...>
    if (argc < 2) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    const char* mode = argv[1];
    int quantum = 0;
    int firstBurstArg = 2;

    if (strcmp(mode, "fcfs") == 0) {
        // fcfs requires at least one burst after the mode
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        firstBurstArg = 2;
    } else if (strcmp(mode, "rr") == 0) {
        // rr requires a quantum and at least one burst
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        quantum = atoi(argv[2]);
        firstBurstArg = 3;
        if (quantum <= 0) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
    } else {
        // Unknown mode
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    int blen = argc - firstBurstArg;
    int* bursts = malloc(sizeof(int) * blen);
    if (bursts == NULL) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }
    for (int i = 0; i < blen; i++) {
        bursts[i] = atoi(argv[firstBurstArg + i]);
    }

    struct pcb* procs = init_procs(bursts, blen);
    free(bursts);
    if (procs == NULL) {
        return 1;
    }

    if (strcmp(mode, "fcfs") == 0) {
        printf("Using FCFS\n\n");
        for (int i = 0; i < blen; i++) {
            printf("Accepted P%d: Burst %d\n", i, procs[i].burst_left);
        }
        fcfs_run(procs, blen);
    } else { // rr
        printf("Using RR(%d)\n\n", quantum);
        for (int i = 0; i < blen; i++) {
            printf("Accepted P%d: Burst %d\n", i, procs[i].burst_left);
        }
        rr_run(procs, blen, quantum);
    }

    double total_wait = 0.0;
    for (int i = 0; i < blen; i++) {
        total_wait += procs[i].wait;
    }
    double avg = (blen > 0) ? (total_wait / blen) : 0.0;
    printf("Average wait time: %.2f\n", avg);

    free(procs);
    return 0;
}
