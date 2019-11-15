#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include "tinycl.h"


#define __USE_GNU
#include <sched.h>

int main(int argc, char **argv)
{
    struct DRX_REGISTER drx;
    drx.dr0 = 0x10;
    drx.dr7 = 0xc2402;
    int dfd;
    dfd = open("/dev/tinyld", O_RDWR);
	if(dfd < 0)
	{
		printf("tinyld driver load fail \n");
		return -1;
	}
    ioctl(dfd,TINYLD_DR_WRITE,&drx);
    close(dfd);
    return 0;
}