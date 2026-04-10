##CPU Cache Timing Experiment

#A C program that measures memory access latency across the CPU cache hierarchy (L1 → L2 → L3 → RAM) to demonstrate the timing side-channel principle underlying CPU–GPU security vulnerabilities.

#What this project does
When a CPU reads data from memory, the time taken depends on where that data lives:

L1 cache — tiny, fastest, inside the CPU core
L2 cache — slightly larger, slightly slower
L3 cache — shared across cores, slower
RAM — large, but ~100x slower than L1

This program creates arrays of increasing sizes and measures how long each takes to read. The timing jumps you see are real hardware cache boundaries — and these same timing differences are the foundation of cache side-channel attacks in modern systems.


#Why this matters for security
Timing differences between cache levels are exploited in attacks like Flush+Reload and Prime+Probe. In CPU–GPU coupled systems, the CPU manages all GPU memory transfers. A timing-aware attacker process can observe these latency patterns to infer:

   1. What workload the GPU is running (e.g. image classification vs LLM inference)
   2. Approximate model or data sizes
   3. Memory access patterns of a co-running process

This project is a first-principles, beginner-level demonstration of that vulnerability class — built in ~80 lines of C with no external libraries.


Motivation
Built as preparation for research in CPU–GPU security analysis, specifically studying cross-layer vulnerabilities in heterogeneous computing systems.

Tools

Language: C
Compiler: GCC
Timer: clock_gettime(CLOCK_MONOTONIC)
OS: Windows (Dev C++)
