#include <stdio.h>
#include <stdlib.h>
// Abdirizaq Ali 000506148
// Lev Samin 000409096
#define MEM_SIZE 640
struct partition {
    int pid;
    int size;
    struct partition *next;
};
struct partition *free_list = NULL;
struct partition *allocated_list = NULL;
void allocate(int pid, int size);
void free_partition(int pid);
void display_free_list();
void display_allocated_list();
int main() {
    // Create initial free partition
    free_list = (struct partition*)malloc(sizeof(struct partition));
    free_list->pid = 0;
    free_list->size = MEM_SIZE;
    free_list->next = NULL;
    // Allocate memory for job1
    allocate(1, 130);
    display_free_list();
    display_allocated_list();
    // Allocate memory for job2
    allocate(2, 60);
    display_free_list();
    display_allocated_list();
    // Allocate memory for job3
    allocate(3, 100);
    display_free_list();
    display_allocated_list();
    // Free memory for job2
    free_partition(2);
    display_free_list();
    display_allocated_list();
    // Allocate memory for job4
    allocate(4, 200);
    display_free_list();
    display_allocated_list();
    // Free memory for job3
    free_partition(3);
    display_free_list();
    display_allocated_list();
    // Free memory for job1
    free_partition(1);
    display_free_list();
    display_allocated_list();
    // Allocate memory for job5
    allocate(5, 140);
    display_free_list();
    display_allocated_list();
    // Allocate memory for job6
    allocate(6, 60);
    display_free_list();
    display_allocated_list();
    // Allocate memory for job7
    allocate(7, 50);
    display_free_list();
    display_allocated_list();
    // Free memory for job6
    free_partition(6);
    display_free_list();
    display_allocated_list();
    return 0;
}
void allocate(int pid, int size) {
    struct partition *p, *prev;
    prev = NULL;
    for (p = free_list; p != NULL; prev = p, p = p->next) {
        if (p->size >= size) {
            // Allocate memory to job
            struct partition allocated = (struct partition)malloc(sizeof(struct partition));
            allocated->pid = pid;
            allocated->size = size;
            allocated->next = allocated_list;
            allocated_list = allocated;
            // Update free partition
            p->size -= size;
            if (p->size == 0) {
                if (prev == NULL) {
                    free_list = p->next;
                } else {
                    prev->next = p->next;
                }
                free(p);
            }
            break;
        }
    }
}
void free_partition(int pid) {
    struct partition *p, *prev;
    prev = NULL;
    for (p = allocated_list; p != NULL; prev = p, p = p->next) {
        if (p->pid == pid) {
            // Free memory and add to free list
            struct partition free_p = (struct partition)malloc(sizeof(struct partition));
            free_p->pid = 0;
            free_p->size = p->size;
            free_p->next = NULL;
            // Find the correct position to insert the new partition
            struct partition *f, *f_prev;
            f_prev = NULL;
            for (f = free_list; f != NULL; f_prev = f, f = f->next) {
                if (f > free_p) {
                    break;
                }
            }
            if (f_prev == NULL) {
                free_p->next = free_list;
                free_list = free_p;
            } else {
                free_p->next = f_prev->next;
                f_prev->next = free_p;
            }
            // Update allocated list
            if (prev == NULL) {
                allocated_list = p->next;
            } else {
                prev->next = p->next;
            }
            free(p);
            break;
        }
    }
}
// This function displays the free partition list
void display_free_list() {
    printf("Free partition list: ");
    struct partition *p;
    // Iterate through the free_list, starting at the head
    for (p = free_list; p != NULL; p = p->next) {
        // Print the PID and size of the partition
        printf("[PID: %d, Size: %d]", p->pid, p->size);
        // If there is another partition in the list, print a separator arrow
        if (p->next != NULL) {
            printf(" -> ");
            }
    }
    printf("\n");
}
// This function displays the allocated partition list
void display_allocated_list() {
    printf("Allocated partition list: ");
    struct partition *p;
    // Iterate through the allocated_list, starting at the head
    for (p = allocated_list; p != NULL; p = p->next) {
        // Print the PID and size of the partition
        printf("[PID: %d, Size: %d]", p->pid, p->size);
        // If there is another partition in the list, print a separator arrow
        if (p->next != NULL) {
            printf(" -> ");
        }
    }
    printf("\n");
}