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

#define __USE_GNU
#include <sched.h>
#include <pthread.h>

#include <ctype.h>
#include "tinyld.h"

int g_dfd;
int g_corenum;
int g_op;
uint32_t g_sedx = 0, g_seax = 0, g_redx = 0, g_reax = 0;

typedef struct _MSR
{
    struct ST_MSR_REGISTER msr;
    int core;
} _MSR;

_MSR g_msr;

struct ST_IO_REGISTER g_io;
struct ST_IOMEM_REGISTER g_mem;

uint64_t str2llhex(char *ptr)
{
    uint64_t data;
    uint32_t i;
    if (((ptr[0] == '0') && (ptr[1] == 'x')) || ((ptr[0] == '0') && (ptr[1] == 'X')))
    {
        ptr += 2;
    }
    for (i = 0, data = 0; (i < 16 && *ptr != 0); i++, ptr++)
    {
        data <<= 4;
        if (*ptr >= '0' && *ptr <= '9')
        {
            data |= (*ptr - '0');
        }
        else if (*ptr >= 'A' && *ptr <= 'F')
        {
            data |= (*ptr - 'A' + 10);
        }
        else if (*ptr >= 'a' && *ptr <= 'f')
        {
            data |= (*ptr - 'a' + 10);
        }
    }
    return data;
}

void getrmwmsrop(char *argstr)
{
    int index = 0;
    while ((argstr != NULL) && (*argstr != 0))
    {
        index = 0;
        switch (*argstr)
        {

        // set
        case 's':
        case 'S':
            if (isdigit(argstr[1]) && isdigit(argstr[2]))
            {
                index = (argstr[1] - '0') * 10 + (argstr[2] - '0');
                argstr += 3;
                if (index > 31)
                {
                    g_sedx |= (1 << (index - 32));
                }
                else
                {
                    g_seax |= (1 << (index - 0));
                }
            }
            else if (isdigit(argstr[1]) && (!isdigit(argstr[2])))
            {
                index = argstr[1] - '0';
                argstr += 2;
                if (index > 31)
                {
                    g_sedx |= (1 << (index - 32));
                }
                else
                {
                    g_seax |= (1 << (index - 0));
                }
            }
            else
            {
                argstr = NULL;
                g_seax = 0;
                g_sedx = 0;
            }
            break;

        // clear reset
        case 'c':
        case 'C':
        case 'r':
        case 'R':
            if (isdigit(argstr[1]) && isdigit(argstr[2]))
            {
                index = (argstr[1] - '0') * 10 + (argstr[2] - '0');
                argstr += 3;
                if (index > 31)
                {
                    g_redx |= (1 << (index - 32));
                }
                else
                {
                    g_reax |= (1 << (index - 0));
                }
            }
            else if (isdigit(argstr[1]) && (!isdigit(argstr[2])))
            {
                index = argstr[1] - '0';
                argstr += 2;
                if (index > 31)
                {
                    g_redx |= (1 << (index - 32));
                }
                else
                {
                    g_reax |= (1 << (index - 0));
                }
            }
            else
            {
                argstr = NULL;
                g_reax = 0;
                g_redx = 0;
            }
            break;

        default:
            argstr = NULL;
            g_reax = 0;
            g_redx = 0;
            g_seax = 0;
            g_sedx = 0;
            break;
        }
    }
    g_reax = ~g_reax;
    g_redx = ~g_redx;
}


int getopcmd(int argc, char **argv)
{
    int op = 0;
    if (!strcmp(argv[1], "-rdmsr"))
    {
        if (argc == 3)
        {
            g_msr.core = -1;
            g_msr.msr.ecx = str2llhex(argv[2]);
            op = 1;
        }
        else if (argc == 5)
        {
            g_msr.core = str2llhex(argv[4]);
            ;
            g_msr.msr.ecx = str2llhex(argv[2]);
            op = 1;
        }
        else
        {
            printf("Invalid op, exit\n");
            exit(0);
        }
    }
    else if (!strcmp(argv[1], "-wrmsr"))
    {
        if (argc == 5)
        {
            g_msr.core = -1;
            g_msr.msr.ecx = str2llhex(argv[2]);
            g_msr.msr.eax = str2llhex(argv[3]);
            g_msr.msr.edx = str2llhex(argv[4]);
            op = 2;
        }
        else if (argc == 7)
        {
            g_msr.core = str2llhex(argv[6]);
            g_msr.msr.ecx = str2llhex(argv[2]);
            g_msr.msr.eax = str2llhex(argv[3]);
            g_msr.msr.edx = str2llhex(argv[4]);
            op = 2;
        }
        else
        {
            printf("Invalid op, exit\n");
            exit(0);
        }
    }
    else if (!strcmp(argv[1], "-rmwmsr"))
    {
        if (argc == 4)
        {
            g_msr.core = -1;
            g_msr.msr.ecx = str2llhex(argv[2]);
            getrmwmsrop(argv[3]);
            op = 3;
        }
        else if (argc == 6)
        {
            g_msr.core = str2llhex(argv[5]);
            g_msr.msr.ecx = str2llhex(argv[2]);
            getrmwmsrop(argv[3]);
            op = 3;
        }
        else
        {
            printf("Invalid op, exit\n");
            exit(0);
        }
    }
    else if (!strcmp(argv[1], "-rdiob"))
    {
        if(argc == 3)
        {
            g_io.io = str2llhex(argv[2]);
            op = 4;
        }
        else
        {
            printf("Invalid op, exit\n");
            exit(0);
        }
    }
    else if (!strcmp(argv[1], "-wriob"))
    {
        if(argc == 4)
        {
            g_io.io = str2llhex(argv[2]);
            g_io.data = str2llhex(argv[3]);
            op = 5;
        }
        else
        {
            printf("Invalid op, exit\n");
            exit(0);
        }
    }
    else if (!strcmp(argv[1], "-rdmemb"))
    {
        if(argc == 3)
        {
            g_mem.iomem = str2llhex(argv[2]);
            op = 6;
        }
        else
        {
            printf("Invalid op, exit\n");
            exit(0);
        }
    }
    else if (!strcmp(argv[1], "-wrmemb"))
    {
        if(argc == 4)
        {
            g_mem.iomem = str2llhex(argv[2]);
            g_mem.bdata = str2llhex(argv[3]);
            op = 7;
        }
        else
        {
            printf("Invalid op, exit\n");
            exit(0);
        }
    }

    return op;
}

