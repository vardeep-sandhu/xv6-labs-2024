#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int primes(int* list_nums, int counter){
    // stopping condition
    if (counter == 0){
        return 0;
    }
    int prime_num = list_nums[0];
    // prime found
    printf("prime: %d\n", prime_num);

    // filtering of array
    int p[2];
    pipe(p);
    int pid = fork();
    
    if (pid == 0){
        close(p[1]); //close write
        int num = 0;
        counter = 0;
        memset(list_nums, 0, sizeof(*list_nums));
        
        while(read(p[0], &num, 4) == 4){
            if (num % prime_num != 0){
                counter ++;
                list_nums[counter] = num;
            }
        }
        printf("%d\n", counter);
        close(p[0]);
        primes(list_nums, counter);
        exit(0);
    }
    else{

        // only writes
        close(p[0]); //close read
        for (int i = 0; i < counter; i++){
            write(p[1], &list_nums[i], 4);
        }
        close(p[1]);
        wait(0);
    }
return 0;
}

int main(){
    int test_list[280];
    int counter = 0;
    for(int i = 0; i <= 278; i++){
        test_list[i] = i + 2;
        counter ++;
    }
    // create a pipe
    primes(test_list, counter);
}