Name: Megha Sravani Lavu
CSU ID: 2762646
~~~~~~~~~~~~~~~~~~~~
Project Description:
~~~~~~~~~~~~~~~~~~~~

Assumptions:

// For option 2 memory size should be of size which is power of 2.
// pid is unique for each process


Option 1 is for segmentation:

The List of Holes are maintained in linked list name hole.

   ->  It stores the hole base location, size of hole and pointer to next hole.

The List of process are also maintained in linked list named segmentlist.

   -> It stores the size of the process, pid, text segment start location and its size,
      Data segment start location and its size, heap start location and its size and total 
      Allocated bytes for the process.

~~~~~~~~~~~~~~~
For allocation:
~~~~~~~~~~~~~~~

The functions segmentallocator, holeallocate are used for allocation.

I have chosen First Fit Policy to find a free region because it will give me the first free region faster than the best fit. This is implemented in holeallocate function.

In  best fit,  it will traverse through the hole list and then it will decide which will be best.

The segment allocator will check whether the size of the process and the sum of size requested for each segment are same or not. If not, segmentation error message is printed.

// if there is not enough space for all the three segments together my program will make that process fail and the count is maintained in failedsegalloc variable.

~~~~~~~~~~~~~~~~~
For deallocation:
~~~~~~~~~~~~~~~~~
The functions segmentdeallocator, addtoholelist, holecoalesc, deletehole are implemented.

 segmentdeallocator is Implemented using brute force algorithm where it will remove the process from its list and before doing that it will call add to hole list to add the space allocated for the process. Free block coalescing is achieved by holecoalesc and delete hole functions.

While deallocation , the process position is list is calculated and depending on that it will deallocate the process.

~~~~~~~~~~~~~~~~
For printtstate:
~~~~~~~~~~~~~~~~

printholelist and printsegmentlist functions are used.

~~~~~~~~~~~~~~~~~~~~~~~
Option 2 is for paging:
~~~~~~~~~~~~~~~~~~~~~~~

The list of process are stored in pagelist.
 -> It includes bytes(number of bytes used for process), pid(processid),pagenum(number of page numbers used for that particular process),page(virtual page number),frame(which frame is used in physical memory), used(list of pages used for this process), pointer to next process data.

Pagetable[] will store details whether that page is used or not.This can hold max 5000 pages only.

~~~~~~~~~~~~~~~
For allocation:
~~~~~~~~~~~~~~~

I used the First available Frame for allocation to the process.


~~~~~~~~~~~~~~~~~
For deallocation:
~~~~~~~~~~~~~~~~~

All the pages related to that process will be initialised to -1 that means u can use this page in future.

Invalid option if u choose other than 1 or 2.



~~~~~~~~~~~~~~~~~~~~~~
Compiling Instruction:
~~~~~~~~~~~~~~~~~~~~~~

gcc melavu.c 
./a.out Input.txt


~~~~~~~~~~~~~~~~~~
Execution Results:
~~~~~~~~~~~~~~~~~~


For segmentation:
--------------
Test case 1:
--------------

Input Text file contains:

1024 1
A 234 1 80 100 54
A 458 2 300 98 60
A 30  3 15 8 7
D 1
A 890 4 200 450 240
D 3
P
A 70 5 55 5 10
D 2
D 5
D 4
P

Output:

% gcc melavu.c
% ./a.out Input.txt
Memory size=1024 bytes, Allocated bytes=458, free=566
There are currently 2 holes and 1 process
Hole List:
        hole=1: startlocation=0, size=332
        hole=2, startlocation=790, size=234
Process List:
process id=2, size=458, allocation=458
        text start=490, size=300
        data start=392, size=98
        heap start=332, size=60
Failed allocations[No memory]= 1
Memory size=1024 bytes, Allocated bytes=0, free=1024
There are currently 1 holes and 0 process
Hole List:
        hole=1, startlocation=0, size=1024
