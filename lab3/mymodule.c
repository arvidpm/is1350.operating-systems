#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#define BUFLEN 12

/*
  19:    501  0    0   17   IO-APIC   19-fasteoi   eno16777736
*/

static int irq;
static char interface[BUFLEN];


module_param_string(interface, interface, BUFLEN, 0);
MODULE_PARM_DESC(interface, "A network interface");

module_param(irq, int, 0);
MODULE_PARM_DESC(irq, "The IRQ of the network interface");

static int mycount = 0;

static irqreturn_t myinterrupt(int irq, void *dev_id, struct pt_regs *regs) {
	if(mycount < 10) {
		printk("Interrupt!\n");
		mycount++;
	}
	return IRQ_NONE;
}

/* Interrupt handler */
static int __init mymodule_init(void) {
	if(request_irq(irq, &myinterrupt, IRQF_SHARED, interface , &irq)) {
		printk(KERN_ERR "mymodule: cannot register IRQ%d\n", irq);
		return -EIO;
	}

	printk("Request on IRQ %d succeeded\n", irq);
	return 0;
}

static void __exit mymodule_exit(void) {

	/* Unregister module from interrupt handler */
	free_irq(irq, &irq);
	printk("Freeing IRQ %d\n", irq);
	return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);
MODULE_LICENSE("GPL");