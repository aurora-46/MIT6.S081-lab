#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fcntl.h"
int getargs(char *buf, char **args, int start)
{
    int n = 0, argc = 0;
    char s, *p;
    
    while (read(0, &s, sizeof(char))>0)
    {
        //printf("%c",s);
        if (s == '\n')
            break;
        else
        {
            buf[n] = s;
            n++;
        }
    }
    buf[n] = 0;

    char *len=buf + strlen(buf);
    for (p = buf; p <len ; p++)
    {
        if (start == MAXARG)
            break;
        if (*p == ' ')
            continue;
        else
        {
            argc++;
            args[start] = p;
            start++;
            while (*p != ' ' && p < len)
                p++;
            *p = 0;
        }
    }

    args[start]=0;

    return argc;
}

int main(int argc, char **argv)
{
    char buf[512];

    char *args[MAXARG+1], *cmd;
    if (argc > MAXARG + 1)
    {
        printf("xargs: too many operand \n");
        exit(0);
    }
    else if (argc == 1)
    {
        printf("xargs: missing operand \n");
        exit(0);
    }
    else
    {
        cmd = argv[1];
        for (int i = 0; i < argc - 1; i++)
            args[i] = argv[i + 1];

        while(getargs(buf, args, argc-1))
        {

            if (fork() == 0)
            {
                exec(cmd, args);
                printf("exec failed \n");
                exit(1);
            }else{
                wait(0);
            }
        }

        exit(0);
    }
}
