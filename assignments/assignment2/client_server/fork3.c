#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid, pid_2;

    // create a child process through fork()
    pid = fork();
    pid_2 = fork();

    printf("pid=%d\n", pid);
    printf("pid_2=%d\n", pid_2);

    
    return 0;
}
