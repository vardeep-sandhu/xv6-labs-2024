#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int read_fd){
    // stopping condition
    int prime_num = 0;
    if (read(read_fd, &prime_num, 4) != 4){
        close(read_fd);
        return;
    }
    printf("prime %d\n", prime_num);

    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0){
        // reading side which recurces
        close(p[1]); 
        primes(p[0]);
        exit(0);
    }
    else{
        // writing side which filters

        // parent
        close(p[0]); // close reading end
        
        int num = 0;
        while(read(read_fd, &num, 4) == 4){
            if (num % prime_num != 0){
                write(p[1], &num, 4);
            }
        }
        close(p[1]);
        close(read_fd);
        wait(0);
    }
    return ;
}

int main(){
    int p[2];
    pipe(p);
    int pid = fork();
    
    if (pid == 0){
        close(p[1]);
        primes(p[0]);
        close(p[0]);
        exit(0);
    }
    else{
    // parent writes
    close(p[0]); // close read part

    for(int i = 2; i <= 280; i++){
            write(p[1], &i, 4);
    }
    close(p[1]);
    wait(0);
    }
exit(0);
}