#include<linux/init.h>
#include<linux/module.h>
//#include<linux/cdev.h>
//nclude <linux/sched.h> //task_pid_nr
//#include <linux/uaccess.h>  //put_user
//#include <linux/sched/task.h>
//#include<linux/slab.h>
//#include<linux/sched/signal.h>
#define BUF_LEN 40



int openingdevfile(struct inode *pinode, struct file *pfile)
{
	//Opening Device node
	printk("%s\n",__FUNCTION__);
	return 0;
}

static ssize_t readingfromdevfile(struct file *pfile, char __user *pbuf, size_t len, loff_t *poff_t)
{
	// Reading device file
	struct task_struct *task;
      	char *array=NULL;
	unsigned int slen=0,cplen=0;

	//allocating memory in kernel space 
	array = kmalloc(BUF_LEN, GFP_KERNEL);
	if (!array)
        {
	printk("Memory is not allocated in kernel space!!");
	return -1;
	}
	memset(array,0x00,BUF_LEN);                          //memory allocated

	printk("Memory is allocated in kernel space!!\n");

	for_each_process(task) 
	{
		printk("%d %d      %s  \n",slen,(int)task_pid_nr(task),task->comm);
		memset(array,0x00,BUF_LEN);
		sprintf(array,"%d   %d    %s  \n",slen,(int)task_pid_nr(task),task->comm);
		cplen=strlen(array);          
		if ( copy_to_user(pbuf+slen, array, cplen) ) 
		{
			printk("Copying failed");
			return -1;
		}
		slen=slen+cplen;
	}

	kfree(array);
	return slen;       // slen;

}

static ssize_t writingtodevfile(struct file *pfile, const char __user *pbuf, size_t len, loff_t *poff_t)
{
	//Writing in device file
        struct kpidnice
        {
        unsigned int pid;
        int nice;
        }*kpid;
  
        struct task_struct *task;
        unsigned char *array=NULL;
       // kpid=(struct kpidnice *)pbuf;
        array = kmalloc(sizeof(struct kpidnice), GFP_KERNEL);
        if (!array)
        {
        printk("Memory is not allocated in kernel space!!");
        return -1;
        }

	printk("%s\n",__FUNCTION__);
        if ( copy_from_user(array,pbuf, len) ) 
	{
	printk("Copying failed");
	return -1;
        }
        kpid=(struct kpidnice *)array;
	for_each_process(task) 
	{
          if(task_pid_nr(task)==((unsigned int)kpid->pid))
          {
           task->prio=(int)kpid->nice; 
           task->normal_prio=(int)kpid->nice;
          }
        } 


	return len;
}

static int closingdevfile(struct inode *pinode, struct file *pfile)
{
	//closing device file
	printk("%s\n",__FUNCTION__);
	return 0;
}

dev_t dev=0;
static struct cdev demodev;
static struct class *dev_class;

static struct file_operations fops={
	.owner          = THIS_MODULE,
	.write          = writingtodevfile,
	.read           = readingfromdevfile,
	.open           = openingdevfile,
	.release        = closingdevfile,
};


static int __init  driverinit(void )
{
	printk("%s",__FUNCTION__);

	printk(KERN_INFO "Loading Module\n");

	//Creating  Devnode with Major and Minor Number
	/*Allocating Major number*/


	if((alloc_chrdev_region(&dev, 0, 1, "Demo")) <0){
		printk(KERN_INFO "Cannot allocate major number\n");
		return -1;
	}
	printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

	cdev_init(&demodev,&fops);

	if((cdev_add(&demodev,dev,1)) < 0){                          //Adding character device node in  /dev
		printk(KERN_INFO "Cannot add the device to the system\n");
		goto r_class;
	}
	if((dev_class = class_create(THIS_MODULE,"Demo")) == NULL){
		printk(KERN_INFO "Cannot create the struct class\n");
	}
	/*Creating device*/
	if((device_create(dev_class,NULL,dev,NULL,"Demo")) == NULL){
		printk(KERN_INFO "Cannot create the Device 1\n");
		goto r_device;
	}
	printk("device class :%s",dev_class->name);
	printk(KERN_INFO "Device Driver Insert...Done!!!\n");
	return 0;

r_device:
	class_destroy(dev_class);

r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

static void __exit driverexit(void )
{
	//Destrying Dev Node
	printk("%s",__FUNCTION__);
	device_destroy(dev_class,dev);
	class_destroy(dev_class);

	cdev_del(&demodev);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(driverinit);
module_exit(driverexit);

MODULE_LICENSE("GPL");


