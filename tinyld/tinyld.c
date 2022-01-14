#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/page.h>
#include <linux/version.h>
#include <asm-generic/ioctl.h>
#include <asm/msr.h>
 
#include <linux/kthread.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,15,0)
#include <linux/uaccess.h>
#include <asm/io.h>
#else
#include <asm/uaccess.h>
#endif


#include "tinyld.h"

MODULE_AUTHOR("Tiny");
MODULE_LICENSE("Dual BSD/GPL");

static int tinyld_major = 0;
module_param(tinyld_major, int, 0);

/*
struct task_struct *dump_thread;	
while(!kthread_should_stop())
{

}			
dump_thread = kthread_create(dump_kthreadfun,(void*)NULL,"dump");
wake_up_process(dump_thread);
*/

static int tinyld_opend(struct inode *inode, struct file *filp)
{
	return 0;
}

static int tinyld_release(struct inode *inode, struct file *filp)
{
	return 0;
}

void tinyld_vma_open(struct vm_area_struct *vma)
{
	printk(KERN_NOTICE "tinyld VMA open, vir %lx, phy %lx\n",vma->vm_start, vma->vm_pgoff << PAGE_SHIFT);
}

void tinyld_vma_close(struct vm_area_struct *vma)
{

}

static struct vm_operations_struct tinyld_remap_vm_ops = 
{
	.open = tinyld_vma_open,
	.close = tinyld_vma_close,
};

static int tinyld_remap_mmap(struct file *filp, struct vm_area_struct *vma)
{
	if(remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot))
		return -EAGAIN;
	vma->vm_ops = &tinyld_remap_vm_ops;
	tinyld_vma_open(vma);
	return 0;
}



