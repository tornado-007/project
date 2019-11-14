   #include<linux/module.h>
   #include<linux/input.h>
   #include<linux/i2c.h>
   #include<linux/init.h>
   #include<linux/slab.h>
   #include<linux/interrupt.h>
   #include<linux/iio/iio.h>
   #include<linux/fs.h>
   #include<linux/cdev.h>
  #include<linux/device.h>
  #include<linux/sysfs.h>
  #include<linux/kobject.h>
  #include<linux/kthread.h>
  #include<linux/delay.h>
  #include<linux/sched.h>
  
  #define lis2dw12_WHO_M_I_ADDR           0x0F
  #define lis2dw12_WHO_M_I_VAL            0x44
  #define freefall_reg_ADDR               0X36
  #define stat_reg                        0x27
  #define stat_reg_value                  0x43
  
  volatile int etx_value=0;
  struct kobject *kobj_ref;
  static struct task_struct *etx_thread;
  int thread_function(void*);
  struct i2c_client *p;
  static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
  static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf,size_t count);
  struct kobj_attribute etx_attr= __ATTR(etx_value,0660,sysfs_show,sysfs_store);
  
  
  static  int lis2dw12_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
  
  {
          int ret,var1,var2,var3,var4;
          p=client;
          ret = i2c_smbus_read_byte_data(client, lis2dw12_WHO_M_I_ADDR);
          if (ret < 0){
                  dev_err(&client->dev, "Error reading device ID: %d\n", ret);
          }
          printk("This is I2c inserted Probing Function..\n");
          printk("%X\n",ret);
  
          if (ret != lis2dw12_WHO_M_I_VAL) {
                  dev_err(&client->dev, "Invalid device ID: %d\n", ret);
                  return -ENODEV;
          }
          else
          {       var1=i2c_smbus_write_byte_data(client, 0X25, 0x00);
                  pr_err("## DEBUG : sensor register %X ",var1);
  
                  var2=i2c_smbus_write_byte_data(client, 0X35, 0x00);
                  pr_err("## DEBUG : wake up register  %X",var2);
  
                  var3=i2c_smbus_write_byte_data(client, 0X20, 0x50);
                  pr_err("## DEBUG : freefall register %X",var3);
  
                  var4=i2c_smbus_write_byte_data(client, 0X36, 0x1B);
                  pr_err("## DEBUG : low power register  %X ",var4);
  
                  printk("Starting program\r\n");
  
                  
                  etx_thread=kthread_create(thread_function,NULL,"eTx THread");
                  if(etx_thread)
                  {
                          wake_up_process(etx_thread);
                          printk(KERN_ERR "kthread created successfully...\n");
                  }
                  else
                  {
                          printk(KERN_ERR"cannot create kthread \n");
                  }
          }
          
   kobj_ref=kobject_create_and_add("etx_sysfs",kernel_kobj);
          if(sysfs_create_file(kobj_ref,&etx_attr.attr))
          {
          pr_err("## DEBUG : Cannot create sysfs file......\n");
          }
          else
          {
          pr_err("## DEBUG : func : %s line : %d sysfs entry created successfully\n", __func__, __LINE__);
          }
          
          if(!i2c_check_functionality(client->adapter , I2C_FUNC_I2C))
          {
                  dev_err(&client->dev, "i2c_check_functionality error\n");
                  return -EIO;
          }
          return 0;
  }
  
  int thread_function(void *pv)
  {
          int fr_fall,st,cnt=0;
  
        while(1)
         {
                 fr_fall=i2c_smbus_read_byte_data(p,freefall_reg_ADDR);
                 printk("inside freefall register %X",fr_fall);
                 st=     i2c_smbus_read_byte_data(p,stat_reg);
                 printk("inside status register %X",st);
                 if(st==stat_reg_value)
                 {
                         cnt++;
                         pr_err("## DEBUG : freefall dected for %X times",cnt);
                 }
                 msleep(2000);
 
         }
 }
 static const struct i2c_device_id lis2dw12_i2c_id[] = {
         { "lis2dw12", 0 },
         { },
 };
 
 MODULE_DEVICE_TABLE(i2c, lis2dw12_i2c_id);
 
 static const struct of_device_id lis2dw12_dt_ids[] = {
         { .compatible = "st,lis2dw12" },
         { }
 };
 
 
 
 MODULE_DEVICE_TABLE(of, lis2dw12_dt_ids);
 
 static struct i2c_driver lis2dw12_i2c_driver = {
 
         .driver = {
                 .name   = "lis2dw12",
                 .of_match_table = lis2dw12_dt_ids,
         },
 
         .probe  = lis2dw12_i2c_probe,
         .id_table=lis2dw12_i2c_id,
 };
 
  static ssize_t sysfs_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
 {
                 printk(KERN_INFO "Sysfs - Read!!!\n");
                 return sprintf(buf,"%d",etx_value);
 }
 
  static ssize_t sysfs_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count)
 {
         printk(KERN_INFO "sysfs – write!!!\n");
 sscanf(buf,"%d",&etx_value);
         return count;
 }
 
 
 
 module_i2c_driver(lis2dw12_i2c_driver);
 MODULE_DESCRIPTION("lis2dw12 I2C Driver");
 MODULE_AUTHOR("Guptaji");
 MODULE_LICENSE("GPL");
