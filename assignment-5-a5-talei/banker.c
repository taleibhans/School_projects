/*
 -------------------------------------
 Assignment 5
 File: banker.c
 -------------------------------------
 Author:  Talei Ibhanesebhor
 ID:      169020238
 Email:   ibha0238@mylaurier.ca
 Version  2023-12-3
 -------------------------------------
 */
//Removed -lrt library from make file for code to run properly

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CUSTOMERS 5
#define MAX_RESOURCES 4

int max_array[MAX_CUSTOMERS][MAX_RESOURCES] = {
    {6, 4, 7, 3},
    {4, 2, 3, 2},
    {2, 5, 3, 3},
    {6, 3, 3, 2},
    {5, 5, 7, 5}
};

int allocated_resources[MAX_CUSTOMERS][MAX_RESOURCES] = {{0}};
int remaining_needs[MAX_CUSTOMERS][MAX_RESOURCES] = {{0}};
int available_resources[MAX_RESOURCES] = {0};
int num_threads = MAX_CUSTOMERS;
int num_resources = MAX_RESOURCES;
pthread_mutex_t printmutex = PTHREAD_MUTEX_INITIALIZER;

void initialize(int argc, char *argv[]);
void printstatus();
bool compare(int *result, int *need);
int* subtract_arrays(int *result, int *allocation);
int* find_safe_sequence();
void* print_thread(void *thread_id);
void processthreads();

