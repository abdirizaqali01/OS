#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Abdirizaq Ali 000506148
// Lev Samin 000409096

#define BUFFER_SIZE 5 // Maximum size of the buffer
#define NUM_ITEMS 10   // Number of items to produce/consume

sem_t empty;  // Semaphore to track the number of empty slots in the buffer
sem_t full;   // Semaphore to track the number of full slots in the buffer
sem_t mutex;  // Semaphore to ensure that only one thread can access the buffer at a time

int buffer[BUFFER_SIZE];  
int in = 0;               // Index of the next slot to write to
int out = 0;              // Index of the next slot to read from
int count = 0;            // Number of items currently in the buffer

// Producer thread function
void *producer(void *arg)
{
    for (int i = 0; i < NUM_ITEMS; i++) {
        sem_wait(&empty);  // Wait for an empty slot in the buffer
        sem_wait(&mutex);  // Acquire the mutex to access the buffer
        buffer[in] = i;    // Add the item to the buffer
        printf("Producer produced item %d\n", i);
        in = (in + 1) % BUFFER_SIZE;  // Update the index for the next slot to write to
        count++;           // Update the count of items in the buffer
        sem_post(&mutex);  // Release the mutex
        sem_post(&full);   // Signal that a slot in the buffer is now full
    }

    return NULL;
}

// Consumer thread function
void *consumer(void *arg)
{
    for (int i = 0; i < NUM_ITEMS; i++) {
        sem_wait(&full);   // Wait for a full slot in the buffer
        sem_wait(&mutex);  // Acquire the mutex to access the buffer
        int item = buffer[out];    // Remove an item from the buffer
        printf("Consumer consumed item %d\n", item);
        out = (out + 1) % BUFFER_SIZE;  // Update the index for the next slot to read from
        count--;            // Update the count of items in the buffer
        sem_post(&mutex);   // Release the mutex
        sem_post(&empty);   // Signal that a slot in the buffer is now empty
    }

    return NULL;
}

int main()
{
    // Initialize the semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    // Create the producer and consumer threads
    pthread_t producer_thread;
    pthread_t consumer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for the threads to complete
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy the semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}