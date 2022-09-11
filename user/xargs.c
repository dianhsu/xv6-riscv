#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXLEN 1024
char *argvs[MAXARG + 1];
char buf[512];
char argBuf[MAXLEN];

int main(int argc,  char* argv[]){
    if(argc <= 1){
        exit(0);
    }
    int idx = 0;
    argvs[idx] = 0;
    while(idx + 1 < argc){
        argvs[idx] = argv[idx + 1];
        ++idx;
        argvs[idx] = 0;
    }
    int cnt = 0;
    int pos = 0;
    while((cnt = read(0, buf, sizeof(buf))) > 0){
        if(pos + cnt >= MAXLEN) exit(0);
        memmove(argBuf + pos, buf, cnt);
        pos += cnt;
    }
    for(int i = 0; i < pos; ++i){
        if(i == 0 || argBuf[i - 1] == '\0'){
            if(idx >= MAXARG) exit(0);
            argvs[idx++] = argBuf + i;
        }
        if(argBuf[i] == '\n') argBuf[i] = '\0';
    }
    int pid;
    if((pid = fork()) > 0){
        wait(0);
    }else{
        exec(argvs[0], argvs);
        exit(0);
    }
    exit(0);
}