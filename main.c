#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
//list all the files in the directory
int listFile(char * path) {
  DIR* dir;
  struct dirent* entry;
  dir = opendir(path);
  printf("Files\n");
  if(dir!=NULL)
    {
        while((entry=readdir(dir))!=NULL){
            printf("%s \n", entry->d_name);
            printf("%d \n", entry->d_type);
          }
    }

  closedir(dir);
  printf("=====================================\n");
  return 0;
}

//specify which files are directories (if any)
int listDirec(char * path) {
  DIR* dir;
  struct dirent* entry;
  dir = opendir(path);

  int size, numdir;
  printf("Directories\n");
  if(dir!=NULL)
    {
        while((entry=readdir(dir))!=NULL){
            size++;
            if (entry->d_type == 4) {

              printf("%s \n", entry->d_name);
              numdir++;
            }
          }
    }
  closedir(dir);
  if (!size)
    printf("No directories found.");

  printf("%d\n", size);
  printf("=====================================\n");
  /**
  char * array[size];
  printf("%d\n", numdir);
  struct dirent* entry1;
  dir = opendir(path);
  if(dir!=NULL)
    {
        while((entry1=readdir(dir))!=NULL){
          size++;
          }
    }
  closedir(dir);
  **/
  return 0;
}

char * makePath(char* s1, char* s2){
  int size = strlen(s1)+strlen(s2)+1;
  char * string = calloc(sizeof(char), size);
  strcat(string, s1);
  strcat(string, "/");
  strcat(string, s2);
  return string;
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
              //printf("%s\n", string);
              int x = stat(string, buf);
              //printf("errno: %d\n", x);
              //printf("%d \n", buf->st_size);
              size += buf->st_size;
              //printf("Mode (permissions): %d\n", buf->st_mode);
              //printf("%s: %d\n",entry->d_name, size);
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
      //size += findSize(path);
      //printf("%d\n", size);
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
              size += listAll(string, loop + 1);

              //printf("=====================================\n");

            }
            if (entry->d_type == 8) {
              printf("|-- %s \n", entry->d_name);
            }

          }

    }
  closedir(dir);
  return size;
}
/**
int findSizeAll(char * path) {
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
              //printf("%s\n", string);
              int x = stat(string, buf);
              //printf("errno: %d\n", x);
              //printf("%d \n", buf->st_size);
              size += buf->st_size;
              //printf("%s: %d\n",entry->d_name, size);
              free(string);
            }
            if (entry->d_type == 4) {
              char * string = makePath(path, entry->d_name);
              size += findSizeAll(string);
              free(string);
            }
          }
    }
  free (buf);
  printf("%d\n", size);
  closedir(dir);
  return size;
}
**/

int main() {
  //listFile(".");
  //listDirec(".");
  findSize("..");
  listAll("..", 0);
  //findSizeAll("..");
  return 0;
}
