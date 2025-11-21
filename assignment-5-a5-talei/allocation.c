/*
 -------------------------------------
 Assignment 5
 File: allocation.c
 -------------------------------------
 Author:  Talei Ibhanesebhor
 ID:      169020238
 Email:   ibha0238@mylaurier.ca
 Version  2023-12-3
 -------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORY 1048576 // 1MB

// Memoryblock structure to represent memory blocks or holes
typedef struct Memoryblock {
    int start;
    int end;
    int size;
    char process[20];
    struct Memoryblock *next;
} Memoryblock;


void initialize(Memoryblock **memory, int size);
void allocateMemory(Memoryblock **allocated, Memoryblock **holes, char process[], int size);
void releaseMemory(Memoryblock **allocated, Memoryblock **holes, char process[]);
void compactMemory(Memoryblock **allocated, Memoryblock **holes);
void printStatus(Memoryblock *allocated, Memoryblock *holes);
void clearMemory(Memoryblock *head);


// Function to initialize memory with a single hole
void initialize(Memoryblock **memory, int size) {
    *memory = (Memoryblock *)malloc(sizeof(Memoryblock));
    (*memory)->start = 0;
    (*memory)->end = size - 1;
    (*memory)->size = size;
    (*memory)->next = NULL;
    strcpy((*memory)->process, "");
}

// Function to allocate memory using the first fit algorithm
void allocateMemory(Memoryblock **allocated, Memoryblock **holes, char process[], int size) {
    Memoryblock *currentHole = *holes;
    Memoryblock *prevHole = NULL;

    while (currentHole != NULL) {
        #pragma unused(prevHole)
        if (currentHole->size >= size) {
            // Allocate memory from the current hole
            Memoryblock *newAllocated = (Memoryblock *)malloc(sizeof(Memoryblock));
            newAllocated->start = currentHole->start;
            newAllocated->end = currentHole->start + size - 1;
            newAllocated->size = size;
            strcpy(newAllocated->process, process);
            newAllocated->next = *allocated;
            *allocated = newAllocated;

            // Update the current hole
            currentHole->start += size;
            currentHole->size -= size;

            // Remove the hole if it's fully used
            if (currentHole->size <= 0) {
                if (prevHole == NULL) {
                    *holes = currentHole->next;
                } else {
                    prevHole->next = currentHole->next;
                }
                free(currentHole);
            }
            printf("Successfully allocated %d to process %s\n", size, process);
            return;
        }
        prevHole = currentHole;
        currentHole = currentHole->next;
    }
    printf("No hole of sufficient size\n");
}

// Function to release memory allocated to a process
void releaseMemory(Memoryblock **allocated, Memoryblock **holes, char process[]) {
    Memoryblock *currentAllocated = *allocated;
    Memoryblock *prevAllocated = NULL;
    while (currentAllocated != NULL) {
        if (strcmp(currentAllocated->process, process) == 0) {
            // Release memory to the holes
            Memoryblock *newHole = (Memoryblock *)malloc(sizeof(Memoryblock));
            newHole->start = currentAllocated->start;
            newHole->end = currentAllocated->end;
            newHole->size = currentAllocated->size;
            newHole->next = *holes;
            *holes = newHole;

            // Remove the allocated block
            if (prevAllocated == NULL) {
                *allocated = currentAllocated->next;
            } else {
                prevAllocated->next = currentAllocated->next;
            }
            free(currentAllocated);
            printf("Successfully released memory for process %s\n", process);
            return;
        }
        prevAllocated = currentAllocated;
        currentAllocated = currentAllocated->next;
    }
    printf("Process %s not found\n", process);
}

// Function to perform memory compaction
void compactMemory(Memoryblock **allocated, Memoryblock **holes) {
    Memoryblock *currentAllocated = *allocated;
    int value = 0;
    while (currentAllocated != NULL) {
        // Update the allocated block's addresses
        currentAllocated->start = value;
        currentAllocated->end = value + currentAllocated->size - 1;
        value = currentAllocated->end + 1;
        currentAllocated = currentAllocated->next;
    }
    // combine adjacent holes
    Memoryblock *currentHole = *holes;
    Memoryblock *prevHole = NULL;
    while (currentHole != NULL) {
        #pragma unused(prevHole)
        Memoryblock *nextHole = currentHole->next;
        if (nextHole != NULL && currentHole->end + 1 == nextHole->start) {
            // combine the holes
            currentHole->end = nextHole->end;
            currentHole->size += nextHole->size;
            // Remove the next hole
            currentHole->next = nextHole->next;
            free(nextHole);
        } else {
            prevHole = currentHole;
        }
        currentHole = currentHole->next;
    }
    printf("Compaction process is successful\n");
}

// Function to print the status of allocated and free memory blocks
void printStatus(Memoryblock *allocated, Memoryblock *holes) {
    printf("Partitions [Allocated memory = %d]:\n", MAX_MEMORY - (holes ? holes->size : 0));
    Memoryblock *currentAllocated = allocated;
    while (currentAllocated != NULL) {
        printf("Address [%d:%d] Process %s\n", currentAllocated->start, currentAllocated->end, currentAllocated->process);
        currentAllocated = currentAllocated->next;
    }
    printf("\nHoles [Free memory = %d]:\n", holes ? holes->size : 0);

    Memoryblock *currentHole = holes;
    while (currentHole != NULL) {
        printf("Address [%d:%d] len = %d\n", currentHole->start, currentHole->end, currentHole->size);
        currentHole = currentHole->next;
    }
}

//Function to free memory space
void clearMemory(Memoryblock *head) {
    while (head != NULL) {
        Memoryblock *temp = head;
        head = head->next;
        free(temp);
        temp = NULL;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <initial_memory_size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int initialMemorySize = atoi(argv[1]);
    if (initialMemorySize <= 0 || initialMemorySize > MAX_MEMORY) {
        printf("Invalid initial memory size\n");
        exit(EXIT_FAILURE);
    }
    Memoryblock *allocatedMemory = NULL;
    Memoryblock *freeMemory = NULL;
    initialize(&freeMemory, initialMemorySize);
    printf("Here, the First Fit approach has been implemented and allocated %d bytes of memory.\n", initialMemorySize);

    char command[3];
    while (1) {
        printf("allocator>");
        scanf("%s", command);
        if (strcmp(command, "RQ") == 0) {
            char process[10];
            int size;
            char algorithm[2];
            scanf("%s %d %s", process, &size, algorithm);
            if (strcmp(algorithm, "F") == 0) {
                allocateMemory(&allocatedMemory, &freeMemory, process, size);
            } else {
                printf("Only First Fit algorithm is available\n");
            }
        } else if (strcmp(command, "RL") == 0) {
            char process[10];
            scanf("%s", process);
            printf("releasing memory for process %s\n",process);
            releaseMemory(&allocatedMemory, &freeMemory, process);
        } else if (strcmp(command, "S") == 0) {
            printStatus(allocatedMemory, freeMemory);
        } else if (strcmp(command, "C") == 0) {
            compactMemory(&allocatedMemory, &freeMemory);
        } else if (strcmp(command, "Exit") == 0) {
            clearMemory(allocatedMemory);
            clearMemory(freeMemory);
            break;
        } else {
            printf("Invalid input, try again\n");
        }
    }
    return 0;
}
