#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include <linux/sched.h> //task_pid_nr
#include <linux/uaccess.h>  //put_user
#include <linux/sched/task.h>
#include<linux/slab.h>
#define BUF_LEN 20
static long int pid;

#define next_task(p) \
        list_entry_rcu((p)->tasks.next, struct task_struct, tasks)

#define for_each_process(p) \
        for (p = &init_task ; (p = next_task(p)) != &init_task ; )


int openingdevfile(struct inode *pinode, struct file *pfile){
    //Opening Device node
printk("%s",__FUNCTION__);

return 0;
}

static ssize_t readingfromdevfile(struct file *pfile, char __user *pbuf, size_t len, loff_t *poff_t){
// Reading device file
struct task_struct *task;
char array[BUF_LEN]={0};
//char *array=NULL;
unsigned int slen=0;

//allocating memory in kernel space 
/*
array = kmalloc(BUF_LEN, GFP_KERNEL);
if (!array){
printk("Memory is not allocated in kernel space!!");
return -1;
}
memset(array,0x00,BUF_LEN);                          //memory allocated
*/
printk("Memory is allocated in kernel space!!\n");

//   performing copy lot of intterupt request get attach for serving


for_each_process(task) {
      //   if(slen>len)
      //   return -1;

       printk("%d %d      %s  \n",slen,(int)task_pid_nr(task),task->comm);
       memset(array,0x00,BUF_LEN);
       sprintf(array,"%d      %s  \n",(int)task_pid_nr(task),task->comm);
//       slen=slen+10+sizeof(int)+strlen(task->comm);
           
     /*   if ( copy_to_user(pbuf+slen, array, BUF_LEN) ) {
       printk("Copying failed");
                return -EFAULT;
        }*/
//     slen=slen+BUF_LEN;
       
//intk("%s\n",array);
       }
       
                       
printk("%d %d\n",sizeof(pbuf),len);
//printk("%s\n",array);
//printk("%s\n",pbuf);
//kfree(array);
return -1;       // slen;

}

static ssize_t writingtodevfile(struct file *pfile, const char __user *pbuf, size_t len, loff_t *poff_t){
//Writing in device file

unsigned int count=0;
printk(" Len:%lu   function : %s",len,__FUNCTION__);
//while(count!=5)
//{
      printk("The a.out id : %ld  process id is %d\n",pid, (int) task_pid_nr(current));
       printk("The process vid is %d\n", (int) task_pid_vnr(current));
       printk("The process name is %s\n", current->comm);
       printk("The process group is %d\n", (int) task_tgid_nr(current));
       printk("The process prioity is %d\n", (int)current->prio);
       printk("%d\n\n",count++);
    printk("%d\n\n",count++);
//}
current->prio=70;
current->static_prio=100;
current->normal_prio=110;
current->rt_priority=105;

       printk("The process prioity is %d\n", (int)current->prio);
       printk("The process prio is %d\n", (int) current->static_prio);
       printk("The process general prio is %d\n",(int) current->normal_prio);
       printk("The process prio is %d\n", (int) current->rt_priority);

return len;
}

static int closingdevfile(struct inode *pinode, struct file *pfile){
//closing device file

return 0;
}

dev_t dev=0;
static struct cdev Demodev;
static struct class *dev_class;

static struct file_operations fops={
        .owner          = THIS_MODULE,
        .write          = writingtodevfile,
        .read           = readingfromdevfile,
        .open           = openingdevfile,
        .release        = closingdevfile,
};


static int __init  driverInit(void )
{
printk("%s",__FUNCTION__);

 printk(KERN_INFO "Loading Module\n");
       printk("The process id is %d\n", (int) task_pid_nr(current));
       printk("The process vid is %d\n", (int) task_pid_vnr(current));
       printk("The process name is %s\n", current->comm);
  //     printk("The process tty is %x\n", current->signal->tty);
       printk("The process group is %d\n", (int) task_tgid_nr(current));
       printk("\n\n");



//Creating  Devnode with Major and Minor Number
/*Allocating Major number*/


        if((alloc_chrdev_region(&dev, 0, 1, "DemoDev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        cdev_init(&Demodev,&fops);

       if((cdev_add(&Demodev,dev,1)) < 0){                          //Adding character device node in  /dev
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
      if((dev_class = class_create(THIS_MODULE,"DemoDev")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
          }
  /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"DemoDev")) == NULL){
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

static void __exit driverExit(void )
{
//Destrying Dev Node
printk("%s",__FUNCTION__);
device_destroy(dev_class,dev);
class_destroy(dev_class);

cdev_del(&Demodev);
unregister_chrdev_region(dev, 1);
printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}

module_init(driverInit);
module_exit(driverExit);

MODULE_LICENSE("GPL");
                                                                                                                    

