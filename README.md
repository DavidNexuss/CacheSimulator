# A simple LRU N way and victim cache simulator

This utility allows you to simulate the behavior of either an lru cache or a victim cache.
Useful for doing ec and ac homework.

Cache hits are printed to stdout, additional information is printed to stderr.
Compile with make

Expected input:
(mode, 0 for direct mapped with n ways, 1 for victim cache with n slots) (blockSlots) (ways) ... blocks

**Example of 2 ways direct mapped cache with 10 blocks slots**

INPUT:  0 10 2 73 55 43 45 73 45 13 43 73 55 45 73 15 4
OUTPUT: 0 0 0 0 1 1 0 0 0 1 1 1 0 1

0 is for miss and 1 for hit
