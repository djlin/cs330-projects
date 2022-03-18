#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid;

    // create a child process through fork()
    pid = fork();

    printf("pid=%d\n", pid);

    
    return 0;
}
