#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <dirent.h>

char pathName[MAXPATHLEN];

extern int alphasort();

long long sumSubDir(const char *dirName) {
    DIR *dir;
    struct dirent *entry;
    struct stat atributes;
    
    long long totalSize = 0;
    char path[MAXPATHLEN];

    dir = opendir(dirName);
    if (dir == NULL) perror("Error while opening the directory");

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
        
        snprintf(path, sizeof(path), "%s/%s", dirName, entry->d_name);
        
        if (stat(path, &atributes) == -1) perror("Error while opening the file");
        
        if (entry->d_type == DT_DIR) totalSize += sumSubDir(path);
        else if (entry->d_type == DT_REG) totalSize += atributes.st_size;
    }

    closedir(dir);
    return totalSize;
}

int main() {
    printf("Soma dos tamanhos de todos os arquivos: %lld\n", sumSubDir("."));

    return 0;
}