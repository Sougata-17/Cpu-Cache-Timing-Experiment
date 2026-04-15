#include <stdio.h>      
#include <stdlib.h>    
#include <string.h>     
#include <time.h>       

/* Step 1: A helper to get current time in nanoseconds */

long long get_time_ns()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

/* Step 2: Read the array and measure how long it takes */

long long measure(int *array, int size_in_ints)
{
    long long start, end;
    int sum = 0;   

    start = get_time_ns();
    
    int i;

    for (i = 0; i < size_in_ints; i++) {
        sum += array[i];
    }

    end = get_time_ns();

    if (sum == -999999) printf("ignore this: %d\n", sum);

    return end - start;
}

/* Step 3: Main program */

int main()
{
    printf("\n");
    printf("======================================\n");
    printf("   CPU Cache Timing Experiment\n");
    printf("======================================\n");
    printf("\n");
    printf("%-15s %-15s %-20s\n", "Array Size", "Time (ns)", "Speed Observation");
    printf("%-15s %-15s %-20s\n", "----------", "---------", "-----------------");


    int sizes_kb[] = {1, 8, 64, 512, 4096, 16384, 131072};
    int num_sizes  = 7;
    
    int s;

    for (s = 0; s < num_sizes; s++) {

        int size_bytes = sizes_kb[s] * 1024;
        int size_ints  = size_bytes / sizeof(int);

        int *array = (int *)malloc(size_bytes);
        if (array == NULL) {
            printf("Not enough memory for %d KB. Skipping.\n", sizes_kb[s]);
            continue;
        }

        memset(array, 1, size_bytes);

        long long best_time = 999999999999LL;
        
        int trial;
        for (trial = 0; trial < 3; trial++) {
            long long t = measure(array, size_ints);
            if (t < best_time) best_time = t;
        }

        const char *observation;
        if      (sizes_kb[s] <= 32)    observation = "<-- L1 cache (fastest!)";
        else if (sizes_kb[s] <= 256)   observation = "<-- L2 cache";
        else if (sizes_kb[s] <= 8192)  observation = "<-- L3 cache";
        else                           observation = "<-- RAM (slowest!)";

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