no Active process
Failed allocations[No memory]= 1

-----------
Testcase 2:
-----------

Input file contains:

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

Output:

% ./a.out Input.txt
Memory size=8192 bytes, Allocated bytes=488, free=7704
There are currently 2 holes and 2 process
Hole List:
        hole=1: startlocation=0, size=7470
        hole=2, startlocation=7958, size=234
Process List:
process id=2, size=458, allocation=458
        text start=7658, size=300
        data start=7560, size=98
        heap start=7500, size=60
process id=3, size=30, allocation=30
        text start=7485, size=15
        data start=7477, size=8
        heap start=7470, size=7
Failed allocations[No memory]= 0
Memory size=8192 bytes, Allocated bytes=1348, free=6844
There are currently 3 holes and 2 process
Hole List:
        hole=1: startlocation=0, size=6580
        hole=2: startlocation=7958, size=234
        hole=3, startlocation=7470, size=30
Process List:
process id=2, size=458, allocation=458
        text start=7658, size=300
        data start=7560, size=98
        heap start=7500, size=60
process id=4, size=890, allocation=890
        text start=7270, size=200
        data start=6820, size=450
        heap start=6580, size=240
Failed allocations[No memory]= 0
Memory size=8192 bytes, Allocated bytes=0, free=8192
There are currently 1 holes and 0 process
Hole List:
        hole=1, startlocation=0, size=8192
no Active process
Failed allocations[No memory]= 0
%

-----------
For paging:
-----------
------------
Test case 1:
------------

Input file contains:

1024 2
A 234 1 80 100 54
A 458 2 300 98 60
A 30  3 15 8 7
D 1
A 890 4 200 450 240
D 3
P
A 70 5 55 5 10
D 2
D 5
D 4
P

Output:

% ./a.out Input.txt
Memory size=1024 bytes, total pages=32
allocatedpages=15, free pages=17
 There are currently 1 active process
Free pagelist:0,1,2,3,4,5,6,7,23,24,25,26,27,28,29,30,31,
Process List:
processid=2, size=458, number of pages=15
 virt page 0 -> phys page 8 used: 32 bytes
 virt page 1 -> phys page 9 used: 32 bytes
 virt page 2 -> phys page 10 used: 32 bytes
 virt page 3 -> phys page 11 used: 32 bytes
 virt page 4 -> phys page 12 used: 32 bytes
 virt page 5 -> phys page 13 used: 32 bytes
 virt page 6 -> phys page 14 used: 32 bytes
 virt page 7 -> phys page 15 used: 32 bytes
 virt page 8 -> phys page 16 used: 32 bytes
 virt page 9 -> phys page 17 used: 32 bytes
 virt page 10 -> phys page 18 used: 32 bytes
 virt page 11 -> phys page 19 used: 32 bytes
 virt page 12 -> phys page 20 used: 32 bytes
 virt page 13 -> phys page 21 used: 32 bytes
 virt page 14 -> phys page 22 used: 10 bytes

Failed allocations[No memory]=1
Memory size=1024 bytes, total pages=32
allocatedpages=0, free pages=32
 There are currently 0 active process
Free pagelist:0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
Failed allocations[No memory]=1
%


------------
Test case 2:
------------

Input file contains:

8192 2
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

Output:


% ./a.out Input.txt
Memory size=8192 bytes, Allocated bytes=488, free=7704
There are currently 2 holes and 2 process
Hole List:
        hole=1: startlocation=0, size=7470
% ./a.out Input.txt
Memory size=8192 bytes, total pages=256
allocatedpages=16, free pages=240
 There are currently 2 active process