void initialize(int argc, char *argv[]) {
    if (argc != num_resources + 1) {
        printf("Insufficient arguments, exiting\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_resources; i++) {
        available_resources[i] = atoi(argv[i + 1]);
    }

    printf("Number of Customers: %d\n", num_threads);

    printf("Currently Available resources: ");
    for (int i = 0; i < num_resources; i++) {
        printf("%d ", available_resources[i]);
    }
    printf("\n");

    printf("Maximum Resources:\n");
    for (int i = 0; i < num_threads; i++) {
        for (int j = 0; j < num_resources; j++) {
            printf("%d ", max_array[i][j]);
            remaining_needs[i][j] = max_array[i][j];
        }
        printf("\n");
    }
}

void printstatus() {
    printf("Available resources: ");
    for (int i = 0; i < num_resources; i++) {
        printf("%d ", available_resources[i]);
    }
    printf("\n");

    printf("Maximum Resources:\n");
    for (int i = 0; i < num_threads; i++) {
        for (int j = 0; j < num_resources; j++) {
            printf("%d ", max_array[i][j]);
        }
        printf("\n");
    }

    printf("Allocated Resources:\n");
    for (int i = 0; i < num_threads; i++) {
        for (int j = 0; j < num_resources; j++) {
            printf("%d ", allocated_resources[i][j]);
        }
        printf("\n");
    }

    printf("Remaining Needs:\n");
    for (int i = 0; i < num_threads; i++) {
        for (int j = 0; j < num_resources; j++) {
            printf("%d ", remaining_needs[i][j]);
        }
        printf("\n");
    }
}

bool compare(int *need, int *result) {
    // compare array to check if  it is less than or equal to the intended
    for (int i = 0; i < num_resources; i++) {
        if (need[i] > result[i]) {
            return false;
        }
    }
    return true;
}

int* subtract_arrays(int *result, int *need) {
    int *ans = malloc(sizeof(int) * num_resources);
    for (int i = 0; i < num_resources; i++) {
        ans[i] = result[i] - need[i];
    }
    return ans;
}

int* find_safe_sequence() {
    int *safe_sequence = malloc(sizeof(int) * num_threads);
    int order[num_resources];
    bool end[num_threads];

    for (int i = 0; i < num_resources; i++) {
        order[i] = available_resources[i];
    }

    for (int i = 0; i < num_threads; i++) {
        end[i] = false;
    }

    int count = 0;
    while (count < num_threads) {
        bool found = false;
        for (int i = 0; i < num_threads; i++) {
            if (!end[i] && compare(remaining_needs[i], order)) {
                subtract_arrays(order, allocated_resources[i]);
                for (int j = 0; j < num_resources; j++) {
                    order[j] += allocated_resources[i][j];
                }
                end[i] = true;
                safe_sequence[count++] = i;

                found = true;
            }
        }

        if (!found) {
            break;
        }
    }

    if (count < num_threads) {
        free(safe_sequence);
        return NULL;
    }

    return safe_sequence;
}

void* print_thread(void *thread_id) {
    int tid = *((int *)thread_id);
    pthread_mutex_lock(&printmutex);

    printf("--> Customer/Thread %d\n", tid);
    printf("    Allocated resources: ");
    for (int j = 0; j < num_resources; j++) {
        printf("%d ", allocated_resources[tid][j]);
    }
    printf("\n");

    printf("    Needed: ");
    for (int j = 0; j < num_resources; j++) {
        printf("%d ", remaining_needs[tid][j]);
    }
    printf("\n");

    printf("    Available: ");
    for (int j = 0; j < num_resources; j++) {
        printf("%d ", available_resources[j]);
    }
    printf("\n");

    printf("    Thread has started\n");
    printf("    Thread has finished\n");
    printf("    Thread is releasing resources\n");

    for (int j = 0; j < num_resources; j++) {
        available_resources[j] += allocated_resources[tid][j];
        allocated_resources[tid][j] = 0;
    }

    printf("    New Available: ");
    for (int j = 0; j < num_resources; j++) {
        printf("%d ", available_resources[j]);
    }
    printf("\n");

    pthread_mutex_unlock(&printmutex);

    return NULL;
}

void processthreads() {
    int *safe_sequence = find_safe_sequence();
    if (safe_sequence != NULL) {
        printf("Safe sequence is: ");
        for (int i = 0; i < num_threads; i++) {
            printf("%d ", safe_sequence[i]);
        }
        printf("\n");

        pthread_t threads[num_threads];
        for (int i = 0; i < num_threads; i++) {
            pthread_create(&threads[i], NULL, print_thread, (void *)&safe_sequence[i]);
        }

        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        free(safe_sequence);
    }
}

int main(int argc, char *argv[]) {
    initialize(argc, argv);

    char input[100];
    int thread_id, resource_request1, resource_request2, resource_request3, resource_request4;


    while (1) {
        printf("Enter Command: ");
        fgets(input, sizeof(input), stdin);
        // Use strncmp to check the first two characters
        if (strncmp(input, "RQ", 2) == 0) {
            // Process 'RQ' command
            sscanf(input, "RQ %d %d %d %d %d", &thread_id, &resource_request1,
                   &resource_request2, &resource_request3, &resource_request4);
            // Check if the request can be satisfied
            bool satisfy = true;
            for (int j = 0; j < num_resources; j++) {
                if (resource_request1 > remaining_needs[thread_id][j] || 
                    resource_request2 > remaining_needs[thread_id][j] ||
                    resource_request3 > remaining_needs[thread_id][j] ||
                    resource_request4 > remaining_needs[thread_id][j] ||
                    resource_request1 > available_resources[j] || 
                    resource_request2 > available_resources[j] ||
                    resource_request3 > available_resources[j] ||
                    resource_request4 > available_resources[j]) {
                    satisfy = false;
                    break;
                }
            }
            // Update allocated resources, remaining needs, and available resources
            if (satisfy) {
                allocated_resources[thread_id][0] += resource_request1;
                allocated_resources[thread_id][1] += resource_request2;
                allocated_resources[thread_id][2] += resource_request3;
                allocated_resources[thread_id][3] += resource_request4;
                remaining_needs[thread_id][0] -= resource_request1;
                remaining_needs[thread_id][1] -= resource_request2;
                remaining_needs[thread_id][2] -= resource_request3;
                remaining_needs[thread_id][3] -= resource_request4;
                available_resources[0] -= resource_request1;
                available_resources[1] -= resource_request2;
                available_resources[2] -= resource_request3;
                available_resources[3] -= resource_request4;

                printf("State is safe, and request is satisfied\n");
            } else {
                printf("State is unsafe, and request is denied\n");
            }
        } else if (strncmp(input, "RL", 2) == 0) {
            // Process 'RL' command
            sscanf(input, "RL %d %d %d %d %d", &thread_id, &resource_request1,
                   &resource_request2, &resource_request3, &resource_request4);

            // Update allocated resources and available resources
            for (int j = 0; j < num_resources; j++) {
                allocated_resources[thread_id][j] -= resource_request1;
                available_resources[j] += resource_request1;
            }

            printf("The resources have been released successfully\n");

        } else if (strncmp(input, "Status", 6) == 0) {
            printstatus();
        } else if (strncmp(input, "Run", 3) == 0) {
            processthreads();
        } else if (strncmp(input, "Exit", 4) == 0) {
            exit(EXIT_SUCCESS);
        } else {
            printf("Invalid input, try again\n");
        }
    }

    return 0;
}

