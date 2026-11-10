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

long long sumSubDir(const char *dirName, int level) {
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
        
        printf("%*s[%s]\n",
            level * 4,
            "",
            entry->d_name
        );
        
        if (entry->d_type == DT_DIR) totalSize += sumSubDir(path, level + 1);
        else if (entry->d_type == DT_REG) totalSize += atributes.st_size;
    }

    closedir(dir);
    return totalSize;
}

int main() {
    printf("[root]\n");
    long long total = sumSubDir(".", 1);

    printf("Soma dos tamanhos de todos os arquivos: %lld\n", total);

    return 0;
}