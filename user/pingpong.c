#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv)
{
    int p1[2], p2[2];

    if (pipe(p1) < 0 || pipe(p2) < 0)
    {
        printf("pipe() failed\n");
        exit(1);
    }
    if (fork() == 0)
    {
        close(p1[1]);
        close(p2[0]);

        char get;
        read(p1[0], &get, 1);

        printf("%d: received ping\n", getpid());
        write(p2[1], &get, 1);
        close(p1[0]);
        close(p2[1]);
    }
    else
    {
        close(p1[0]);
        close(p2[1]);

        char old = 's', new;

        write(p1[1], &old, 1);
        read(p2[0], &new, 1);
        if (old != new)
            printf("pingpong failed!");
        printf("%d: received pong\n", getpid());

        close(p1[1]);
        close(p2[0]);
    }
    exit(0);
}
