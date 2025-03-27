#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 20

void *print_message(void *thread_id) {
    int id = *((int *)thread_id);
    printf("Thread %d is running\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        int rc = pthread_create(&threads[i], NULL, print_message, (void *)&thread_ids[i]);
        if (rc) {
            printf("Error: unable to create thread, %d\n", rc);
            return -1;
        }
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("All threads completed\n");
    return 0;
}
