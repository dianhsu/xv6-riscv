#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
void checkAndShow(char* path, char* name){
    static char buf[DIRSIZ + 1];
    char *p;
    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ) return;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    for(int i = 0; ; ++i){
        if(p[i] == '\0' && name[i] == '\0'){
            printf("%s\n", path);
            break;
        }else if(p[i] == '\0' || name[i] == '\0'){
            break;
        }else if(p[i] != name[i]){
            break;
        }
    }
}
void find(char* program, char *path, char* name){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(path, 0)) < 0){
        fprintf(2, "%s: cannot open %s\n", program, path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "%s: cannot stat %s\n", program, path);
        close(fd);
        return;
    }
    switch(st.type){
        case T_FILE:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("%s: path too long\n", program);
                break;
            }
            checkAndShow(path, name);
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("%s: path too long\n", program);
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0){
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("%s: cannot stat %s\n", program, buf);
                    continue;
                }
                if(st.type == T_DIR && (p[0] == '.' && (p[1] == '\0' || (p[1] == '.' && p[2] == '\0')))){
                    continue;
                }else{
                    find(program, buf, name);
                }
            }
            break;
    }
    close(fd);
}

int main(int argc, char* argv[]){
    if(argc == 2){
        find(argv[0], ".", argv[1]);
        exit(0);
    }
    if(argc != 3){
        exit(0);
    }
    char *dir = argv[1];
    char *name = argv[2];
    find(argv[0], dir, name);
    exit(0);
}