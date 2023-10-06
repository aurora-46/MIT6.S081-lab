#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "find: cannot open %s \n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    //printf("%s %s\n",path,name);

    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        if (strcmp(path + strlen(path) - strlen(name), name) == 0)
            printf("%s \n", path);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long \n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0||strcmp(".",de.name)==0||strcmp("..",de.name)==0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            find(buf, name);
        }
        break;
    }
    close(fd);
}
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("find: missing operand\n");
        exit(0);
    }
    else if (argc == 2)
    {
        find(".", argv[1]);
        exit(0);
    }
    else
    {
        find(argv[1], argv[2]);
        exit(0);
    }
}