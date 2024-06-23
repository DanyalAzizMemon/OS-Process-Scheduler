# OS-Process-Scheduler

## Description
This repository contains the implementation of a Process Scheduler using Multilevel Feedback Queues and Waiting Queues in C++. The scheduler simulates various process states and transitions to efficiently manage CPU resources.

## Files
- **ProcessScheduler.h**: The header file containing all the code for the process scheduler.

## Assignment Description
### Process Concept
A process is an active entity with attributes including CPU, Memory, and Hardware state. The process can be in one of the five states:
1. **New**: Process is created and stored in the Process List.
2. **Ready**: Process can be executed anytime if CPU is assigned.
3. **Running**: Instructions of the process are being executed.
4. **Waiting**: Waiting for the completion of an I/O operation.
5. **Terminated**: The process is finished, and its memory and resources are deallocated.

### Process Scheduling
The scheduler is implemented using Multilevel Feedback Queues and Waiting Queues:
- **Ready Queues**: Three queues with FCFS-1 (highest priority), FCFS-2 (medium priority), and SJF (lowest priority).
- **Waiting Queues**: Three queues for Hard Disk, Network, and Peripheral Devices using FCFS scheduling.

### CPU Time Management
The CPU runs a single instruction at a time. To avoid monopolization, CPU time is divided into time slices, and processes are scheduled based on their queue priorities.

### Global Time
The simulation uses a global time starting from 0. Each CPU instruction increments the global time by 1 tick. 

### Process List
A doubly linked list maintains the process information sorted by arrival time.

### Files Description
- **job.txt**: Contains process names and their arrival times.
- **ProcessName.txt**: Contains the instructions for each process (e.g., P0.txt, P1.txt, P2.txt).
- **Process.txt**: Tracks process states, queue residence, or CPU.
- **CPU.txt**: Tracks the process currently being executed and the instruction.
- **Queue.txt**: Lists processes in each queue.

## Function for Testing
```cpp
void runProcessScheduling(const char* PathofJobFile, int globalTick)
//1.PathofJobFile: Path to the job.txt file.
//2.globalTick: Simulation run time in global ticks.
