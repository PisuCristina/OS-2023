#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>


int is_file_size_greater(char* filePath, int size_to_be_compared) {
    int fd;
    off_t size;
    fd = open(filePath,O_RDONLY);
    if(fd == -1) {
        printf("could not open file\n");
        return -1;
    }
    size = lseek(fd, 0, SEEK_END);
    if(size > size_to_be_compared)
        return 1;
    return 0;
}

int starts_with(char s[], char* sub_s) {
    for (size_t i = 0; sub_s[i] != '\0'; ++i)
    {
        if (s[i] != sub_s[i])
        {
            return 0;
        }
    }
    return 1;
}


int listDir(int size, char* name, char* path) {
    char filePath[512];
    struct stat statbuf;

    DIR *dir = NULL;
    struct dirent *entry = NULL;
    dir = opendir(path);
    
    if(dir == NULL) {
        printf("ERROR\ninvalid directory path\n");
        return -1;
    }
    printf("SUCCESS\n");

   
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")){
            snprintf(filePath, 512, "%s/%s", path, entry->d_name);
            if(size){
                if(lstat(filePath, &statbuf) == 0) { 
                    if(S_ISREG(statbuf.st_mode)) {
                        if(is_file_size_greater(filePath,size))
                            printf("%s\n", filePath);
                    }
                }
            }else if(strcmp(name,"")){
                if(starts_with(entry->d_name,name)){
                    printf("%s\n", filePath);
                }
            }
            else
                printf("%s\n", filePath);
        }
    }

    closedir(dir);
    return 0;
}

void listRec(int size, char* name, char *path) { 
    DIR *dir = NULL;
    struct dirent *entry = NULL; 
    char fullPath[512]; 
    struct stat statbuf; 
    dir = opendir(path); 
    if(dir == NULL) {
        printf("ERROR\ninvalid directory path\n");
        return;
    }
    
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                if(size){   
                    if(S_ISREG(statbuf.st_mode)) {
                        if(is_file_size_greater(fullPath,size))
                            printf("%s\n", fullPath);
                    }
                } else if(strcmp(name,"")){
                    if(starts_with(entry->d_name,name)){
                        printf("%s\n", fullPath);
                    }
                }else
                    printf("%s\n", fullPath);
                if(S_ISDIR(statbuf.st_mode)) {
                    listRec(size,name,fullPath);
                }
            }
        }
    }
    closedir(dir);
 }

void list(int size, char* name, char* path) {
    listDir(size, name, path);
}

void list_rec(int size, char* name, char* path) {
    printf("SUCCESS\n");
    listRec(size, name, path);
}

int parse(char* filePath) {
    int fd = -1;
    char magic[2] = {'\0'};
    int header_size = 0;
    int version = 0;
    int nr_sect = 0;
    unsigned char name[19] = {'\0'};
    int type = 0;
    int size = 0;

    fd = open(filePath,O_RDONLY);
    if(fd == -1) {
        printf("invalid file path\n");
        return -1;
    }

    
   
    lseek(fd,-4,SEEK_END);
    read(fd,&header_size,2);
    read(fd,magic,2);
    if(strcmp(magic,"d1") != 0){
        printf("ERROR\nwrong magic");
        return -1;
    }

    lseek(fd,-header_size,SEEK_END);
    read(fd,&version,1);
    if(version < 62 || version > 88) {
        printf("ERROR\nwrong version");
        return -1;
    }
    read(fd,&nr_sect,1);
    if(nr_sect < 4 || nr_sect > 13) {
        printf("ERROR\nwrong sect_nr");
        return -1;
    }

    int crt = lseek(fd,0,SEEK_CUR);
    for (int i = 0; i < nr_sect; ++i)
    {
        read(fd,name,14);
        read(fd,&type,2);
        lseek(fd,4,SEEK_CUR);
        read(fd,&size,4);
        if(type != 39 && type !=17 && type != 72 && type != 42 && type != 52 && type != 43) {
            printf("ERROR\nwrong sect_types");
            return -1;
        }
    }

    printf("SUCCESS\nversion=%d\nnr_sections=%d\n",version,nr_sect);
    lseek(fd,crt,SEEK_SET);
    for (int i = 0; i < nr_sect; ++i)
    {
        read(fd,name,14);
        read(fd,&type,2);
        lseek(fd,4,SEEK_CUR);
        read(fd,&size,4);
        printf("section%d: %s %d %d\n",i+1,name,type,size);
    }    

    close(fd);
    return 0;
}





int main(int argc, char **argv){
    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("34379\n");
        }else if(strcmp(argv[1], "list") == 0){
            int opt = 0;
            int sz = 0;
            char* name = "";
            char* path;
        	for(int i=2;i<argc;i++){
                if(!strcmp(argv[i],"recursive"))
                    opt++;
                else {
                    char* token = strtok(argv[i],"=");
                    if(!strcmp(token,"name_starts_with")) {
                        name = strtok(NULL,"=");
                    }else if(!strcmp(token,"size_greater")) {
                        token = strtok(NULL,"=");
                        sscanf(token,"%d",&sz);
                    }else if(!strcmp(token,"path")) {
                        path = strtok(NULL,"=");
                    }
                }
            }
            
            if(opt){
                list_rec(sz,name,path);
            }else{
                list(sz,name,path);
            }
            }else if(strcmp(argv[1], "parse") == 0){
            char* file_path = strtok(argv[2],"=");
            file_path = strtok(NULL,"=");
            parse(file_path);
            }
    }
    return 0;
}