Free pagelist:0,1,2,3,4,5,6,7,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,
Process List:
processid=2, size=458, number of pages=15
 virt page 0 -> phys page 8 used: 32 bytes
 virt page 1 -> phys page 9 used: 32 bytes
 virt page 2 -> phys page 10 used: 32 bytes
 virt page 3 -> phys page 11 used: 32 bytes
 virt page 4 -> phys page 12 used: 32 bytes
 virt page 5 -> phys page 13 used: 32 bytes
 virt page 6 -> phys page 14 used: 32 bytes
 virt page 7 -> phys page 15 used: 32 bytes
 virt page 8 -> phys page 16 used: 32 bytes
 virt page 9 -> phys page 17 used: 32 bytes
 virt page 10 -> phys page 18 used: 32 bytes
 virt page 11 -> phys page 19 used: 32 bytes
 virt page 12 -> phys page 20 used: 32 bytes
 virt page 13 -> phys page 21 used: 32 bytes
 virt page 14 -> phys page 22 used: 10 bytes
processid=3, size=30, number of pages=1
 virt page 0 -> phys page 23 used: 30 bytes

Failed allocations[No memory]=0
Memory size=8192 bytes, total pages=256
allocatedpages=43, free pages=213
 There are currently 2 active process
Free pagelist:23,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,
Process List:
processid=2, size=458, number of pages=15
 virt page 0 -> phys page 8 used: 32 bytes
 virt page 1 -> phys page 9 used: 32 bytes
 virt page 2 -> phys page 10 used: 32 bytes
 virt page 3 -> phys page 11 used: 32 bytes
 virt page 4 -> phys page 12 used: 32 bytes
 virt page 5 -> phys page 13 used: 32 bytes
 virt page 6 -> phys page 14 used: 32 bytes
 virt page 7 -> phys page 15 used: 32 bytes
 virt page 8 -> phys page 16 used: 32 bytes
 virt page 9 -> phys page 17 used: 32 bytes
 virt page 10 -> phys page 18 used: 32 bytes
 virt page 11 -> phys page 19 used: 32 bytes
 virt page 12 -> phys page 20 used: 32 bytes
 virt page 13 -> phys page 21 used: 32 bytes
 virt page 14 -> phys page 22 used: 10 bytes
processid=4, size=890, number of pages=28
 virt page 0 -> phys page 0 used: 32 bytes
 virt page 1 -> phys page 1 used: 32 bytes
 virt page 2 -> phys page 2 used: 32 bytes
 virt page 3 -> phys page 3 used: 32 bytes
 virt page 4 -> phys page 4 used: 32 bytes
 virt page 5 -> phys page 5 used: 32 bytes
 virt page 6 -> phys page 6 used: 32 bytes
 virt page 7 -> phys page 7 used: 32 bytes
 virt page 8 -> phys page 24 used: 32 bytes
 virt page 9 -> phys page 25 used: 32 bytes
 virt page 10 -> phys page 26 used: 32 bytes
 virt page 11 -> phys page 27 used: 32 bytes
 virt page 12 -> phys page 28 used: 32 bytes
 virt page 13 -> phys page 29 used: 32 bytes
 virt page 14 -> phys page 30 used: 32 bytes
 virt page 15 -> phys page 31 used: 32 bytes
 virt page 16 -> phys page 32 used: 32 bytes
 virt page 17 -> phys page 33 used: 32 bytes
 virt page 18 -> phys page 34 used: 32 bytes
 virt page 19 -> phys page 35 used: 32 bytes
 virt page 20 -> phys page 36 used: 32 bytes
 virt page 21 -> phys page 37 used: 32 bytes
 virt page 22 -> phys page 38 used: 32 bytes
 virt page 23 -> phys page 39 used: 32 bytes
 virt page 24 -> phys page 40 used: 32 bytes
 virt page 25 -> phys page 41 used: 32 bytes
 virt page 26 -> phys page 42 used: 32 bytes
 virt page 27 -> phys page 43 used: 26 bytes

Failed allocations[No memory]=0
Memory size=8192 bytes, total pages=256
allocatedpages=0, free pages=256
 There are currently 0 active process
Free pagelist:0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,
Failed allocations[No memory]=0
%


