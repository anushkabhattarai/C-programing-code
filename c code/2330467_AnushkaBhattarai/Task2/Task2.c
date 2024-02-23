#include <stdio.h> // Including standard input-output library
#include <pthread.h> // Including pthread library for threading
#include <math.h> // Including math library for mathematical functions
#include <unistd.h> // Including unistd library for POSIX API access

#define MAX_THREADS 64 // Defining a macro for maximum number of threads

struct ThreadData { // Defining a struct for thread data
    long int start; // Starting index for computation
    long int end; // Ending index for computation
    double sum; // Sum of terms
};

void *calculate_pi(void *arg) { // Function to calculate PI within a thread
    struct ThreadData *data = (struct ThreadData *)arg; // Casting argument to thread data struct pointer
    data->sum = 0.0; // Initializing sum to 0

    for (long int i = data->start; i < data->end; i++) { // Looping through the assigned range of terms
        double term = pow(-1, i) / (2 * i + 1); // Calculating the term of the series
        data->sum += term; // Adding the term to the sum
    }

    pthread_exit(NULL); // Exiting thread
}

int main() { // Main function
    long int n, num_threads; // Variables to store number of iterations and threads
    double pi = 0.0; // Variable to store the calculated value of PI
    
    printf("Enter number of iteration you want: "); // Prompting user to enter number of iterations
    scanf("%ld", &n); // Reading input for number of iterations
    
    printf("Enter number of threads: "); // Prompting user to enter number of threads
    scanf("%ld", &num_threads); // Reading input for number of threads

    if (num_threads > MAX_THREADS) { // Checking if the number of threads exceeds the maximum allowed
        printf("Error: Exceeded maximum number of threads.\n"); // Displaying error message
        return 1; // Exiting program with error code
    }

    pthread_t threads[MAX_THREADS]; // Array to hold thread IDs
    struct ThreadData thread_data[MAX_THREADS]; // Array to hold thread data

    long int terms_per_thread = n / num_threads; // Calculating number of terms per thread
    long int remainder = n % num_threads; // Calculating remainder terms
    long int start = 0; // Initializing starting index

    for (long int i = 0; i < num_threads; i++) { // Looping through threads
        thread_data[i].start = start; // Assigning start index to thread data
        thread_data[i].end = start + terms_per_thread + (i < remainder ? 1 : 0); // Assigning end index to thread data
        start = thread_data[i].end; // Updating start index for next thread

        pthread_create(&threads[i], NULL, calculate_pi, (void *)&thread_data[i]); // Creating thread
    }

    for (long int i = 0; i < num_threads; i++) { // Looping through threads
        pthread_join(threads[i], NULL); // Waiting for thread to finish
        pi += thread_data[i].sum; // Accumulating sum from each thread
    }

    pi *= 4; // Multiplying sum by 4 to get the value of PI

    printf("\nPI = %.6lf\n", pi); // Printing the calculated value of PI

    return 0; // Exiting program with success code
}
