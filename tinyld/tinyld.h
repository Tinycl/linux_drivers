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

#define TINYLD_CHX002_FSBC_DUMP_IOMEM _IO(TINYLD_MAGIC,30)

#define SET_BIT(x,n) (x | 1llu<<(n-1llu)) // set (n-1) to 1
#define CLEAR_BIT(x,n) (x & ~(1llu<<(n-1llu)))
#define SET_BITS(x,n,m) (x | ~(~0llu<<(m-n+1llu))<<(n-1llu))
#define GET_BITS(x,n,m) (x & ~(~0llu<<(m-n+1llu))<<(n-1llu))>>(n-1llu) // GET_BITS(0xfabf, 5,12) -> 0xab (bit4~bit11)
#define GET_BIT(x,n) ((x >> (n-1llu)) & 1llu)

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

struct ST_CHX002_FSBC_REGISTER
{
	struct ST_MSR_REGISTER data0_1604;
	struct ST_MSR_REGISTER data1_1605;
	struct ST_MSR_REGISTER data2_1606;
	struct ST_MSR_REGISTER data3_1607;
	struct ST_MSR_REGISTER datamask_1608;
	struct ST_MSR_REGISTER req_1609;
	struct ST_MSR_REGISTER cnt_160a;
	struct ST_MSR_REGISTER ini_160b;
	struct ST_MSR_REGISTER result_160c;
	
};
struct ST_CHX002_FSBC_CONFIG
{
	uint8_t isdone;
	uint8_t iswrap;
	uint8_t istriggerpattern;
	uint64_t base_iomem_addr; //base
	uint64_t wrap_iomem_addr; //base + len*ratio offset
	uint64_t trigger_iomem_addr; //base + random offset
	uint64_t buffersize; //M uint
	uint8_t position;
};
#endif
