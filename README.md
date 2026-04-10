#  CPU Cache Timing Experiment

A C program that measures memory access latency across the CPU cache hierarchy  
(L1 → L2 → L3 → RAM) to demonstrate the timing side-channel principle underlying CPU–GPU security vulnerabilities.


##  What This Project Does

When a CPU reads data from memory, the time taken depends on where that data lives:

- **L1 Cache** — tiny, fastest, inside the CPU core  
- **L2 Cache** — slightly larger, slightly slower  
- **L3 Cache** — shared across cores, slower  
- **RAM** — large, but ~100× slower than L1  

This program:

- Creates arrays of increasing sizes  
- Measures how long each takes to read  
- Highlights timing jumps corresponding to cache boundaries  

These timing differences are real hardware effects and form the basis of cache side-channel attacks.


##  Why This Matters for Security

Timing differences between cache levels are exploited in attacks such as:

- **Flush+Reload**
- **Prime+Probe**

In CPU–GPU coupled systems:

- The CPU manages GPU memory transfers  
- A timing-aware attacker can observe latency patterns  

This allows potential inference of:

1. GPU workload type (e.g., image classification vs LLM inference)  
2. Approximate model or data sizes  
3. Memory access patterns of co-running processes  

This project provides a **first-principles, beginner-level demonstration** of such vulnerabilities using ~80 lines of C code with no external libraries.


## Motivation

Built as preparation for research in:

> **CPU–GPU Security Analysis**  
> Focusing on cross-layer vulnerabilities in heterogeneous computing systems.