long tinyld_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	uint8_t *va;
	uint64_t *va64;
    uint64_t kk;
	struct ST_DRX_REGISTER drx_register;
    struct ST_MSR_REGISTER msr_register;
    struct ST_IO_REGISTER io_register;
    struct ST_IOMEM_REGISTER iomem_register;
    void __iomem *iomemva;
	uint64_t drx = 0;

		
	switch(cmd)
	{
	    case TINYLD_DR_WRITE:
	    {
	        if(copy_from_user(&drx_register, (void __user *)arg, sizeof(struct ST_DRX_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            if(drx_register.endr0 == 1)
            {
		        __asm__ __volatile__ ("movq %%dr0, %%rax" :"=a"(drx) :);
		        printk("old dr0 is 0x%llx\n", drx);
		        __asm__ __volatile__ ("movq %%rax, %%dr0" : : "a"(drx_register.dr0));
		        __asm__ __volatile__ ("movq %%dr0, %%rax" :"=a"(drx) :);
		        printk("now dr0 is 0x%llx\n", drx);
            }
            if(drx_register.endr1 == 1)
            {
		        __asm__ __volatile__ ("movq %%dr1, %%rax" :"=a"(drx) :);
		        printk("old dr1 is 0x%llx\n", drx);
		        __asm__ __volatile__ ("movq %%rax, %%dr1" : : "a"(drx_register.dr1));
		        __asm__ __volatile__ ("movq %%dr1, %%rax" :"=a"(drx) :);
		        printk("now dr1 is 0x%llx\n", drx);
            }
            if(drx_register.endr2 == 1)
            {
		        __asm__ __volatile__ ("movq %%dr2, %%rax" :"=a"(drx) :);
		        printk("old dr2 is 0x%llx\n", drx);
		        __asm__ __volatile__ ("movq %%rax, %%dr2" : : "a"(drx_register.dr2));
		        __asm__ __volatile__ ("movq %%dr2, %%rax" :"=a"(drx) :);
		        printk("now dr2 is 0x%llx\n", drx);
            }
            if(drx_register.endr3 == 1)
            {
		        __asm__ __volatile__ ("movq %%dr3, %%rax" :"=a"(drx) :);
		        printk("old dr3 is 0x%llx\n", drx);
		        __asm__ __volatile__ ("movq %%rax, %%dr3" : : "a"(drx_register.dr3));
		        __asm__ __volatile__ ("movq %%dr3, %%rax" :"=a"(drx) :);
		        printk("now dr3 is 0x%llx\n", drx);
            }
            if(drx_register.endr4 == 1)
            {
		        __asm__ __volatile__ ("movq %%dr4, %%rax" :"=a"(drx) :);
		        printk("old dr4 is 0x%llx\n", drx);
		        __asm__ __volatile__ ("movq %%rax, %%dr4" : : "a"(drx_register.dr4));
		        __asm__ __volatile__ ("movq %%dr4, %%rax" :"=a"(drx) :);
		        printk("now dr4 is 0x%llx\n", drx);
            }
            if(drx_register.endr5 == 1)
            {
		        __asm__ __volatile__ ("movq %%dr5, %%rax" :"=a"(drx) :);
		        printk("old dr5 is 0x%llx\n", drx);
		        __asm__ __volatile__ ("movq %%rax, %%dr5" : : "a"(drx_register.dr5));
		        __asm__ __volatile__ ("movq %%dr5, %%rax" :"=a"(drx) :);
		        printk("now dr5 is 0x%llx\n", drx);
            }
            if(drx_register.endr6 == 1)
            {
		        __asm__ __volatile__ ("movq %%dr6, %%rax" :"=a"(drx) :);
		        printk("old dr6 is 0x%llx\n", drx);
		        __asm__ __volatile__ ("movq %%rax, %%dr6" : : "a"(drx_register.dr6));
		        __asm__ __volatile__ ("movq %%dr6, %%rax" :"=a"(drx) :);
		        printk("now dr6 is 0x%llx\n", drx);
            }
            if(drx_register.endr7 == 1)
            {
		        __asm__ __volatile__ ("movq %%dr7, %%rax" :"=a"(drx) :);
		        printk("old dr7 is 0x%llx\n", drx);
		        __asm__ __volatile__ ("movq %%rax, %%dr7" : : "a"(drx_register.dr7));
		        __asm__ __volatile__ ("movq %%dr7, %%rax" :"=a"(drx) :);
		        printk("now dr7 is 0x%llx\n", drx);
            }
	        if(copy_to_user((void __user*)arg, &drx_register,sizeof(struct ST_DRX_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
		    break;
    
	    }
		
        case TINYLD_RDMSR:
        {
	        if(copy_from_user(&msr_register, (void __user *)arg, sizeof(struct ST_MSR_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            __asm__ __volatile__("rdmsr":"=d"(msr_register.edx),"=a"(msr_register.eax):"c"(msr_register.ecx));
            printk("rdmsr: msr is 0x%x, high is 0x%x, low is 0x%x\n",msr_register.ecx, msr_register.edx, msr_register.eax);
	        if(copy_to_user((void __user*)arg, &msr_register,sizeof(struct ST_MSR_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }

        case TINYLD_WRMSR:
        {
	        if(copy_from_user(&msr_register, (void __user *)arg, sizeof(struct ST_MSR_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            __asm__ __volatile__("wrmsr": :"c"(msr_register.ecx),"d"(msr_register.edx),"a"(msr_register.eax));
            __asm__ __volatile__("rdmsr":"=d"(msr_register.edx),"=a"(msr_register.eax):"c"(msr_register.ecx));
            printk("wrmsr: msr is 0x%x, high is 0x%x, low is 0x%x\n",msr_register.ecx, msr_register.edx, msr_register.eax);
	        if(copy_to_user((void __user*)arg, &msr_register,sizeof(struct ST_MSR_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
  
        case TINYLD_IORB:
        {
	        if(copy_from_user(&io_register, (void __user *)arg, sizeof(struct ST_IO_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            __asm__ __volatile__("in %%dx, %%al":"=a"(io_register.data):"d"(io_register.io));
            printk("ior: io is 0x%x, data is  0x%x\n",io_register.io, io_register.data);
	        if(copy_to_user((void __user*)arg, &io_register,sizeof(struct ST_IO_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
        case TINYLD_IOWB:
        {
	        if(copy_from_user(&io_register, (void __user *)arg, sizeof(struct ST_IO_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            __asm__ __volatile__("out %%al, %%dx": :"a"(io_register.data),"d"(io_register.io));
            __asm__ __volatile__("in %%dx, %%al":"=a"(io_register.data):"d"(io_register.io));
            printk("iow: io is 0x%x, data is  0x%x\n",io_register.io, io_register.data);
	        if(copy_to_user((void __user*)arg, &io_register,sizeof(struct ST_IO_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
        case TINYLD_IOMEMRB:
        {
	        if(copy_from_user(&iomem_register, (void __user *)arg, sizeof(struct ST_IOMEM_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            iomemva = ioremap(iomem_register.iomem, sizeof(char));
            iomem_register.bdata = readb(iomemva);
            iounmap(iomemva);
            printk("iomemrb: iomem is 0x%llx, bdata is  0x%x\n",iomem_register.iomem, iomem_register.bdata);
	        if(copy_to_user((void __user*)arg, &iomem_register,sizeof(struct ST_IOMEM_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
        case TINYLD_IOMEMWB:
        {
	        if(copy_from_user(&iomem_register, (void __user *)arg, sizeof(struct ST_IOMEM_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            iomemva = ioremap(iomem_register.iomem, sizeof(char));
            writeb(iomem_register.bdata,iomemva);
            iomem_register.bdata = readb(iomemva);
            iounmap(iomemva);
            printk("iomemwb: iomem is 0x%llx, bdata is  0x%x\n",iomem_register.iomem, iomem_register.bdata);
	        if(copy_to_user((void __user*)arg, &iomem_register,sizeof(struct ST_IOMEM_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
        case TINYLD_IOMEMRW:
        {
	        if(copy_from_user(&iomem_register, (void __user *)arg, sizeof(struct ST_IOMEM_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            iomemva = ioremap(iomem_register.iomem, 2*sizeof(char));
            iomem_register.wdata = readw(iomemva);
            iounmap(iomemva);
            printk("iomemrw: iomem is 0x%llx, wdata is  0x%x\n",iomem_register.iomem, iomem_register.wdata);
	        if(copy_to_user((void __user*)arg, &iomem_register,sizeof(struct ST_IOMEM_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
        case TINYLD_IOMEMWW:
        {
	        if(copy_from_user(&iomem_register, (void __user *)arg, sizeof(struct ST_IOMEM_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            iomemva = ioremap(iomem_register.iomem, 2*sizeof(char));
            writew(iomem_register.wdata,iomemva);
            iomem_register.wdata = readw(iomemva);
            iounmap(iomemva);
            printk("iomemww: iomem is 0x%llx, wdata is  0x%x\n",iomem_register.iomem, iomem_register.wdata);
	        if(copy_to_user((void __user*)arg, &iomem_register,sizeof(struct ST_IOMEM_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
        case TINYLD_IOMEMRD:
        {
	        if(copy_from_user(&iomem_register, (void __user *)arg, sizeof(struct ST_IOMEM_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            iomemva = ioremap(iomem_register.iomem, 4*sizeof(char));
            iomem_register.ddata = readl(iomemva);
            iounmap(iomemva);
            printk("iomemrd: iomem is 0x%llx, ddata is  0x%x\n",iomem_register.iomem, iomem_register.ddata);
	        if(copy_to_user((void __user*)arg, &iomem_register,sizeof(struct ST_IOMEM_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
		case TINYLD_IOMEMWD:
        {
	        if(copy_from_user(&iomem_register, (void __user *)arg, sizeof(struct ST_IOMEM_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            iomemva = (uint64_t *)ioremap(iomem_register.iomem, 4*sizeof(char));
            writel(iomem_register.ddata,iomemva);
            iomem_register.ddata = readl(iomemva);
            iounmap(iomemva);
            printk("iomemww: iomem is 0x%llx, ddata is  0x%x\n",iomem_register.iomem, iomem_register.ddata);
	        if(copy_to_user((void __user*)arg, &iomem_register,sizeof(struct ST_IOMEM_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
        case TINYLD_IOMEMWQ:
        {
	        if(copy_from_user(&iomem_register, (void __user *)arg, sizeof(struct ST_IOMEM_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            va64 = ioremap(iomem_register.iomem, 8*sizeof(char));
            *va64 = iomem_register.qdata;
            iomem_register.qdata = *va64;
            iounmap(va64);
            printk("iomemwq: iomem is 0x%llx, qdata is  0x%llx\n",iomem_register.iomem, iomem_register.qdata);
	        if(copy_to_user((void __user*)arg, &iomem_register,sizeof(struct ST_IOMEM_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
		case TINYLD_IOMEMRQ:
        {
	        if(copy_from_user(&iomem_register, (void __user *)arg, sizeof(struct ST_IOMEM_REGISTER)))
	        {	
		        printk(KERN_NOTICE "fun: copy_from_user error! \n");
		        return 1;
	        }
            va64 = ioremap(iomem_register.iomem, 8*sizeof(char));
            iomem_register.qdata = *va64;
            iounmap(va64);
            printk("iomemwq: iomem is 0x%llx, qdata is  0x%llx\n",iomem_register.iomem, iomem_register.qdata);
	        if(copy_to_user((void __user*)arg, &iomem_register,sizeof(struct ST_IOMEM_REGISTER)))
	        {
		        printk("fun: copy_to_user is error! \n");
		        return 1;
	        }
            break;
        }
		case TINYLD_IOMEMR4K:
		{
			if (copy_from_user(&iomem_register, (void __user *)arg, sizeof(struct ST_IOMEM_REGISTER)))
			{
				printk(KERN_NOTICE "fun: copy_from_user error! \n");
				return -1;
			}
			va = (uint8_t *)ioremap(iomem_register.iomem, 0x1000);
			for (kk = 0; kk < 0x1000; kk++)
			{
				iomem_register.data4k[kk] = readb(&va[kk]);
			}
			iounmap(va);
			if (copy_to_user((void __user *)arg, &iomem_register, sizeof(struct ST_IOMEM_REGISTER)))
			{
				printk("fun: copy_to_user is error! \n");
				return -1;
			}

			break;
		}
		case TINYLD_IOMEMW4k:
		{
			if (copy_from_user(&iomem_register, (void __user *)arg, sizeof(struct ST_IOMEM_REGISTER)))
			{
				printk(KERN_NOTICE "fun: copy_from_user error! \n");
				return -1;
			}
			//write
			va = (uint8_t *)ioremap(iomem_register.iomem, 0x1000);
			for (kk = 0; kk < 0x1000; kk++)
			{
				writeb(iomem_register.data4k[kk], &va[kk]);
			}
			//read
			for (kk = 0; kk < 0x1000; kk++)
			{
				iomem_register.data4k[kk] = readb(&va[kk]);
			}
			iounmap(va);
			if (copy_to_user((void __user *)arg, &iomem_register, sizeof(struct ST_IOMEM_REGISTER)))
			{
				printk("fun: copy_to_user is error! \n");
				return -1;
			}
			break;
		}	

        default:
            break;
	}
	return 0;
}

static struct file_operations tinyld_remap_ops = 
{
	.owner = THIS_MODULE,
	.open = tinyld_opend,
	.release = tinyld_release,
	.unlocked_ioctl = tinyld_ioctl,
	.compat_ioctl = tinyld_ioctl,
	.mmap = tinyld_remap_mmap,
};

static void tinyld_setup_cdev(struct cdev *dev, int minor, struct file_operations *fops)
{
	int err, devno = MKDEV(tinyld_major, minor);
	cdev_init(dev, fops);
	dev->owner = THIS_MODULE;
	dev->ops = fops;
	err = cdev_add(dev, devno, 1);
	if(err)
	{
		printk(KERN_NOTICE "add tinyld dev error: err is %d, devno is %d\n",err,devno);
	}
		
}

static struct cdev tinyldDevs[2];
struct class *my_class;
dev_t dev;
static int tinyld_init(void)
{
	int result;
	dev = MKDEV(tinyld_major,0);
	if(tinyld_major)
	{
		result = register_chrdev_region(dev, 2, "tinyld");
	}
	else
	{
		result = alloc_chrdev_region(&dev,0,2,"tinyld");
		tinyld_major = MAJOR(dev);
	}
	if(result < 0)
	{
		printk(KERN_WARNING "tinyld: unable to get major %d\n", tinyld_major);
		return result;
	}
	if(tinyld_major == 0)
	{
		tinyld_major = result;
	}
	my_class = class_create(THIS_MODULE,"tinyld_class");
	tinyld_setup_cdev(tinyldDevs, 0, &tinyld_remap_ops);
	device_create(my_class, NULL, dev,NULL, "tinyld");
	printk(KERN_NOTICE "tinyld init \n");


	return 0;
}

static void tinyld_exit(void)
{
	cdev_del(tinyldDevs);
	device_destroy(my_class, dev);
	class_destroy(my_class);
	unregister_chrdev_region(MKDEV(tinyld_major,0),2);
	printk(KERN_NOTICE "tinyld exit \n");
}

module_init(tinyld_init);
module_exit(tinyld_exit);
