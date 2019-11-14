#include <linux/module.h>
#include <linux/printk.h>
#include <linux/sched.h>
//nclude <linux/sched/task.h>
#include<linux/syscalls.h>
#include<linux/sched/signal.h>
static int __init ex_init(void)
{
    struct task_struct *task;
int i;
for(i=0;i<5;i++)
{
    for_each_process(task);
    pr_info("%s [%d]\n", task->comm, task->pid);
}
printk("%d",sizeof(run_queue));
    return 0;
}

static void __exit ex_fini(void)
{
}

module_init(ex_init);
module_exit(ex_fini);
