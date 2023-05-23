#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Abdirizaq Ali 000506148
// Lev Samin 000409096

#define NUM_PHILOSOPHERS 5 // Number of philosophers

sem_t chopsticks[NUM_PHILOSOPHERS]; // Semaphore for each chopstick
sem_t mutex; // Semaphore for mutual exclusion (for accessing chopsticks)

// Philosopher thread function
void *philosopher(void *arg)
{
    int id = *(int *)arg;
    int left_chopstick = id; // Index of the left chopstick
    int right_chopstick = (id + 1) % NUM_PHILOSOPHERS; // Index of the right chopstick

    while (1) {
        printf("Philosopher %d is thinking\n", id);// Think
        sleep(rand() % 5); // Sleep for random amount of time

        // Waiting
        sem_wait(&mutex); // Wait for mutex
        sem_wait(&chopsticks[left_chopstick]); // Wait for left chopstick
        sem_wait(&chopsticks[right_chopstick]); // Wait for right chopstick
        sem_post(&mutex); // Release mutex

        printf("Philosopher %d is eating\n", id);// Eat
        sleep(rand() % 5); // Sleep for random amount of time

        // Put down chopsticks
        sem_post(&chopsticks[right_chopstick]); // Release right chopstick
        sem_post(&chopsticks[left_chopstick]); // Release left chopstick
    }
    return NULL;
}

int main()
{
    srand(time(NULL)); // Initialize random seed

    sem_init(&mutex, 0, 1); // Initialize mutex semaphore to 1

    int ids[NUM_PHILOSOPHERS]; // Array to hold philosopher ids
    pthread_t threads[NUM_PHILOSOPHERS]; // Array to hold thread ids

    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) { // Initialize chopstick semaphores to 1
        sem_init(&chopsticks[i], 0, 1);
        ids[i] = i; // Assign each philosopher an id
        pthread_create(&threads[i], NULL, philosopher, &ids[i]); // Create philosopher threads
    }

    // Wait for philosopher threads to complete and destroy chopstick semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
        sem_destroy(&chopsticks[i]);
    }

    sem_destroy(&mutex); // Destroy mutex semaphore

    return 0;
}