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
//#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <asm-generic/ioctl.h>

#include <asm/msr.h>  //rdmsr

#include "tinycl.h"

MODULE_AUTHOR("Tiny");
MODULE_LICENSE("Dual BSD/GPL");

static int tinyld_major = 0;
module_param(tinyld_major, int, 0);

spinlock_t spinlock;


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
	long retval = 0;
	struct DRX_REGISTER drx_register;
	unsigned long long drx = 0;
	spin_lock(&spinlock);
	if(copy_from_user(&drx_register, (void __user *)arg, sizeof(struct DRX_REGISTER)))
	{	
		spin_unlock(&spinlock);
		printk(KERN_NOTICE "fun: copy_from_user error! \n");
		retval = -1;
		return retval;
	}
		
	switch(cmd)
	{
	case TINYLD_DR_WRITE:
	{
		__asm__ __volatile__ ("movq %%dr7, %%rax" :"=a"(drx) :);
		printk("dr7 is 0x%llx\n", drx);
		__asm__ __volatile__ ("movq %%rax, %%dr7" : : "a"(drx_register.dr7));
		__asm__ __volatile__ ("movq %%dr7, %%rax" :"=a"(drx) :);
		printk("dr7 is 0x%llx\n", drx);

		__asm__ __volatile__ ("movq %%dr0, %%rax" :"=a"(drx) :);
		printk("dr0 is 0x%llx\n", drx);
		__asm__ __volatile__ ("movq %%rax, %%dr0" : : "a"(drx_register.dr0));
		__asm__ __volatile__ ("movq %%dr0, %%rax" :"=a"(drx) :);
		printk("dr0 is 0x%llx\n", drx);

		break;
	}
	default:
	{
		spin_unlock(&spinlock);
		return -ENOTTY;	
	}
		
	}
	if(copy_to_user((void __user*)arg, &drx_register,sizeof(struct DRX_REGISTER)))
	{
		spin_unlock(&spinlock);
		printk("fun: copy_to_user is error! \n");
		retval = -1;
		return retval;
	}
	spin_unlock(&spinlock);
	return retval;
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
	spin_lock_init(&spinlock);


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
