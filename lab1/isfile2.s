.data
path: .asciz "/home/arvid/is1350/lab1/hello.sjjh"

.bss
sb: .skip 64 # allocate space, skip 64 bytes

.text

.global main
main:
	mov $106,%eax # 106 for sys-call stat
	mov $path,%ebx # move path to %ebx register
	mov $sb,%ecx # move buffer to register %ecx
	int $0x80 # wake up the kernel to run


	# Print
	mov %eax,$path #move output
	mov $4,%eax
	mov $1,%ebx
	mov $14,%edx
	int $0x80 # wake up the kernel to run

	# exit
	mov $1,%eax
	mov $0,%ebx
	int $0x80
	
