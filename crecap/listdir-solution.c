#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>

char *defdirs[1] = {"."};

int main(int argc, char **argv) {
    
    // handle no arguments (technically not necessary since the for-loop body is
    // not executed
    char **dirs = argc < 2 ? defdirs : (argv + 1);
    int ndirs = argc < 2 ? 1 : argc - 1;

    for(int i = 0; i < ndirs; i++) {
        char *path = dirs[i];
        DIR *dir = opendir(path);
        if(!dir) {
            fprintf(stderr, "Failed to open '%s'\n", path);
            // do not attempt to readdir() from NULL
            continue;
        }
        // reset errno
        errno = 0;
        struct dirent *de;
        while ((de = readdir(dir))) {
            // use -> instead of .
            printf("%s/%s\n", path, de->d_name);
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
