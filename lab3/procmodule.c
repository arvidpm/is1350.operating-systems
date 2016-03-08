/* 
Arvid Persson Moosavi
amoosavi@kth.se

Quotes and instructions borrowed from:
http://www.crashcourse.ca/introduction-linux-kernel-programming/lesson-12-adding-proc-files-your-modules-part-2 
http://pointer-overloading.blogspot.se/2013/09/linux-creating-entry-in-proc-file.html
https://www.linux.com/learn/linux-training/37985-the-kernel-newbie-corner-kernel-debugging-using-proc-qsequenceq-files-part-1
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

/* Additional required headers */
#include <linux/proc_fs.h> // for the proc filesystem

/* Declarations specifically for the "sequence file" implementation of proc files.
This isn't really needed for such a short printout, but it's best practice */
#include <linux/seq_file.h> 

/* BUFLEN constant increased to allow interface names above 10 chars */
#define BUFLEN 12

/* interface information from /proc/interrupts
  19:    501  0    0   17   IO-APIC   19-fasteoi   eno16777736
*/

static int irq;
static char interface[BUFLEN];

/* procinterrupt file structure */
static struct proc_dir_entry* procinterrupt_file;

// first parameter
module_param_string(interface, interface, BUFLEN, 0);
MODULE_PARM_DESC(interface, "A network interface");

// second parameter
module_param(irq, int, 0);
MODULE_PARM_DESC(irq, "The IRQ of the network interface");

static int mycount = 0;



/* myinterrupt slimmed to keep counting interrupts
----------------------------------------------------------------------------
static irqreturn_t intr_handler(int irq, void *dev_id, struct pt_regs *regs)

irq 	= 	numeric value of the interrupt line the handler is servicing
dev_id 	= 	generic pointer to the same dev_id that was given
			to request_irq() when the interrupt handler was registered
regs 	=	pointer to a structure containing the processor registers and state 
			before servicing the interrupt (used for debugging)
*/
static irqreturn_t myinterrupt(int irq, void *dev_id, struct pt_regs *regs) {

	printk("Interrupt!\n");
	mycount++;

	// module only count interrupts, passes all calls 
	return IRQ_NONE;
}



/*------------------------------------------------------------*/

/* The "show" routine take, as the first argument, 
a pointer to a sequence file structure, and use seq_printf() 
to write to it as much data formatted any way you want, 
then return zero to show success. */
static int procinterrupt_show(struct seq_file *m, void *v) {

	/* Our module cannot output more than one page of data to the pseudo-file at once. 
	A page is a pre-defined amount of memory, typically 4096 bytes, 
	and is available in the PAGE_SIZE macro. 
	This limitation is bypassed by using sequence files, 
	which provide an interface to print multi-paged outputs easily, 
	as a (you guessed it right!) sequence of pages. */
	seq_printf(m, "count: %d\n", mycount);
    return 0;
}

/* "open" your proc file -- ignore that first inode parameter for now, 
and pass a third parameter of NULL to the single_open() routine.*/
static int procinterrupt_open(struct inode *inode, struct file *file)
{
    return single_open(file, procinterrupt_show, NULL);
}

/* The file_operations structure is declared in the header file fs.h 
and represents a collection of file operations that are defined for any type of file, 
not just sequence files. But since sequence files are such a simple type of file, 
you're free to ignore most of the fields in that structure when you define 
an example for a sequence file. */
static const struct file_operations procinterrupt_fops = {
    .owner		= THIS_MODULE,
    .open		= procinterrupt_open,
    .read		= seq_read,
    .llseek		= seq_lseek,
    .release	= single_release,
};

/*------------------------------------------------------------*/


/* Interrupt handler */
static int __init mymodule_init(void) {

	/* Registering interrupt handler on IRQ/interface given in parameters 
	---------------------------------------------------------------------
	request_irq parameters are:
	irq 			= irq given as param2
	&myinterrupt 	= pointer to myinterrupt
	IRQF_SHARED 	= flag (allow sharing the irq among several devices)
	interface 		= name given as param1
	&irq 			= id to interrupt handler
	*/
	if(request_irq(irq, &myinterrupt, IRQF_SHARED, interface , &irq)) {
		printk(KERN_ERR "mymodule: cannot register IRQ%d\n", irq);
		return -EIO; /* I/O error */
	}
	printk("Request on IRQ %d succeeded\n", irq);

	/* ------------------------------------------------------------------------
	proc_create(file_name, permission_bits, parent_dir, file_operations) where 
	"procinterrupt" is name, 
	0 in the permission_bits parameter means default 0444 (only read) permission for file, 
	NULL in parent_dir means that our file is located at /proc. 
	------------------------------------------------------------------------ */
	procinterrupt_file = proc_create("procinterrupt", 0, NULL, &procinterrupt_fops);

	/* if no proc file created */
    if (!procinterrupt_file) {
    	return -ENOMEM; // Out of memory
    }
    /* else */
    printk("/proc/procinterrupt creation successful\n");

    return 0;
}



/* Unregister module from interrupt handler */
static void __exit mymodule_exit(void) {

	/* Removing proc entry */
	printk("Removing proc entry...\n");
	remove_proc_entry("procinterrupt", NULL);

	/* free irq */
	free_irq(irq, &irq);
	printk("Freeing IRQ %d\n", irq);
	return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);
MODULE_LICENSE("GPL");