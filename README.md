# Project 3: Parallel Cryptocurrency Miner

See instructions here: https://www.cs.usfca.edu/~mmalensek/cs220/assignments/project-3.html

## Written Responses

Edit this readme.md file to answer the following questions:

### Busy-waiting vs. Condition Variables

Once you have condition variables working in your program, compare the performance of busy waiting with condition variables. To ensure a fair comparison, use a single thread.

Report for the original program as well as modified version:

1. Total program run time
2. Hashes per second
3. CPU usage during execution (use the `top` command to get a rough estimate of the average CPU usage)

Original:
1. 0.00s
2. 722.54 hashes/sec
3. 10

Modified:
1. 0.24s
2. 290234.56 hashes/sec
3. 7.5

Which version performs better? Is this the result you expected? Why or why not?
The modified version was better, this is not what I expected as the busy waiting is supposed to add to the time since you are waiting for the thread to be finished to continue on. 

### Nonces Per Task

Experiment with different values for `NONCES_PER_TASK`. What value yields the best performance in terms of hashes per second on your machine?

25 NONCES PER TASK

### Performance I

When evaluating parallel programs, we use speedup and efficiency. Why are these metrics not as useful when measuring the performance of our parallel crytocurrency miner?

These metrics are not as useful because we are using threads to find our hashes. 

### Performance II

Using any of the `kudlick` machines (in our 220 classroom), what is the highest performance you were able to achieve in terms of hashes per second? What configuration did you use (`NONCES_PER_TASK`, number of threads, block data, compiler options)?

252581.97 hashes/sec - 25 NONCES_PER_TASK, 1 thread, 5 Difficulty 
