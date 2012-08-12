#include <stdio.h>
#include <unistd.h>

#define FILE_NAME    "test1.c"
#define BUFSIZE    100

char currentPath[BUFSIZE] = "";

int main(void)
{
    getcwd(currentPath,BUFSIZE);
    printf("getcwd currentPath = %s\n",currentPath);
    return 0;
}
