# Project 3: Parallel Cryptocurrency Miner

See instructions here: https://www.cs.usfca.edu/~mmalensek/cs220/assignments/project-3.html

## Written Responses

Edit this readme.md file to answer the following questions:

### Busy-waiting vs. Condition Variables

Once you have condition variables working in your program, compare the performance of busy waiting with condition variables. To ensure a fair comparison, use a single thread.

Report for the original program as well as modified version:

1. Total program run time
2. Hashes per second
3. CPU usage during execution (use the `top` command to get a rough estimate of the average CPU us10ge)

Original:
1. 0.24s
2. 722.54 hashes/sec
3. 10

Modified:
1. 0.01s
2. 290234.56 hashes/sec
3. 7.5

### Nonces Per Task

Experiment with different values for `NONCES_PER_TASK`. What value yields the best performance in terms of hashes per second on your machine?

1000 NONCES PER TASK
