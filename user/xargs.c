#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void xargs(int argc, char** argv, char** args){
    int pid = fork();
    if (pid == 0){
        // printf("I am here: %s\n", argv[1]);
        exec(argv[1], args);
        fprintf(2, "Error in child");
    }
    else if (pid < 0){
        fprintf(2, "Error in child");
        exit(1);
    }
    else {
        wait(0);
    }
}

int main(int argc, char *argv[]){
    char *args[MAXARG];
    int counter = 0;
    // add elements from argv
    for (int i = 1; i < argc; i++){
        args[counter] = argv[i];
        counter += 1;
    }
    // add elements from stdin
    char c;
    int n = 0;
    int idx = 0;
    char word[512];
    while ((n = read(0, &c, 1)) == 1){
        if (c == ' ' || c == '\n' ){
            word[idx] = '\0';
            args[counter] = malloc(idx+1);
            strcpy(args[counter], word);
            // counter ++;
            idx = 0;
            xargs(argc, argv, args);
        }
        else{
            word[idx++] = c;
        }
    }
    // xargs(argc, argv, args);
}

// find . wc | xargs a b c
// echo abc bcd | xargs echo b c
// (echo 1 ; echo 2) | xargs echo