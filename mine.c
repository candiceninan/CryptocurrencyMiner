/**
 * mine.c
 *
 * Parallelizes the hash inversion technique used by cryptocurrencies such as
 * bitcoin.
 *
 * Input:    Number of threads, block difficulty, and block contents (string)
 * Output:   Hash inversion solution (nonce) and timing statistics.
 *
 * Compile:  gcc -g -Wall mine.c -o mine
 *              (or run make)
 *           When your code is ready for performance testing, you can add the
 *           -O3 flag to enable all compiler optimizations.
 *
 * Run:      ./mine 4 24 'Hello CS 220!!!'
 *
 * Difficulty Mask: 00000000000000000000000011111111
 * Number of threads: 4
 * . (NOTE: a '.' appears every 1,000,000 hashes to track progress)
 * Solution found by thread 1:
 * Nonce: 1011686
 * Hash: 000000B976A3E2B94CB9AB668E0C9C727782787B
 * 1016000 hashes in 0.26s (3960056.52 hashes/sec)
 */

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "sha1.c"

// TODO ten nonces per task is fairly low. There is probably also a point where
// increasing it too much will reduce performance. You will need to experiment
// to find the optimal value.
#define NONCES_PER_TASK 10

pthread_mutex_t task_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t task_staging = PTHREAD_COND_INITIALIZER;
pthread_cond_t task_ready = PTHREAD_COND_INITIALIZER;

uint32_t difficulty_mask;
char *bitcoin_block_data;
uint64_t *task_pointer = NULL;
bool solution_found = false;

struct thread_info {
    pthread_t thread_handle;
    unsigned int thread_id;
    uint64_t num_inversions;
    uint64_t nonce;
    char solution_hash[41];
};

/** Function Prototypes */
double get_time();
void *mine(void *arg);
void print_binary32(uint32_t num);
/** ------------------- */

/* Function: get_time()
 *
 * Purpose: retrieves the current system time (in seconds).
 */
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

/* TODO documentation */
void print_binary32(uint32_t num) {
    int i;
    for (i = 31; i >= 0; --i) {
        uint32_t position = 1 << i;
        printf("%c", ((num & position) == position) ? '1' : '0');
    }
}

