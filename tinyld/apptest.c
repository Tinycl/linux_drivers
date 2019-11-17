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
#include "tinyld.h"


#define __USE_GNU
#include <sched.h>

int main(int argc, char **argv)
{
	struct ST_DRX_REGISTER drx;
    struct ST_MSR_REGISTER msr;
    struct ST_IO_REGISTER io;
    struct ST_IOMEM_REGISTER iomem;

	int dfd;
    dfd = open("/dev/tinyld", O_RDWR);
	if(dfd < 0)
	{
		printf("tinyld driver load fail \n");
		return -1;
	}
    //test for drx
	drx.dr0 = 0x10;
    drx.endr0 = 1;
	drx.dr7 = 0xc2402;
    drx.endr7 = 1;
    ioctl(dfd,TINYLD_DR_WRITE,&drx);
    
    //test for rdmsr wrmsr
    msr.ecx = 0x199;
    ioctl(dfd,TINYLD_RDMSR,&msr);
    
    // test for io
    io.io = 0x80;
    io.data = 0x55;
    ioctl(dfd,TINYLD_IOWB, &io);

    //test for iomem
    iomem.iomem = 0x00000054;
    ioctl(dfd,TINYLD_IOMEMRB, &iomem);
    close(dfd);
    return 0;
}
