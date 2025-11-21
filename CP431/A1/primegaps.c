#include <stdio.h>
#include <gmp.h>
#include "mpi.h"

typedef struct {
    double first;
    double second;
    double diff;
    double total_first;
    double total_last;
} PrimeResult;

int main(int argc, char* argv[]) {
    mpz_t rop;
    mpz_init(rop);

    PrimeResult max = {0, 0, 0, 0, 0};
    double prev = 0, curr, start, limit;
    unsigned int start_limit = 1000000000;

    int my_rank, process_count;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    // Wait for all processes to start and begin timer
    MPI_Barrier(MPI_COMM_WORLD);
    double elapsed_time = -MPI_Wtime();

    // Calculate work for each process
    int work = start_limit / process_count;
    int prev_process = my_rank - 1;
    start = (prev_process + 1) * work;
    limit = (my_rank + 1) * work;
 
    if (my_rank == process_count - 1) {
        // Add remainer to last process' limit
        limit += start_limit % process_count;
    }

    printf("rank: %d, start: %f, limit: %f\n", my_rank, start, limit);
    mpz_set_d(rop, start);
    prev = start;

    // Find primes and largest gap
    while (prev <= limit) {
        mpz_nextprime(rop, rop);
        curr = mpz_get_d(rop);

        if (prev == start) {
            max.total_first = curr;
        } else if (curr > limit) {
            max.total_last = prev;
            break;
        }

        if (curr - prev > max.diff) {
            max.diff = curr - prev;
            max.first = prev;
            max.second = curr;
        }

        prev = curr;
    }

    if (my_rank == 0) {
        PrimeResult edge_primes[process_count];
        edge_primes[0] = max;

        // Receive primes from each process
        MPI_Status status;
        for (int i = 1; i < process_count; i++) {
            printf("Receiving %d!\n", i);
            MPI_Recv(&edge_primes[i], 5, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_ERROR == MPI_SUCCESS) {
                // Compare received primes to max to find largest gap
                if (edge_primes[i].diff > max.diff) {
                    max.first = edge_primes[i].first;
                    max.second = edge_primes[i].second;
                    max.diff = edge_primes[i].diff;
                }
            }
        }

        // Check gap between first and last primes from different processes
        for (int i = 0; i < process_count - 1; i++) {
            if (edge_primes[i + 1].total_first - edge_primes[i].total_last > max.diff) {
                max.first = edge_primes[i].total_last;
                max.second = edge_primes[i + 1].total_first;
                max.diff = edge_primes[i + 1].total_first - edge_primes[i].total_last;
            }
        }

        // Stop timer
        elapsed_time += MPI_Wtime();
        printf("Time elapsed: %.2fs\n", elapsed_time);

        // Print results
        printf("Largest prime gap between 1 and %d is: %.0f and %.0f with a gap of %.0f\n",
                start_limit, max.first, max.second, max.diff);
    } else {
        printf("Sending from %d!\n", my_rank);
        // Send found primes
        MPI_Send(&max, 5, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
