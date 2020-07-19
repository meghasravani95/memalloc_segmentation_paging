# memory management

The goal of this lab is to write a simple memory management simulator based on the topics covered in class. You must write a memory manager that supports both segmentation and paged memory allocation. For simplicity, assume that processes do not grow or shrink, and that no compaction is performed by the memory manager.

Segmentation: 
Write a segmentation based allocator which allocates three segments for each process: text, data, and heap. The memory region within each segment must be contiguous, but the three segments do not need to be placed contiguously. Instead, you should use either a best fit, first fit, or worst fit memory allocation policy to find a free region for each segment. The policy choice is yours, but you must explain why you picked that policy in your README. When a segment is allocated within a hole, if the remaining space is less than 16 bytes then the segment should be allocated the full hole. This will cause some internal fragmentation but prevents the memory allocator from having to track very small holes.

You should consider the following issues while designing your memory manager:

Efficiency of your search algorithms: 
              First-fit, worst-fit, and best-fit all require you to search for an appropriate hole to accommodate the new process. You should pay careful attention to your data structures and search algorithms. For instance, keeping the list of holes sorted by size and using binary search to search for a hole might improve efficiency of your best-fit algorithms. We do not require you to use a specific algorithm/data structure to implement the selected policy; you have the flexibility of using any search algorithm/data structure that is efficient. We'll give you 20 points for any design that is more efficient that a brute-force linear search through an unsorted list of holes. Similarly, use an efficient search algorithm when deallocating a process from the processList. Explain all design decisions, the data structures and search algorithms used clearly in the README file.
              
Free block coalescing: 
            When a process terminates, the memory allocated to that process is returned to the list of holes. You should take care to combine (coalesce) holes that are adjacent to each other and form a larger contiguous hole. This will reduce the degree of fragmentation incurred by your memory manager
Paging: Next write a paging based memory allocator. This should split the system memory into a set of fixed size 32 byte pages, and then allocate pages to each process based on the amount of memory it needs. Paging does not require free block coalescing, but you should explain in your README your choice of algorithm and data structure for tracking free pages.


Data structures
            Both of your memory managers should maintain a processList that lists all currently active processes, the process Id and size of each process. For the segmentation allocator, you should also track the start location of each segment. For the paging allocator you must track what physical page is mapped to each virtual page within the process, as well as the number of bytes used in each page.

You are free to use any data structures (arrays, linked list, doubly linked list, etc) to implement these lists, but you should explain why you pick the data structure you choose. This decision will also affect the use of search algorithms in the segmentation allocator.

Input file
You program should take input from a file and perform actions specified in the file, while printing out the result of each action. The format of the input file is as follows:
 
 
memorySize policy    //initialize memory to this size and use this policy
A  size pid text data heap  // allocate so much memory to this process (split into segments if using Segmentation) 
D   pid                // deallocate memory for this process
P                        // print current state of memory
 
An actual file may look as follows

 
8192 1
A 234 1 80 100 54
A 458 2 300 98 60
A 30  3 15 8 7
D 1
P
A 890 4 200 450 240
D 3
P
A 70 5 55 5 10
D 2
D 5
D 4
P

Output file should look like:

SEGMENTATION Example: 
  // Memory size = 1024 bytes, allocated bytes = 179, free = 845
  // There are currently 10 holes and 3 active process
  // Hole list:
  // hole 1: start location = 0, size = 202
  // ...
  // Process list:
  // process  id=34, size=95 allocation=95
  //    text start=202, size=25
  //    data start=356, size=16
  //    heap start=587, size=54
  // process id=39, size=55 allocation=65
  // ...
  // Failed allocations (No memory) = 2
  // 
  
  // PAGING Example:
  // Memory size = 1024 bytes, total pages = 32 
  // allocated pages = 6, free pages = 26
  // There are currently 3 active process
  // Free Page list:
  //   2, 6, 7, 8, 9, 10, 11, 12...
  // Process list:
  //  Process id=34, size=95 bytes, number of pages=3
  //    Virt Page 0 -> Phys Page 0  used: 32 bytes
  //    Virt Page 1 -> Phys Page 3  used: 32 bytes
  //    Virt Page 2 -> Phys Page 4  used: 31 bytes
  //  Process id=39, size=55 bytes, number of pages=2
  //    Virt Page 0 -> Phys Page 1  used: 32 bytes
  //    Virt Page 1 -> Phys Page 13  used: 23 bytes
  //  Process id=46, size=29 bytes, number of pages=1
  //    Virt Page 0 -> Phys Page 5  used: 29 bytes
  //
  // Failed allocations (No memory) = 2
