#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define RANDOM_MAX 71

void generate_arrays(int arrA[], int arrB[], int n) {
    arrA[0] = rand() % RANDOM_MAX;
    arrB[0] = rand() % RANDOM_MAX;
    for (int i = 1; i < n; i++) {
        arrA[i] = arrA[i - 1] + (rand() % RANDOM_MAX);
        arrB[i] = arrB[i - 1] + (rand() % RANDOM_MAX);
    }
}

int binary_search(int x, int arr[], int n) {
    int low = 0, high = n - 1;
    int index = n;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] < x) {
            low = mid + 1;
        } else {
            index = mid;
            high = mid - 1;
        }
    }
    return index;
}

void find_indices(int indices[], int size, int arrA[], int arrB[], int n) {
    int k = (int) log2(n);
    for (int i = 1; i < size; i++) {
        int j = binary_search(arrA[i * k], arrB, n);
        indices[i] = j;
    }
}

void merge_arrays(int arrA[], int arrB[], int sizeA, int sizeB, int arrC[]) {
    int i = 0, j = 0, k = 0;
    while (i < sizeA && j < sizeB) {
        if (arrA[i] < arrB[j]) {
            arrC[k++] = arrA[i++];
        } else {
            arrC[k++] = arrB[j++];
        }
    }
    while (i < sizeA) {
        arrC[k++] = arrA[i++];
    }
    while (j < sizeB) {
        arrC[k++] = arrB[j++];
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }

    MPI_Init(&argc, &argv);
    unsigned int n = atoi(argv[1]);
    int k = (int) log(n);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Wait for all processes to start and begin timer
    MPI_Barrier(MPI_COMM_WORLD);
    double elapsed_time = -MPI_Wtime();
    
    if (rank == 0) {
        // Generate the random sorted arrays
        int arrA[n], arrB[n];
        generate_arrays(arrA, arrB, n);
        // printf("Array A: \n");
        // for (int i = 0; i < n; i++) {
        //     printf("%d ", arrA[i]);
        // }
        // printf("\nArray B: \n");
        // for (int i = 0; i < n; i++) {
        //     printf("%d ", arrB[i]);
        // }
        // printf("\n");
        // printf("k: %d, Num slices: %d + 1\n", k, (int) floor((double) n / k));

        int prev_j = 0;
        int slices_sent = 0;
        while(prev_j < n) {
            // Find the indices of the arrays to be processed by each process
            int arr_sizes[size - 1];
            for (int i = 1; i < size; i++) {
                if (prev_j >= n) {
                    // Send a signal to the other processes to stop
                    int sizeB = 0;
                    MPI_Send(&sizeB, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    continue;
                }

                int indexA = ((slices_sent + 1) * k) < n ? (slices_sent + 1) * k : n - 1;
                int j = binary_search(arrA[indexA], arrB, n);

                // Send slices of the arrays to the other processes
                int sizeB = j - prev_j;
                // printf("Sending %d elements to process %d\n", sizeB, i);
                // printf("arrA[%d] = %d, arrB[%d] = %d, arrB[j = %d] = %d\n", (slices_sent + 1) * k, arrA[indexA], prev_j, arrB[prev_j], j, arrB[j]);
                MPI_Send(&sizeB, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&arrA[slices_sent * k], k, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&arrB[prev_j], sizeB, MPI_INT, i, 0, MPI_COMM_WORLD);

                prev_j = j;
                arr_sizes[i - 1] = k + sizeB;
                slices_sent++;
            }
            // printf("Slices sent: %d\n", slices_sent);

            // Wait for the results from the other processes
            int num_processors = (slices_sent % (size - 1)) == 0 ? size - 1 : (slices_sent % (size - 1));
            // printf("Receiving from %d processes\n", num_processors);
            for (int i = 1; i < num_processors + 1; i++) {
                int result_arr[arr_sizes[i - 1]];
                MPI_Recv(&result_arr, arr_sizes[i - 1], MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for (int j = 0; j < arr_sizes[i - 1]; j++) {
                    printf("%d ", result_arr[j]);
                }
                // printf("\n");
            }
            // printf("(slices_send + 1) * k: %d, j: %d, n: %d\n", (slices_sent + 1) * k, prev_j, n);
        }

        // Print left over elements in array A
        if (slices_sent * k < n) {
            for (int i = slices_sent * k; i < n; i++) {
                printf("%d ", arrA[i]);
            }
            // printf("\n");
        }

        // Send a signal to the other processes to stop
        for (int i = 1; i < size; i++) {
            int sizeB = 0;
            MPI_Send(&sizeB, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Stop timer
        elapsed_time += MPI_Wtime();
        printf("\nTime elapsed: %.2fs\n", elapsed_time);
    } else {
        while (1) {
            // Receive size of array B
            int sizeB;
            MPI_Recv(&sizeB, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (sizeB == 0) {
                break;
            }

            // Receive the slices of the arrays
            int arrA[k], arrB[sizeB];
            MPI_Recv(arrA, k, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(arrB, sizeB, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Merge the arrays
            int arrC[k + sizeB];
            merge_arrays(arrA, arrB, k, sizeB, arrC);

            // Send the result back to the master process
            MPI_Send(&arrC, k + sizeB, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
}
