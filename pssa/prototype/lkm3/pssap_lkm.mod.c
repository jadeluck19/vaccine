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
	{ 0xcd71858e, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x1b6314fd, __VMLINUX_SYMBOL_STR(in_aton) },
	{ 0xc111fbdb, __VMLINUX_SYMBOL_STR(sock_create) },
	{ 0xef0ef00c, __VMLINUX_SYMBOL_STR(sock_release) },
	{ 0x6acb2d5d, __VMLINUX_SYMBOL_STR(sock_recvmsg) },
	{ 0x8cdbe595, __VMLINUX_SYMBOL_STR(sock_sendmsg) },
	{ 0x1b9b2208, __VMLINUX_SYMBOL_STR(iov_iter_init) },
	{ 0x987cdbcc, __VMLINUX_SYMBOL_STR(security_bprm_check_unset_pss_filter) },
	{ 0xfa509c21, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0x7b2d2ee9, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0x649a4c3b, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0xdaa79899, __VMLINUX_SYMBOL_STR(security_bprm_check_set_pss_filter) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "8B81AA170724937CB9953D6");
