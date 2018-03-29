#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    DIR           *dp;
    struct dirent *dirp;

    printf("Directory content:\n");
    if ((dp = opendir(".")) != NULL) {
        while ((dirp = readdir(dp)) != NULL) {
	  if (strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0)
	    continue;

            printf("%s\n", dirp->d_name);
        }

        closedir(dp);
    } else
        fprintf(stderr, "Can't open current directory\n");
}