void cpuid_helper(unsigned int eax, unsigned int ecx, unsigned int *pbuffer)
{
    unsigned int oeax, oebx, oecx, oedx;
    __asm__ volatile("cpuid"
                     : "=a"(oeax), "=b"(oebx), "=c"(oecx), "=d"(oedx)
                     : "a"(eax), "c"(ecx));
    *pbuffer = oeax;
    *(pbuffer + 1) = oebx;
    *(pbuffer + 2) = oecx;
    *(pbuffer + 3) = oedx;
}

void *readmsr(void *arg)
{
    _MSR *pmsr = (_MSR *)arg;
    cpu_set_t mask, get;
    int core, flag;

    core = pmsr->core;
    pthread_detach(pthread_self());
    CPU_ZERO(&mask);
    CPU_SET(core, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
    {
        printf("set affinity failed \n");
        return 0;
    }
    CPU_ZERO(&get);
    if (sched_getaffinity(0, sizeof(get), &get) == -1)
    {
        printf("get affinity failed \n");
        return 0;
    }

    while (!flag)
    {
        for (int kk = 0; kk < core + 1; kk++)
        {
            if (CPU_ISSET(kk, &get))
            {
                flag = 1;
                break;
            }
        }
    }

    ioctl(g_dfd, TINYLD_RDMSR, &(pmsr->msr));
    printf("core %d 0x%x  0x%x  0x%x\n", pmsr->core, pmsr->msr.ecx, pmsr->msr.eax, pmsr->msr.edx);
}

void *writemsr(void *arg)
{
    _MSR *pmsr = (_MSR *)arg;
    cpu_set_t mask, get;
    int core, flag;

    core = pmsr->core;
    pthread_detach(pthread_self());
    CPU_ZERO(&mask);
    CPU_SET(core, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
    {
        printf("set affinity failed \n");
        return 0;
    }
    CPU_ZERO(&get);
    if (sched_getaffinity(0, sizeof(get), &get) == -1)
    {
        printf("get affinity failed \n");
        return 0;
    }

    while (!flag)
    {
        for (int kk = 0; kk < core + 1; kk++)
        {
            if (CPU_ISSET(kk, &get))
            {
                flag = 1;
                break;
            }
        }
    }

    ioctl(g_dfd, TINYLD_WRMSR, &(pmsr->msr));
    ioctl(g_dfd, TINYLD_RDMSR, &(pmsr->msr));
    printf("core %d 0x%x  0x%x  0x%x\n", pmsr->core, pmsr->msr.ecx, pmsr->msr.eax, pmsr->msr.edx);
}

void *rmwmsr(void *arg)
{
    _MSR *pmsr = (_MSR *)arg;
    cpu_set_t mask, get;
    int core, flag;
    uint32_t oldeax, oldedx;

    core = pmsr->core;
    pthread_detach(pthread_self());
    CPU_ZERO(&mask);
    CPU_SET(core, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
    {
        printf("set affinity failed \n");
        return 0;
    }
    CPU_ZERO(&get);
    if (sched_getaffinity(0, sizeof(get), &get) == -1)
    {
        printf("get affinity failed \n");
        return 0;
    }

    while (!flag)
    {
        for (int kk = 0; kk < core + 1; kk++)
        {
            if (CPU_ISSET(kk, &get))
            {
                flag = 1;
                break;
            }
        }
    }
    ioctl(g_dfd, TINYLD_RDMSR, &(pmsr->msr));
    oldeax = pmsr->msr.eax;
    oldedx = pmsr->msr.edx;

    pmsr->msr.eax = oldeax & g_reax;
    pmsr->msr.eax |= g_seax;

    pmsr->msr.edx = oldedx & g_redx;
    pmsr->msr.edx |= g_sedx;

    ioctl(g_dfd, TINYLD_WRMSR, &(pmsr->msr));
    ioctl(g_dfd, TINYLD_RDMSR, &(pmsr->msr));
    printf("core %d 0x%x  0x%x  0x%x -> 0x%x  0x%x\n", pmsr->core, pmsr->msr.ecx, oldeax, oldedx, pmsr->msr.eax, pmsr->msr.edx);
}

int main(int argc, char **argv)
{
    pthread_t ptid = 0;
    int ret = 0;

    if (argc == 1)
    {
        printf("sudo ./tinytool -rdmsr xxx\n");
        printf("sudo ./tinytool -rdmsr xxx -core xx\n");
        printf("sudo ./tinytool -wrmsr xxx  eax edx\n");
        printf("sudo ./tinytool -wrmsr xxx eax edx -core xx\n");
        printf("sudo ./tinytool -rmwmsr xxx s[0:63]r[0:63]\n");
        printf("sudo ./tinytool -rmwmsr xxx s[0:63]r[0:63] -core xx\n");
        printf("sudo ./tinytool -rdiob xxx\n");
        printf("sudo ./tinytool -wriob xxx xx\n");
        printf("sudo ./tinytool -rdmemb xxx\n");
        printf("sudo ./tinytool -wrmemb xxx xx\n");
        return 0;
    }
    g_op = getopcmd(argc, argv);

    g_corenum = sysconf(_SC_NPROCESSORS_CONF);

    g_dfd = open("/dev/tinyld", O_RDWR);
    if (g_dfd < 0)
    {
        printf("tinyld driver load fail \n");
        return -1;
    }

    if (g_op == 1)
    {
        if (g_msr.core == -1)
        {
            for (unsigned int kk = 0; kk < g_corenum; kk++)
            {
                g_msr.core = kk;
                pthread_create(&ptid, NULL, readmsr, &g_msr);
                if (ret)
                {
                    printf("pthread_create fail \n");
                    return -1;
                }
                pthread_join(ptid, NULL);
            }
        }
        else
        {
            pthread_create(&ptid, NULL, readmsr, &g_msr);
            if (ret)
            {
                printf("pthread_create fail \n");
                return -1;
            }
            pthread_join(ptid, NULL);
        }
    }
    else if (g_op == 2)
    {
        if (g_msr.core == -1)
        {
            for (unsigned int kk = 0; kk < g_corenum; kk++)
            {
                g_msr.core = kk;
                pthread_create(&ptid, NULL, writemsr, &g_msr);
                if (ret)
                {
                    printf("pthread_create fail \n");
                    return -1;
                }
                pthread_join(ptid, NULL);
            }
        }
        else
        {
            pthread_create(&ptid, NULL, writemsr, &g_msr);
            if (ret)
            {
                printf("pthread_create fail \n");
                return -1;
            }
            pthread_join(ptid, NULL);
        }
    }
    else if (g_op == 3)
    {
        if (g_msr.core == -1)
        {
            for (unsigned int kk = 0; kk < g_corenum; kk++)
            {
                g_msr.core = kk;
                pthread_create(&ptid, NULL, rmwmsr, &g_msr);
                if (ret)
                {
                    printf("pthread_create fail \n");
                    return -1;
                }
                pthread_join(ptid, NULL);
            }
        }
        else
        {
            pthread_create(&ptid, NULL, rmwmsr, &g_msr);
            if (ret)
            {
                printf("pthread_create fail \n");
                return -1;
            }
            pthread_join(ptid, NULL);
        }
    }
    else if(g_op == 4)
    {
        ioctl(g_dfd,TINYLD_IORB, &g_io);
        printf("io 0x%x 0x%x\n", g_io.io, g_io.data);
    }
    else if(g_op == 5)
    {
        ioctl(g_dfd,TINYLD_IOWB, &g_io);
        ioctl(g_dfd,TINYLD_IORB, &g_io);
        printf("io 0x%x 0x%x\n", g_io.io, g_io.data);
    }
    else if(g_op == 6)
    {
        ioctl(g_dfd,TINYLD_IOMEMRB, &g_mem);
        printf("mem 0x%llx 0x%x\n", g_mem.iomem, g_mem.bdata);
    }
    else if(g_op == 7)
    {
        ioctl(g_dfd,TINYLD_IOMEMWB, &g_mem);
        ioctl(g_dfd,TINYLD_IOMEMRB, &g_mem);
        printf("mem 0x%llx 0x%x\n", g_mem.iomem, g_mem.bdata);
    }

    //test for drx  when EIP = 0
    /*
	drx.dr0 = 0x0;
    drx.endr0 = 1;
	drx.dr7 = 0x402;
    drx.endr7 = 1;
    ioctl(dfd,TINYLD_DR_WRITE,&drx);
    */


    close(g_dfd);

    return 0;
}
