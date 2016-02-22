#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h> /* Specifically, a module */
#include <linux/proc_fs.h> /* Necessary because we use the proc fs */
#include <linux/kernel.h> /* We're doing kernel work */

/*
  IRQ: 19 Interface: eno16777736
*/

// BUFLEN increased to allow virtual NICs
#define BUFLEN 12

// Creating proc struct
struct proc_dir_entry *proc;

struct file_operations proc_fops = {
	read:   read_proc
};

static int irq;
static char interface[BUFLEN];

module_param_string(interface, interface, BUFLEN, 0);
MODULE_PARM_DESC(interface, "A network interface");

module_param(irq, int, 0);
MODULE_PARM_DESC(irq, "The IRQ of the network interface");



static int mycount = 0;

static irqreturn_t myinterrupt(
	int irq, void *dev_id, struct pt_regs *regs) {
	
	mycount++;

	int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) {
		char *data;
		data=PDE_DATA(file_inode(filp));
		if(!(data)){
			printk(KERN_INFO "Null data");
			return 0;
		}
		
		if(count>temp) count=temp;
		temp=temp-count;

		copy_to_user(buf,data, count);
		if(count==0) temp=len;

		return count;
	}

	return IRQ_NONE;
}

void create_new_proc_entry(void) {
	proc=proc_create_data("nicinterrupt",0,NULL,&proc_fops,NULL);
}

/* Interrupt handler */
static int __init mymodule_init(void) {
	if(request_irq(irq, &myinterrupt, IRQF_SHARED, interface , &irq)) {
		printk(KERN_ERR "mymodule: cannot register IRQ%d\n", irq);
		return -EIO;
	}
	proc_init();

	printk("Request on IRQ %d succeeded\n", irq);
	return 0;
}



int proc_init (void) {
	create_new_proc_entry();
 	return 0;
}



static void __exit mymodule_exit(void) {

	/* Unregister module from interrupt handler.
	Remove proc entry. */
	remove_proc_entry("nicinterrupt",NULL);

	free_irq(irq, &irq);
	printk("Freeing IRQ %d\n", irq);
	return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);
MODULE_LICENSE("GPL");