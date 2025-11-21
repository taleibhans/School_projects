#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define NUM_SEQ_SEND 1000

typedef struct {
    unsigned int sequences[NUM_SEQ_SEND];
    unsigned int count;
} SequenceCount;

int is_pseudorandom(unsigned int sequence, unsigned int n) {
    // Find k <= log2(n)
    unsigned int k = floor(log2(n));
    unsigned int mask = (1 << k) - 1;
    double one_over_sqrt_n = 1 / sqrt(n);

    // Loop through every sequence X of length k
    for (int x = 0; x < pow(2, k); x++) {
        unsigned int temp_sequence = sequence;
        unsigned int count = 0, num_shifts = 0;

        while (num_shifts < n - k + 1) {
            // If X is in the sequence, increment count
            if ((temp_sequence & mask) == x) {
                count++;
                temp_sequence >>= k;
                num_shifts += k;
            } else {
                temp_sequence >>= 1;
                num_shifts++;
            }
        }

        // Check if count is within the range
        if (count - ((n - k + 1) / pow(2, k)) > one_over_sqrt_n) {
            return FALSE;
        }
    }

    return TRUE;
}

void print_sequence(unsigned int sequence, unsigned int n) {
    for (int i = 0; i < n; i++) {
        // Print -1 or 1 based on the value of the bit
        printf("%d", (sequence & (1 << (n - i - 1))) ? 1 : -1);
    }
    printf("\n");
}

void search_pseudorandom(unsigned int n) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define the range of sequences to check based on rank and size
    unsigned int start = (rank + 1) * (1 << (n - 1)) / size;
    unsigned int end = (rank + 2) * (1 << (n - 1)) / size;

    // printf("Rank %d: start = %u, end = %u\n", rank, start, end);

    // Send the sequences to the master process a few at a time
    SequenceCount seq_send;
    seq_send.count = 0;

    // Loop over the sequences in the defined range
    for (unsigned int i = start; i < end; i++) {
        if (is_pseudorandom(i, n)) {
            seq_send.sequences[seq_send.count] = i;
            seq_send.count++;
            if (seq_send.count == NUM_SEQ_SEND) {
                // Send the sequences to the master process
                MPI_Send(&seq_send, NUM_SEQ_SEND + 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);
                seq_send.count = 0;
            }
        }
    }

    // Send the remaining sequences to the master process
    if (seq_send.count > 0) {
        MPI_Send(&seq_send, NUM_SEQ_SEND + 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
    }

    unsigned int n = atoi(argv[1]);
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int all_finished = FALSE, finish_count = 0;

        while (!all_finished) {
            SequenceCount seq_recv;
            MPI_Status status;
            MPI_Recv(&seq_recv, NUM_SEQ_SEND + 1, MPI_UNSIGNED, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            // printf("Received %u sequences from rank %d\n", seq_recv.count, status.MPI_SOURCE);
            for (int i = 0; i < seq_recv.count; i++) {
                print_sequence(seq_recv.sequences[i], n);
            }

            if (seq_recv.count < NUM_SEQ_SEND) {
                // Process has no more sequences to send
                finish_count++;
                if (finish_count == size - 1) {
                    all_finished = TRUE;
                }
            }
        }
    } else {
        search_pseudorandom(n);
    }

    MPI_Finalize();
    return 0;
}