/* TODO documentation */
void *mine(void *arg) {

    struct thread_info *info = (struct thread_info *) arg;
    uint64_t *task_nonces = NULL;

    /* We'll keep on working until a solution for our bitcoin block is found */
    while (true) {

        while (task_pointer == NULL && solution_found == false) {
            /* We will busy wait here until task_pointer is not NULL. */
        }

        if (solution_found) {
            /* Another thread has found the solution. This thread can exit. */
            return NULL;
        }

        task_nonces = task_pointer;

        /* We have the task. Empty out our task_pointer so another thread can
         * receive a task. */
        task_pointer = NULL;

        int i;
        for (i = 0; i < NONCES_PER_TASK; ++i) {
            /* Convert the nonce to a string */
            char buf[21];
            sprintf(buf, "%llu", task_nonces[i]);

            /* Create a new string by concatenating the block and nonce string.
             * For example, if we have 'Hello World!' and '10', the new string
             * is: 'Hello World!10' */
            size_t str_size = strlen(buf) + strlen(bitcoin_block_data);
            char *tmp_str = malloc(sizeof(char) * str_size + 1);
            strcpy(tmp_str, bitcoin_block_data);
            strcat(tmp_str, buf);

            /* Hash the temporary (combined) string */
            uint8_t digest[20];
            sha1sum(digest, tmp_str);

            /* Clean up the temporary string */
            free(tmp_str);

            /* Take the front 32 bits of the hash (spit across four 8-bit
             * unsigned integers and combine them */
            uint32_t hash_front = 0;
            hash_front |= digest[0] << 24;
            hash_front |= digest[1] << 16;
            hash_front |= digest[2] << 8;
            hash_front |= digest[3];

            /* Check to see if we've found a solution to our block. We perform a
             * bitwise AND operation and check to see if we get the same result
             * back. */
            if ((hash_front & difficulty_mask) == hash_front) {
                solution_found = true;
                info->nonce = task_nonces[i];
                sha1tostring(info->solution_hash, digest);
                return NULL;
            }
        }

        free(task_nonces);
        info->num_inversions += NONCES_PER_TASK;
    }

    return NULL;
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Usage: %s threads difficulty 'block data (string)'\n", argv[0]);
        return EXIT_FAILURE;
    }

    // TODO we have hard coded the difficulty to 20 bits (0x0000FFF). This is a
    // fairly quick computation -- something like 28 will take much longer.  You
    // should allow the user to specify anywhere between 1 and 32 bits of zeros.
    difficulty_mask = 0x000000FF;
    printf("Difficulty Mask: ");
    print_binary32(difficulty_mask);
    printf("\n");

    /* We use the input string passed in (argv[3]) as our block data. In a
     * complete bitcoin miner implementation, the block data would be composed
     * of bitcoin transactions. */
    bitcoin_block_data = argv[3];
    // TODO we should probably check to make sure the user entered a valid
    // (non-empty) string...

    // TODO we have hard-coded the number of threads here because the
    // single-threaded version of the program can only run with one worker
    // thread a time. We'll need to create the number of threads specified by
    // the user and store them in an array. The thread_info struct contains a
    // pthread_t handle that we will use later to join the threads. Each struct
    // is passed in as the thread routine's argument.
    unsigned int num_threads = 1;
    printf("Number of threads: %d\n", num_threads);
    struct thread_info *thread = calloc(1, sizeof(struct thread_info));
    thread->thread_id = 0;
    pthread_create(&thread->thread_handle, NULL, mine, thread);

    double start_time = get_time();
    uint64_t current_nonce = 0;
    while (current_nonce < UINT64_MAX) {
        /* Let's create a new array to store the nonces. We will malloc enough
         * space to store NONCES_PER_TASK worth of nonces */
        uint64_t *nonces = malloc(sizeof(uint64_t) * NONCES_PER_TASK);
        int i;
        for (i = 0; i < NONCES_PER_TASK; ++i) {
            nonces[i] = current_nonce++;

            if (current_nonce % 1000000 == 0) {
                /* Print out '.' to show progress every 1m hashes: */
                printf(".");
                fflush(stdout);
            }
        }

        /* Nonces are ready to be consumed. We will wait for a consumer thread
         * to pick up the job. */
        pthread_mutex_lock(&task_mutex);
        while (task_pointer != NULL && solution_found == false) {
            /* When task_pointer is not NULL, the task has not been picked up by
             * a consumer yet. We will wait until a consumer is ready. */
            // TODO we are just busy waiting here. In your multi-threaded
            // version of the program, you should wait on a condition variable.
            //pthread_cond_wait(&task_staging, &task_mutex);
        }

        if (solution_found == true) {
            break;
        }

        /* We have acquired a mutex on task_mutex. We can now update the pointer
         * to point to the task we just generated */
        task_pointer = nonces;

        /* Tell the consumer a new task is ready */
        pthread_cond_signal(&task_ready);

        /* We are done with the mutex; we don't need to hold it while we
         * generate the next task. */
        pthread_mutex_unlock(&task_mutex);
    }

    printf("\n");

    /* If we reach this point, one of the threads found a solution. We will
     * signal any waiting worker threads so they will wake up and see that we
     * are done. */
    pthread_cond_broadcast(&task_ready);
    pthread_mutex_unlock(&task_mutex);

    double end_time = get_time();
    uint64_t total_inversions = 0;
    // TODO we should join on all the threads here and add up the total number
    // of hashes computed.
    pthread_join(thread->thread_handle, NULL);
    if (strlen(thread->solution_hash) > 0) {
        printf("Solution found by thread %d:\n", thread->thread_id);
        printf("Nonce: %llu\n", thread->nonce);
        printf("Hash: %s\n", thread->solution_hash);
    }
    total_inversions += thread->num_inversions;

    double total_time = end_time - start_time;
    printf("%llu hashes in %.2fs (%.2f hashes/sec)\n",
            total_inversions, total_time, total_inversions / total_time);

    return 0;
}
