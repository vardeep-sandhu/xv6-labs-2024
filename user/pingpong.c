#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(){
    int p1[2];
    int p2[2];
    pipe(p1); // from parent to child P[writes] -> Child[reads]
    pipe(p2); // from child to parent C[writes] -> P[reads]
    int pid = fork();

    if (pid == 0){
        // child process
        close(p1[1]);   // Pipe1: close write for child
        close(p2[0]);   // Pipe2: close read for child

        // for (int i=0;i < 10;i++){
        char buf;

        read(p1[0], &buf, 1);
        int pid = getpid();
        printf("%d", pid);
        write(1, ": received ping\n", 16);

        write(p2[1], "H", 1);
        // }
        close(p1[0]);   //Pipe 1: close read
        close(p2[1]);   //Pipe 2: close write
    }
    else{
        // parent process
        
        close(p1[0]);   // Pipe 1 close read for parent
        close(p2[1]);   // Pipe 2 close write for parent
        // for (int i=0;i < 10;i++){
        
        write(p1[1], "h", 1);

        char buf;
        read(p2[0], &buf, 1);

        int pid = getpid();
        printf("%d", pid);
        write(1, ": received pong\n", 16);
        // }
        close(p1[1]);
        wait(0);
    }
}