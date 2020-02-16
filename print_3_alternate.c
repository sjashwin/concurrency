#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *print(void*);

static int counter = 1;
pthread_mutex_t lock;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;

int main(){
    pthread_t thread_id[2];
    int num = 30;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond1, NULL);
    pthread_cond_init(&cond2, NULL);
    pthread_cond_init(&cond3, NULL);

    for (int i=0; i<3; i++){
        pthread_create(&(thread_id[i]), NULL, print, (void *)&num);
        printf("Thread id %d\n", (int)(thread_id[i]));
    }
    printf("------------------------------------\n");
    pthread_exit(NULL);
    return 0;
}

void *print(void *num){
    while(counter <= *(int *)num){
        pthread_mutex_lock(&lock);
        if (counter%3 == 0){
            printf("Thread: %d\t %d\n", (int)pthread_self(), counter++);
            pthread_cond_signal(&cond2);
            pthread_cond_wait(&cond1, &lock);
        }else if (counter%3 == 1){
            printf("Thread: %d\t %d\n", (int)pthread_self(), counter++);
            pthread_cond_signal(&cond3);
            pthread_cond_wait(&cond2, &lock);
        }else{
            printf("Thread: %d\t %d\n", (int)pthread_self(), counter++);
            pthread_cond_signal(&cond1);
            pthread_cond_wait(&cond3, &lock);
        }
        pthread_mutex_unlock(&lock);
    }
    pthread_cond_signal(&cond1);
    pthread_cond_signal(&cond2);
    pthread_cond_signal(&cond3);
    return NULL;
}