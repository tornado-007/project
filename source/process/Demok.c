#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include <linux/sched.h> //task_pid_nr
#include <linux/uaccess.h>  //put_user
//nclude<linux/sched.h>

static long int pid;



static int OpeningDevFile(struct inode *pinode, struct file *pfile){
    //Opening Device node
printk("%s",__FUNCTION__);

return 0;
}

//#define size 10
//char To[size];



static ssize_t ReadingFromDevFile(struct file *pfile, char __user *pbuf, size_t len, loff_t *poff_t){
// Reading device file
char Array[100]={0};

sprintf(Array,"%d",(pid=task_pid_nr(current)));

printk("pid %s function  %s",Array,__FUNCTION__);
printk("buffer %x lenght  %lu",(unsigned int)pbuf,len);



for_each_process(task) {

       /* compare your process name with each of the task struct process name*/

//    printk("The process id is %ld\n", (int) task_pid_nr(task));
//     printk("The process vid is %d\n", (int) task_pid_vnr(task));
//     printk("The process name is %s\n", task->comm);
       sprintf(Array,"%d    %s ",(int)task_pid_nr(task),task->comm);
              /* if matched that is your user process PID */
           
 if ( copy_to_user(pbuf, Array, len) ) {
       printk("Copying failed");
                return -EFAULT;
        }
return len;
}
static ssize_t WritingToDevFile(struct file *pfile, const char __user *pbuf, size_t len, loff_t *poff_t){
//Writing in device file
printk(" Len:%lu   function : %s",len,__FUNCTION__);
int count=0;
struct task_struct *temp=current;
//void *tmph=current->rcu_tasks_holdout_list;
while(count!=15)
{
      printk("The a.out id : %ld  process id is %d\n",pid, (int) task_pid_nr(temp));
       printk("The process vid is %d\n", (int) task_pid_vnr(temp));
       printk("The process name is %s\n", temp->comm);
       printk("The process group is %d\n", (int) task_tgid_nr(temp));
       printk("The process prioity is %d\n", (int)current->prio);
       printk("%d\n\n",count++);
//temp=temp->group_leader;
    //   printk("The process prioity is %ld\n", (void*)tmph);
    printk("%d\n\n",count++);
//tmph=(struct list_head*)tmph->next;  
}
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

static int ClosingDevFile(struct inode *pinode, struct file *pfile){
//closing device file

return 0;
}

dev_t dev=0;
static struct cdev Demodev;
static struct class *dev_class;

static struct file_operations fops={
        .owner          = THIS_MODULE,
        .write          = WritingToDevFile,
        .read           = ReadingFromDevFile,
        .open           = OpeningDevFile,
        .release        = ClosingDevFile,
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


        if((alloc_chrdev_region(&dev, 0, 1, "DemoDevfile")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        cdev_init(&Demodev,&fops);

       if((cdev_add(&Demodev,dev,1)) < 0){                          //Adding character device node in  /dev
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
      if((dev_class = class_create(THIS_MODULE,"DemoDevfile")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }

        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"DemoDevfile")) == NULL){
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
