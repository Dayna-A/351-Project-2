# Project 1 - System Simulation 
A project for CPSC 351 Operating Systems at CSUF

## Created by 
[Dayna Anderson](dayna.anderson@csu.fullerton.edu)

[Stephen Lastimosa](slastimosa@csu.fullerton.edu)

[Ian Alvarez](ian_alvarez23@csu.fullerton.edu)

## Requirements
Write a simulation that explores the effects of limited memory and memory management policies. Your simulator will read policy information and the characteristics of the workload from input files and then will simulate the execution of the processes as well as decisions of Memory Manager (MM). The simulator will generate an output file with the trace of important events, as well as the memory map and the input queue status after each event. At the end of the simulation, your program will also print the average turnaround time per process. 

Your simulator will prompt the user for the size of the memory and associated parameters information. The Memory Size (an integer) denotes the capacity of the main memory in your simulation (it can be interpreted as a multiple of Kbytes). The policy parameter will denote the page and frame size. You can assume that the Memory Size will always be a multiple of the page/frame size. The workload file will first have an integer value N that tells you how many processes are defined in the file. 
The characteristics of each individual process will include: a unique process id on the first the system (Arrival Time) and its lifetime after it is admitted to the main memory (Lifetime in Memory) on the second line, and finally, its memory requirements (Address Space). Processes in the file will be listed in arrival order. If two processes have the same arrival time, the process listed first in the file goes on the input queue first. 
The Address Space line is a sequence of one or more integers separated by a single blank. The first integer gives the number of 'pieces' of memory on the line. You simply sum these integers to get the overall space requirement. 
Note that since the memory is limited, a process may have to wait until the system can accommodate its memory requirements. The lifetime in memory information for a given process defines how long the process will run once it has been given space in main memory. The memory space for a process will be freed by the memory manager when it completes. 
Your simulator must generate output (to the screen) that explicitly shows the trace of important events that modify the memory contents and input queue, including: 
* Process arrival 
* Process admission to the main memory 
* Process completion 

For this simulation, you can keep an integer (or long) variable representing your virtual clock. Then you can increment its value until all the processes have completed. Each arriving process will be first put to Input Queue. Processes in Input Queue will be ordered according to their arrival times. Whenever a process completes or a new process arrives, the memory manager (MM) must be invoked. In case of a completion, MM will first adjust the memory map to reflect the fact that the memory region(s) previously allocated to the process is/are now free. After that, (and also when a new process arrives) MM will check to see if it can move the process at the head of the input queue to the memory. If so, it will make the allocation and it will update the input queue. Then it will check whether the head of the updated input queue (new head) can be also moved to the main memory, and so on. Even if the current commitments in memory do not allow MM to admit the process at position X of Input Queue, MM should try to load other processes at positions X + 1,X + 2, …, and in that order. 
The simulation should terminate when there are no more processes in the input queue or time reaches 100,000.

## Implementation
Memory Map Data structures used:

- A "Page" struct was created with 5 integer fields to hold the page's: start address, end address, process id, page number, arrival time and completion time.

 - A vector of page structs was created and (memory size/page size) number of entries were initialized into the vector to simulate a dynamically allocated multidimensional array without the need for pointer use. After the initialization no further pages were added to the vector.

## Contributions: 

Dayna Anderson:     - Implementation of Project2.cpp

Stephen Lastimosa:  - Implementation of linux co

Ian Alvarez:        - Implementation of design doc

## Execution
```
$ make 
$ ./simulator in1.txt
```