#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 16
#define MAX_NUMBERS 10000

// Structure to hold data for each thread
typedef struct {
    int* nums; // Array of numbers
    int start_idx; // Starting index of the number range assigned to the thread
    int end_idx; // Ending index of the number range assigned to the thread
    int prime_count; // Count of prime numbers found by the thread
} ThreadData;

// Function to check if a number is prime
int is_prime(int num) {
    if (num < 2) return 0;
    int i;
    for (i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// Global mutex for synchronization
pthread_mutex_t mutex;

// Thread function for counting prime numbers in a range
void* count_primes(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->prime_count = 0; // Initializing the thread's prime count to zero.
    int i;
    // Iterating over the assigned range of numbers
    for (i = data->start_idx; i < data->end_idx; i++) {
        if (is_prime(data->nums[i])) {
            // Locking the mutex before updating the shared resource
            pthread_mutex_lock(&mutex);
            data->prime_count++;
            // Unlocking the mutex after updating the shared resource
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL); // Exiting the thread
}

int main() {
    int i;
    int num_threads;
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[MAX_THREADS];
    int total_nums = 0;

    FILE* files[3];
    files[0] = fopen("PrimeData1.txt", "r");
    files[1] = fopen("PrimeData2.txt", "r");
    files[2] = fopen("PrimeData3.txt", "r");

    if (!files[0] || !files[1] || !files[2]) {
        fprintf(stderr, "Error opening files.\n");
        return 1;
    }

    // Reading numbers from files and counting total numbers
    for (i = 0; i < 3; i++) {
        int num;
        while (fscanf(files[i], "%d", &num) == 1) {
            total_nums++;
        }
        rewind(files[i]);
    }

    // Allocating memory for numbers
    int* nums = malloc(total_nums * sizeof(int));
    if (!nums) {
        fprintf(stderr, "Memory allocation error.\n");
        return 1;
    }

    // Reading numbers from files and storing in the array
    int index = 0;
    for (i = 0; i < 3; i++) {
        int num;
        while (fscanf(files[i], "%d", &num) == 1) {
            nums[index++] = num;
        }
    }

    // Closing files
    for (i = 0; i < 3; i++) {
        fclose(files[i]);
    }

    // Initializing the mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        fprintf(stderr, "Mutex initialization failed.\n");
        return 1;
    }

    // Dividing numbers among threads
    int nums_per_thread = total_nums / num_threads;

    // Creating threads and processing slices of numbers
    for (i = 0; i < num_threads; i++) {
        thread_data[i].nums = nums;
        thread_data[i].start_idx = i * nums_per_thread;
        thread_data[i].end_idx = (i == num_threads - 1) ? total_nums : (i + 1) * nums_per_thread;
        pthread_create(&threads[i], NULL, count_primes, (void*)&thread_data[i]);
    }

    // Waiting for threads to finish
    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculating total prime count
    int total_primes = 0;
    for (i = 0; i < num_threads; i++) {
        total_primes += thread_data[i].prime_count;
    }

    // Printing the total prime numbers found
    printf("Total prime numbers: %d\n", total_primes);

    // Writing prime numbers to an output file
    FILE* output_file = fopen("PrimeNumbersOutput.txt", "w");
    if (!output_file) {
        fprintf(stderr, "Error opening output file.\n");
        return 1;
    }

    fprintf(output_file, "Prime numbers:\n");
    for (i = 0; i < num_threads; i++) {
        fprintf(output_file, "Thread %d: %d\n", i, thread_data[i].prime_count);
    }

    fclose(output_file);

    // Freeing allocated memory
    free(nums);

    // Destroying the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
