#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>

static int OpeningDevFile(struct inode *pinode, struct file *pfile){
    //Opening Device node
printk("%s",__FUNCTION__);
return 0;
}


static ssize_t ReadingFromDevFile(struct file *pfile, char __user *pbuf, size_t len, loff_t *poff_t){
// Reading device file
printk("%s",__FUNCTION__);
return len;
}
static ssize_t WritingToDevFile(struct file *pfile, const char __user *pbuf, size_t len, loff_t *poff_t){
//Writing in device file
printk("%s",__FUNCTION__);

return len;
}

static int ClosingDevFile(struct inode *pinode, struct file *pfile){
//closing device file
printk("%s",__FUNCTION__);
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
