#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    printf("%d ticks consumed\n",uptime());
    exit(0);
}