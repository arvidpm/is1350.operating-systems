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

/* BUFLEN constant increased to allow interface names above 10 chars */
#define BUFLEN 12

/* interface information from /proc/interrupts
  19:    501  0    0   17   IO-APIC   19-fasteoi   eno16777736
*/

static int irq;
static char interface[BUFLEN];

// first parameter
module_param_string(interface, interface, BUFLEN, 0);
MODULE_PARM_DESC(interface, "A network interface");

// second parameter
module_param(irq, int, 0);
MODULE_PARM_DESC(irq, "The IRQ of the network interface");

static int mycount = 0;

/*----------------------------------------------------------------------------
static irqreturn_t intr_handler(int irq, void *dev_id, struct pt_regs *regs)

irq 	= 	numeric value of the interrupt line the handler is servicing
dev_id 	= 	generic pointer to the same dev_id that was given
			to request_irq() when the interrupt handler was registered
regs 	=	pointer to a structure containing the processor registers and state 
			before servicing the interrupt (used for debugging)

*/

/* Interrupt handler */
static irqreturn_t myinterrupt(int irq, void *dev_id, struct pt_regs *regs) {
	if(mycount < 10) {
		printk("Interrupt!\n");
		mycount++;
	}
	// module only count interrupts, passes all calls 
	return IRQ_NONE;
}



/* Register interrupt */
static int __init mymodule_init(void) {

	/* Registering interrupt handler on IRQ/interface given in parameters 
	---------------------------------------------------------------------
	request_irq parameters are:
	irq 			= irq given as param2
	&myinterrupt 	= pointer to myinterrupt
	IRQF_SHARED 	= flag (allow sharing the irq among several devices).
					  These flags used only by the kernel as part of the
					  irq handling routines.
	interface 		= name given as param1
	&irq 			= id to interrupt handler
	*/
	if(request_irq(irq, &myinterrupt, IRQF_SHARED, interface , &irq)) {
		printk(KERN_ERR "mymodule: cannot register IRQ%d\n", irq);
		return -EIO; /* I/O error */
	}

	/* else */
	printk("Request on IRQ %d succeeded\n", irq);
	return 0;
}



/* Unregister module from interrupt handler */
static void __exit mymodule_exit(void) {

	/* free irq */
	free_irq(irq, &irq);
	printk("Freeing IRQ %d\n", irq);
	return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);
MODULE_LICENSE("GPL");