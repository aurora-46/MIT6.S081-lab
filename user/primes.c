#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void primes(int p[2])
{
    close(p[1]);
    int n;
    read(p[0], &n, sizeof(int));
    printf("prime %d\n", n);

    int num;
    int start = 0;
    int p1[2];

    while (read(p[0], &num, sizeof(int)))
    {
        if (num % n)
        {
            if (!start)
            {
                start = !start;
                if (pipe(p1) < 0)
                {
                    printf("pipe() failed\n");
                    exit(1);
                }

                if(fork()){
                    primes(p1);
                    wait(0);
                    return;
                }
                else{
                    close(p1[0]);
                }
            }
            write(p1[1],&num,sizeof(int));
        }
    }
    close(p[0]);
    if(start)
    close(p1[1]);
}
int main(int argc, char **argv)
{
    int p1[2];

    if (pipe(p1) < 0)
    {
        printf("pipe() failed\n");
        exit(1);
    }

    if (fork())
    {
        close(p1[0]);
        for (int i = 2; i <= 35; i++)
            write(p1[1], &i, sizeof(int));
        close(p1[1]);
        wait(0);
    }
    else
    {
        primes(p1);
    }

   
    exit(0);
}
