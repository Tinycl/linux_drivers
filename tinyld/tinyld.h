#ifndef __TINYLD_H__
#define __TINYLD_H__

#define TINYLD_MAGIC 'Z'
#define TINYLD_RDMSR _IO(TINYLD_MAGIC,2)
#define TINYLD_WRMSR _IO(TINYLD_MAGIC,3)
#define TINYLD_IOWB _IO(TINYLD_MAGIC,4)
#define TINYLD_IORB _IO(TINYLD_MAGIC,5)
#define TINYLD_IOMEMWB _IO(TINYLD_MAGIC,6) //8
#define TINYLD_IOMEMRB _IO(TINYLD_MAGIC,7)
#define TINYLD_IOMEMWW _IO(TINYLD_MAGIC,8) //16
#define TINYLD_IOMEMRW _IO(TINYLD_MAGIC,9)
#define TINYLD_IOMEMWD _IO(TINYLD_MAGIC,10) //32
#define TINYLD_IOMEMRD _IO(TINYLD_MAGIC,11)
#define TINYLD_DR_WRITE _IO(TINYLD_MAGIC,13)

#ifndef uint64_t
#define uint64_t unsigned long long 
#endif
#ifndef int64_t
#define int64_t long long
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif
#ifndef int32_t
#define int32_t int
#endif

#ifndef uint16_t
#define uint16_t unsigned short
#endif
#ifndef int16_t
#define int16_t short
#endif

#ifndef uint8_t
#define uint8_t unsigned char
#endif
#ifndef int8_t
#define int8_t char
#endif

struct ST_DRX_REGISTER
{
    uint8_t endr0;
    uint8_t endr1;
    uint8_t endr2;
    uint8_t endr3;
    uint8_t endr4;
    uint8_t endr5;
    uint8_t endr6;
    uint8_t endr7;
	uint64_t dr0;
	uint64_t dr1;
	uint64_t dr2;
  	uint64_t dr3;
	uint64_t dr4;
	uint64_t dr5;
   	uint64_t dr6;
	uint64_t dr7;  
};

struct ST_MSR_REGISTER
{
    uint32_t ecx;  //msr
    uint32_t edx;  //high
    uint32_t eax;  //low
};

struct ST_IO_REGISTER
{
    uint16_t io; //io address
    uint8_t  data;  //io data
};

struct ST_IOMEM_REGISTER
{
    uint64_t iomem; //iomem address
    uint8_t  bdata; // 8
    uint16_t wdata; // 16
    uint32_t ddata; // 32
};
#endif
