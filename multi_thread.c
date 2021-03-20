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
bool empty = true;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

void *citacie_vlakno(){
    while(1){
        pthread_mutex_lock(&mutex);

        while(full)
            pthread_cond_wait(&not_full, &mutex);

        size_read = read(input_fd,buffer,buff_const);
        if(size_read > 0){
            empty = false;
            full = true;
        }
            
        
        else
            finished_reading = true;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }
}

void *zapisovacie_vlakno(){
    while(1){
        pthread_mutex_lock(&mutex);

        if(finished_reading)
            pthread_exit(NULL);

        while(empty)
            pthread_cond_wait(&not_empty, &mutex);
    
        size_write = write(output_fd, buffer, size_read);
        
        if(size_write > 0){
            full = false;
            empty = true;
        }
            

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    
    pthread_t t1, t2;
    pthread_create(&t1,NULL,citacie_vlakno,NULL);
    pthread_create(&t2,NULL,zapisovacie_vlakno,NULL);
    
    pthread_join(t2, NULL);

    return 0;
}
