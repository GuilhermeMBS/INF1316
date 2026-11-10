#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/param.h>

char pathName[MAXPATHLEN];

extern int alphasort();

int main() {
    int count;
    struct direct **files;

    if (getwd(pathName) == NULL) perror("Error while opening current dir");

    count = scandir(pathName, &files, NULL, alphasort);
    if (count <= 0) perror("No files in this directory");

    printf("Number of files = %d\n", count);

    for (int i = 0; i < count; i++) {
        struct stat atributes;

        if (stat(files[i]->d_name, &atributes) == 0) {
            time_t now = time(NULL);

            double time_passed = difftime(now, atributes.st_ctime);
            int days_passed = time_passed / (60 * 60 * 24);
            
            printf("%s \t inode %lu \t size: %ld \t age: %d \t num. links: %ld\n", 
                files[i]->d_name, 
                (unsigned long)atributes.st_ino, 
                atributes.st_size, 
                days_passed,
                atributes.st_nlink
            );
        }
    }

    return 0;
}