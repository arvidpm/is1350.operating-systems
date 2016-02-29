#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x6775bbc2, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x58431907, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xb86f3e2c, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x814fae71, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x10fe443c, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x7bfe26e7, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x56d4ca56, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x8420e445, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "0FF472A0D8C23DD419F5CE0");
