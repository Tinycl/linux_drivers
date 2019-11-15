#ifndef __TINYLD_H__
#define __TINYLD_H__

#define TINYLD_MAGIC 'Z'
#define TINYLD_RDMSR _IO(TINYLD_MAGIC,0)
#define TINYLD_WRMSR _IO(TINYLD_MAGIC,1)
#define TINYLD_WRITE _IO(TINYLD_MAGIC,2)
#define TINYLD_READ _IO(TINYLD_MAGIC,3)
#define TINYLD_DR_WRITE _IO(TINYLD_MAGIC,13)

struct DRX_REGISTER
{
	unsigned long long dr0;
	unsigned long long dr1;
	unsigned long long dr2;
  	unsigned long long dr3;
	unsigned long long dr4;
	unsigned long long dr5;
    unsigned long long dr6;
	unsigned long long dr7;  
};

#endif