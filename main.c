#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

char * makePath(char* s1, char* s2){
  int size = strlen(s1)+strlen(s2)+2;
  char * string = calloc(sizeof(char), size);
  strcpy(string, s1);
  strcat(string, "/");
  strcat(string, s2);
  //free(string);
  return string;
}

char * type(int x){
  if(x == DT_DIR){
    return "DIR";
  }else if(x == DT_REG){
    return "REG";
  }else if(x == DT_FIFO){
    return "pipe/FIFO";
  }else if(x == DT_SOCK){
    return "SOCK";
  }else if(x == DT_CHR){
    return "CHR";
  }else if(x == DT_BLK){
    return "BLK";
  }else if(x == DT_LNK){
    return "LNK";
  }
  return "UNKNOWN";
 }

//list all the files in the directory
int listFile(char * path) {
  DIR* dir;
  struct dirent* entry;
  dir = opendir(path);
  printf("==============================================================================\n");
  printf("Regular files:\n");
  if(dir!=NULL)
    {
        while((entry=readdir(dir))!=NULL){
            printf("\t%s\n", entry->d_name);
            printf("\t\t\ttype: %s\n", type(entry->d_type));
            char * string = makePath(path, entry->d_name);

            struct stat *buf = malloc(sizeof(struct stat));
            stat(string, buf);
            printf("\t\t\tsize: %lld\n", buf->st_size);
            // printf("\t\t\tmode (permissions): %d\n", buf->st_mode);
            char x[12];
            strmode(buf->st_mode, x);
            printf("\t\t\tmode (permissions): %s\n", x);
            printf("\t\t\ttime of last access: %s\n", ctime(&(buf->st_atime)));
            free(buf);
          }
    }

  closedir(dir);
  return 0;
}

//specify which files are directories (if any)
int listDirec(char * path) {
  DIR* dir;
  struct dirent* entry;
  dir = opendir(path);

  int size, numdir;
  printf("==============================================================================\n");
  printf("Directories:\n");
  if(dir!=NULL)
    {
        while((entry=readdir(dir))!=NULL){
            size++;
            if (entry->d_type == 4) {
              printf("\t%s \n", entry->d_name);
              numdir++;
            }
          }
    }
  closedir(dir);
  if (!size)
    printf("No directories found.");

  printf("\t\t\tsize: %d\n", size);
  return 0;
}

//show the total size of all the regular files the directory
int findSize(char * path) {
  DIR* dir;
  struct dirent* entry;
  dir = opendir(path);

  struct stat *buf = malloc(sizeof(struct stat));

  int size = 0;
  //printf("size?\n");

  if(dir!=NULL)
    {
        while((entry=readdir(dir))!=NULL){
            if (entry->d_type != 4) {
              char * string = makePath(path, entry->d_name);
              int x = stat(string, buf);
              size += buf->st_size;
              //printf("Mode (permissions): %d\n", buf->st_mode);

              free(string);
            }
          }
    }
  free (buf);
  //printf("%d\n", size);
  closedir(dir);
  return size;
}

int listAll(char * path, int loop) {
  DIR* dir;
  struct dirent* entry;
  dir = opendir(path);
  int size = 0;

  //printf("Directories\n");
  if(dir!=NULL)
    {
      //printf("%d\n", findSize(path) );
      findSize(path);
        while((entry=readdir(dir))!=NULL){
            //size++;

            for (int i = loop; i > 0; i-- )
              printf("  ");

            if (strcmp (".", entry->d_name) == 0)
              printf("|-- .\n");

            else if (strcmp ("..", entry->d_name) == 0)
              printf("|-- ..\n");

            else if (strcmp (".git", entry->d_name) == 0)
              printf("|-- .git\n");



            else if (entry->d_type == 4) {
              char * string = makePath(path, entry->d_name);
              printf("|-- %s\n", entry->d_name);
              listAll(string, loop + 1);
              size += findSize(string);
              //printf("%d\n", size);
              //printf("=====================================\n");

            }
            if (entry->d_type == 8) {
              printf("|-- %s \n", entry->d_name);
            }

          }

    }
  closedir(dir);
  //printf("%d\n", size);
  return size;
}

int listAllW(char * path) {
  int a = listAll(path, 0);
  printf("==============================================================================\n");
  printf("Statistics for directory: %s \nTotal Directory Size: %d bytes \n", path, a);

  double b = a/1000.;
  double c = b/1000.;
  double d = c/1000.;

  char sz[100];
  sprintf(sz, "\tFile size: %d B = %e KB = %e MB = %e GB", a, b, c, d);
  printf("%s\n", sz);
  //listAll(path, 0);
  return 0;
}

int main(int argc, char *argv[]) {
  int x;
  for(x = 0; argc > 0; x++){
    findSize(argv[x]);
    listAllW(argv[x]);
    listDirec(argv[x]);
    listFile(argv[x]);
    argc--;
  }
  return 0;
}
