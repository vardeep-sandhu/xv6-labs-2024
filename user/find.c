#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char * name){
    struct dirent de;
    int fd;
    struct stat st;
    if ((fd = open(path, O_RDONLY)) < 0){
      fprintf(2, "can not open %s\n", path);
      return;
    }

    if (fstat(fd, &st) < 0){
      fprintf(2, "can not stat %s\n", path);
      close(fd);
      return;
    }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    char * basename = path;
    for (int i = 0; path[i]; i++){
      if (path[i] == '/')
      basename = path + i + 1;
    }
    if (strcmp(name, basename) == 0){
      printf("%s\n", path);
    }
    break;

  case T_DIR:
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      if(de.inum == 0)
        continue;

    char buf[512];
    char * p;
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    // prepare name of dir
    char namesbuf[DIRSIZ+1];
    memmove(namesbuf, de.name, DIRSIZ);
    namesbuf[DIRSIZ] = 0;
    
    strcpy(p, namesbuf);

    find(buf, name);
    }
    break;
  }
close(fd);
}

int main(int argc, char *argv[]){
        
    if (argc < 3){
        fprintf(2, "error: wrong number of args");
        exit(1);
    }
    find(argv[1], argv[2]);
}