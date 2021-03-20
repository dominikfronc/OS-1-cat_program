#include <stdlib.h>
#include <stdio.h>

int main() {
    int buff_const = 1000;
    char buffer[buff_const];
    int size_read;
    
    int input_fd = 0;
    int output_fd = 1;
    
    do{
        size_read = read(input_fd,buffer,buff_const);
        if(size_read > 0){
            int size_write = write(output_fd,buffer,size_read);
            while(size_write!=size_read){
                size_write = write(output_fd,buffer,size_read);
            }
        }
    }while(size_read > 0);
    return 0;
}
