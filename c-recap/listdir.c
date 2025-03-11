#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>

// there are 4 mistakes in this program, find them.

int main(int argc, char **argv) {

    char **dirs = (argv + 1);
    int ndirs = argc - 1;

    for(int i = 0; i < ndirs; i++) {
        char *path = dirs[i];
        DIR *dir = opendir(path);
        if(!dir) {
            fprintf(stderr, "Failed to open '%s'\n", path);
        }

        struct dirent *de;
        while ((de = readdir(dir))) {
            printf("%s/%s\n", path, de.d_name);
        }

        if(errno) {
            fprintf(stderr, "Error occurred reading directory: %s\n",
                    strerror(errno));
        }

        if(closedir(dir)) {
            printf("failed to close dir\n");
        }
    }
}
