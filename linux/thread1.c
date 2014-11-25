/* compile and build
 * cc -D_REENRANT -I/usr/include/nptl thread1.c -o thread1 -L/usr/lib/nptl -lpthread
 * or
 * cc -D_REENRANT thread1.c -o thread1 -lpthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *thread_function(void *arg);

char message[] = "Hello World";

int main(){
    int res;
    pthread_t a_thread;
    void *thread_result;

    /*create the thread, and set the callback*/
    res = pthread_create(&a_thread, NULL, thread_function, (void*)message);
    if(res != 0){
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for thread to finish...\n");

    res = pthread_join(a_thread, &thread_result);
    if(res != 0){
        perror("Thread join failed");
        exit(0);
    }

    printf("Thread joined, it returned %s\n", (char *)thread_result);
    printf("Message is now %s\n", message);

    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg){
    printf("thread_function is running. argument was %s\n", (char *)arg);
    sleep(3);
    strcpy(message, "Bye!");
    pthread_exit("Thank you for the CPU time");
}