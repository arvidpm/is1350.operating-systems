#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/types.h>

struct proc_dir_entry *proc;
int len,temp;
char *msg;

int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{
char *data;
data=PDE_DATA(file_inode(filp));
if(!(data)){
printk(KERN_INFO "Null data");
return 0;
}

if(count>temp)
{
count=temp;
}
temp=temp-count;

copy_to_user(buf,data, count);
if(count==0)
temp=len;
return count;
}

struct file_operations proc_fops = {
read: read_proc
};

void create_new_proc_entry(void) {

msg="Hello World\n";
proc=proc_create_data("hello",0,NULL,&proc_fops,msg);

len=strlen(msg);
temp=len;
}


int proc_init (void) {
 create_new_proc_entry();
 return 0;
}

void proc_cleanup(void) {
 remove_proc_entry("hello",NULL);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
