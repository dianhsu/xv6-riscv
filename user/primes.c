#include "kernel/types.h"
#include "user/user.h"
int isPrime(int v){
    for(int i = 2; i * i <= v; ++i){
        if(v % i == 0){
            return 0;
        }
    }
    return 1;
}
int main(){
    int p[2];
    int c[2];
    if(pipe(p) < 0){
        exit(1);
    }
    if(pipe(c) < 0){
        exit(1);
    }
    if(fork() > 0){
        close(p[0]);
        close(c[1]);
        char buf[8];
        for(int i = 2; i <= 35; ++i){
            buf[0] = i;
            write(p[1], buf, 1);
        }
        close(p[1]);
        while(read(c[0], buf, 1) != 0){
            fprintf(2, "prime %d\n", (int)buf[0]);
        }
        close(c[0]);
        wait(0);
    }else{
        close(p[1]);
        close(c[0]);
        char buf[8];
        while(read(p[0], buf, 1) != 0){
            int v = buf[0];
            if(isPrime(v)){
                if(fork() == 0){
                    write(c[1], buf, 1);    
                    close(c[1]);
                    exit(0);
                }
                wait(0);
            }
            if(v == 35) break;
        }
        close(p[0]);
        close(c[1]);
        exit(0);
    }
    exit(0);
}