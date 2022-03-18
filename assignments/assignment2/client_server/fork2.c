#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid;

    // create a child process through fork()
    // when the child process is created, it will get a
    // pid =0
    // the parent process will get the pid = child's process id
    pid = fork();

    if(pid == 0) {
        printf("hello from child\n");
    }
    else if (pid > 0) {
        printf("hello from parent\n");
    }
    else {
        perror("fork() failed\n");
    }
    
    return 0;
}
