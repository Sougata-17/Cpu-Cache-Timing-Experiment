/*
 * cache_timing.c
 * ===============
 * CPU Cache Timing Experiment
 *
 * What this does:
 *   Creates arrays of different sizes and measures how long
 *   it takes to read them. Small arrays fit in fast cache (L1/L2/L3).
 *   Large arrays spill into slow RAM. You will SEE the speed difference.
 *
 * Build:   gcc -O0 -o cache_timing cache_timing.c
 * Run:     ./cache_timing
 *
 * IMPORTANT: Use -O0 so the compiler does not skip your code.
 */

#include <stdio.h>      /* printf                */
#include <stdlib.h>     /* malloc, free          */
#include <string.h>     /* memset                */
#include <time.h>       /* clock_gettime         */

/* -- Step 1: A helper to get current time in nanoseconds --------- */

long long get_time_ns()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

/* -- Step 2: Read the array and measure how long it takes -------- */

long long measure(int *array, int size_in_ints)
{
    long long start, end;
    int sum = 0;   /* 'sum' stops the compiler removing our loop */

    start = get_time_ns();
    
    int i;

    /* Read every element in the array once */
    for (i = 0; i < size_in_ints; i++) {
        sum += array[i];
    }

    end = get_time_ns();

    /* This line stops the compiler from skipping the loop */
    if (sum == -999999) printf("ignore this: %d\n", sum);

    /* Return total time taken in nanoseconds */
    return end - start;
}

/* -- Step 3: Main program ---------------------------------------- */

int main()
{
    printf("\n");
    printf("======================================\n");
    printf("   CPU Cache Timing Experiment\n");
    printf("======================================\n");
    printf("\n");
    printf("%-15s %-15s %-20s\n", "Array Size", "Time (ns)", "Speed Observation");
    printf("%-15s %-15s %-20s\n", "----------", "---------", "-----------------");

    /*
     * Test these sizes:
     *   1 KB   -> fits in L1 cache  (fastest)
     *   8 KB   -> fits in L1 cache
     *   64 KB  -> fits in L2 cache
     *   512 KB -> fits in L2/L3 cache
     *   4 MB   -> fits in L3 cache
     *   16 MB  -> might spill to RAM
     *   128 MB -> definitely in RAM  (slowest)
     */

    int sizes_kb[] = {1, 8, 64, 512, 4096, 16384, 131072};
    int num_sizes  = 7;
    
    int s;

    for (s = 0; s < num_sizes; s++) {

        /* Convert KB to number of integers */
        int size_bytes = sizes_kb[s] * 1024;
        int size_ints  = size_bytes / sizeof(int);

        /* Allocate the array */
        int *array = (int *)malloc(size_bytes);
        if (array == NULL) {
            printf("Not enough memory for %d KB. Skipping.\n", sizes_kb[s]);
            continue;
        }

        /* Fill array with 1s so reading gives real work */
        memset(array, 1, size_bytes);

        /* Measure 3 times and take the best (most accurate) result */
        long long best_time = 999999999999LL;
        
        int trial;
        for (trial = 0; trial < 3; trial++) {
            long long t = measure(array, size_ints);
            if (t < best_time) best_time = t;
        }

        /* Figure out which cache level this probably hit */
        const char *observation;
        if      (sizes_kb[s] <= 32)    observation = "<-- L1 cache (fastest!)";
        else if (sizes_kb[s] <= 256)   observation = "<-- L2 cache";
        else if (sizes_kb[s] <= 8192)  observation = "<-- L3 cache";
        else                           observation = "<-- RAM (slowest!)";

        /* Print the size in a readable way */
        char size_label[20];
        if (sizes_kb[s] >= 1024)
            sprintf(size_label, "%d MB", sizes_kb[s] / 1024);
        else
            sprintf(size_label, "%d KB", sizes_kb[s]);

        printf("%-15s %-15lld %s\n", size_label, best_time, observation);

        free(array);
    }

    printf("\n");
    printf("What to observe:\n");
    printf("  - Small arrays (1-64 KB) should be FAST (low ns)\n");
    printf("  - Large arrays (16MB+)   should be SLOW (high ns)\n");
    printf("  - You will see the time JUMP at certain sizes.\n");
    printf("  - That jump = your cache boundary!\n");
    printf("\n");

    return 0;
}
