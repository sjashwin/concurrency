#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *print(void*);

static int counter = 1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t even = PTHREAD_COND_INITIALIZER; 
pthread_cond_t odd = PTHREAD_COND_INITIALIZER;
int main(){
    int num = 10;
    pthread_t thread_id[2];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&even, NULL);
    pthread_cond_init(&odd, NULL);

    printf("Spawning new threads\n");
    for(int i=0; i<2; i++){
        pthread_create(&thread_id[i], NULL, print, (void *)&num);
        printf("Thread ID: %d\n", (int)thread_id[i]);
    }
    printf("---------------------------------\n");
    pthread_exit(NULL);
    return 0;
}

void *print(void *num){
    while(counter <= *(int *)(num)){
        pthread_mutex_lock(&lock);
        if (counter%2){
            printf("Thread: %d\t %d\n", (int)pthread_self(), counter++);
            pthread_cond_signal(&odd);
            pthread_cond_wait(&even, &lock);
        }else{
            printf("Thread: %d\t %d\n", (int)pthread_self(), counter++);
            pthread_cond_signal(&even);
            pthread_cond_wait(&odd, &lock);
        }
        pthread_mutex_unlock(&lock);
    }
    pthread_cond_signal(&odd);
    pthread_cond_signal(&even);
    return NULL;
}