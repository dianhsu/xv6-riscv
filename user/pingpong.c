#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    char buf[16];
    int p_p2c[2], p_c2p[2];
    if(pipe(p_p2c) < 0){
        exit(1);
    }
    if(pipe(p_c2p) < 0){
        exit(1);
    }
    if(fork() > 0){
        close(p_p2c[0]);
        close(p_c2p[1]);

        write(p_p2c[1], "p", 1);
        read(p_c2p[0], buf, 1); 
        fprintf(2, "%d: received pong\n", getpid());

        close(p_p2c[1]);
        close(p_c2p[0]);
        wait(0);
    }else{
        close(p_p2c[1]);
        close(p_c2p[0]);

        read(p_p2c[0], buf, 1);
        fprintf(2, "%d: received ping\n", getpid());
        write(p_c2p[1], "p", 1);

        close(p_p2c[0]);
        close(p_c2p[1]);
        exit(0);
    }

    exit(0);
}