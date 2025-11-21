#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define WIDTH 10000
#define HEIGHT 10000
#define MAX_ITERATIONS 100
#define C_REAL 0.28
#define C_IMAG 0.008

// Function to compute Julia set for a given complex number c
// init_z = x + yi, z = z^2 + c, c = c_real + c_imag
unsigned char computeJuliaSet(double x, double y, double c_real, double c_imag, unsigned char maxIterations) {
    double z_real, z_imag, temp;
    // If c_real = c_imag = 0, then render Mandelbrot Set instead (c = x + yi)
    if (c_real == 0 && c_imag == 0) {
        z_real = z_imag = 0;
        c_real = x;
        c_imag = y;
    } else {
        z_real = x;
        z_imag = y;
    }

    // Iterate until maxIterations or until the point escapes
    for (int iter = 0; iter < maxIterations; iter++) {
        temp = z_real * z_real - z_imag * z_imag + c_real;
        z_imag = 2 * z_real * z_imag + c_imag;
        z_real = temp;
        
        // If the magnitude of the point exceeds 2, it has escaped
        if (z_real * z_real + z_imag * z_imag > 4) {
            // float smooth_iter = iter + 1 - log(log(sqrt(z_real * z_real + z_imag * z_imag))) / log(2);
            return iter;
        }
    }

    return maxIterations; // Point did not escape within max iterations
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Setup timer
    double time = -MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);

    // Define the region of the complex plane
    double xmin = -2.0, xmax = 2.0;
    double ymin = -2.0, ymax = 2.0;

    // Compute the portion of the grid to be handled by each process
    int chunkSize = WIDTH / size;
    int startCol = rank * chunkSize;
    int endCol = startCol + chunkSize;

    // Allocate memory for data
    size_t fileSize = chunkSize * HEIGHT;
    unsigned char* data = (unsigned char*) malloc(fileSize * sizeof(unsigned char));
    if (data == NULL) {
        printf("Error allocating memory for process %d.\n", rank);
        MPI_Finalize();
        return 1;
    }

    // Iterate through the portion of the grid and compute Julia set
    for (int i = startCol; i < endCol; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            // Map screen coordinates to complex plane coordinates
            double x = xmin + i * (xmax - xmin) / WIDTH;
            double y = ymin + j * (ymax - ymin) / HEIGHT;
            unsigned char iterations = computeJuliaSet(x, y, C_REAL, C_IMAG, MAX_ITERATIONS); // Adjust maxIterations as needed

            // Save data
            data[(i - startCol) * HEIGHT + j] = iterations;
        }
    }

    // Write data to file
    char filename[100];
    sprintf(filename, "output/julia_set_%d_%d.txt", startCol, endCol);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file for process %d.\n", rank);
        MPI_Finalize();
        return 1;
    }

    fwrite(data, sizeof(unsigned char), fileSize, fp);

    // Close file
    if (fclose(fp) != 0) {
        printf("Error closing file for process %d.\n", rank);
    }
    free(data);

    if (rank == 0) {
        printf("Process %d finished, waiting to aggregate files.\n", rank);
        // Wait for all other processes to finish
        MPI_Barrier(MPI_COMM_WORLD);
        
        // Return time taken
        time += MPI_Wtime();
        printf("Time taken: %f seconds\n", time);

        // Aggregate all files into one
        time = -MPI_Wtime();

        printf("Aggregating files...\n");
        FILE *output = fopen("output/julia_set.txt", "w");

        for (int i = 0; i < size; i++) {
            char filename[100];
            sprintf(filename, "output/julia_set_%d_%d.txt", i * chunkSize, (i + 1) * chunkSize);
            FILE *fp = fopen(filename, "r");
            if (fp == NULL) {
                printf("Error opening file for process %d.\n", i);
                MPI_Finalize();
                return 1;
            }

            unsigned char* buf = (unsigned char*) malloc(fileSize * sizeof(unsigned char));
            if (buf == NULL) {
                printf("Error allocating memory for process %d.\n", i);
                MPI_Finalize();
                return 1;
            }

            // Read and write to output file
            fread(buf, sizeof(unsigned char), fileSize, fp);
            fwrite(buf, sizeof(unsigned char), fileSize, output);

            if (fclose(fp) != 0) {
                printf("Error closing file for process %d.\n", i);
            }

            free(buf);
        }

        if (fclose(output) != 0) {
            printf("Error closing output file.\n");
        }

        time += MPI_Wtime();
        printf("Time taken to aggregate files: %f seconds\n", time);
    } else {
        printf("Process %d finished.\n", rank);
        // Wait for process 0 to aggregate files
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
