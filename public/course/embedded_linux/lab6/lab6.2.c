#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

void *work_hard(void *arg) {
    while (1) {
        printf("%s", (char *)arg);
        fflush(stdout);
    }
    return NULL;
}

int main() {
    pthread_t th0, th1;
    pthread_attr_t attr0, attr1;
    struct sched_param param0, param1;
    
    // Initialize thread attributes
    pthread_attr_init(&attr0);
    pthread_attr_init(&attr1);
    
    // Set scheduling policy to SCHED_FIFO for both threads
    pthread_attr_setschedpolicy(&attr0, SCHED_FIFO);
    pthread_attr_setschedpolicy(&attr1, SCHED_FIFO);
    
    // Set priority for th0 (priority 1)
    param0.sched_priority = 1;
    pthread_attr_setschedparam(&attr0, &param0);
    pthread_attr_setinheritsched(&attr0, PTHREAD_EXPLICIT_SCHED);
    
    // Set priority for th1 (priority 2)
    param1.sched_priority = 2;
    pthread_attr_setschedparam(&attr1, &param1);
    pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    
    // Create threads
    pthread_create(&th0, &attr0, work_hard, "0");
    pthread_create(&th1, &attr1, work_hard, "1");
    
    // Wait for threads (which never return)
    pthread_join(th0, NULL);
    pthread_join(th1, NULL);
    
    return 0;
}
