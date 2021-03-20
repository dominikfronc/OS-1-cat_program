#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define buff_const 1000
char buffer[buff_const];
int size_read = 0;
int size_write = 0;
int input_fd = 0;
int output_fd = 1;

bool finished_reading = false;
bool full = false;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

void *read_thread(){
    while(1){
        pthread_mutex_lock(&mutex);

        while(full)
            pthread_cond_wait(&not_full, &mutex);

        size_read = read(input_fd,buffer,buff_const);
        if(size_read > 0)
            full = true;
        
        else
            finished_reading = true;

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&not_empty);
        
    }
}

void *write_thread(){
    while(1){
        pthread_mutex_lock(&mutex);

        if(finished_reading)
            pthread_exit(NULL);

        while(!full)
            pthread_cond_wait(&not_empty, &mutex);
    
        size_write = write(output_fd, buffer, size_read);
        
        if(size_write > 0)
            full = false;
        
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&not_full);

    }
}

int main() {
    
    pthread_t t1, t2;
    pthread_create(&t1,NULL,read_thread,NULL);
    sleep(1);
    pthread_create(&t2,NULL,write_thread,NULL);
    
    pthread_join(t2, NULL);

    return 0;
}
