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

void cpuid_helper(unsigned int eax, unsigned int ecx, unsigned int *pbuffer)
{
	unsigned int oeax, oebx, oecx, oedx;
	__asm__ volatile("cpuid": "=a"(oeax), "=b"(oebx), "=c"(oecx), "=d"(oedx) : "a"(eax), "c"(ecx));
	*pbuffer = oeax;
	*(pbuffer + 1) = oebx;
	*(pbuffer + 2) = oecx;
	*(pbuffer + 3) = oedx;
}
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
    //test for drx  when EIP = 0
    /*
	drx.dr0 = 0x0;
    drx.endr0 = 1;
	drx.dr7 = 0x402;
    drx.endr7 = 1;
    ioctl(dfd,TINYLD_DR_WRITE,&drx);
    */

    //test for rdmsr wrmsr
    /*
    msr.ecx = 0x199;
    ioctl(dfd,TINYLD_RDMSR,&msr);
    */
    
    msr.ecx = 0x160c;
    ioctl(dfd,TINYLD_RDMSR,&msr);
    // test for io
    io.io = 0x80;
    io.data = 0x66;
    ioctl(dfd,TINYLD_IOWB, &io);

    //test for iomem
    
    iomem.iomem = 0x100000010;
    ioctl(dfd,TINYLD_IOMEMRB, &iomem);
    
    
    ioctl(dfd,TINYLD_CHX002_FSBC_DUMP_IOMEM,NULL);
    
    msr.ecx = 0x160c;
    ioctl(dfd, TINYLD_RDMSR,&msr);
    close(dfd);
    return 0;
}
