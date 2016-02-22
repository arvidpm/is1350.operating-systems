.data
path: .asciz "/home/arvid/is1350/lab1/hello.s"

.bss
sb: .skip 64 # allocate space, skip 64 bytes

.text

.global main
main:
	mov $106,%eax # 106 for sys-call stat
	mov $path,%ebx # move path to %ebx register
	mov $sb,%ecx # move buffer to register %ecx
	int $0x80 # wake up the kernel to run

	# exit
	mov %eax,%ebx # status number we will return to the OS placed in %ebx
	mov $1,%eax # move sys-call exit to %eax register
	int $0x80 # wake up the kernel to run 
