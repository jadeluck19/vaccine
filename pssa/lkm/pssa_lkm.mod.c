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
	{ 0x6ee730cb, __VMLINUX_SYMBOL_STR(d_path) },
	{ 0xf1669456, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xd29f126d, __VMLINUX_SYMBOL_STR(send_sig) },
	{ 0x1b6314fd, __VMLINUX_SYMBOL_STR(in_aton) },
	{ 0xef0ef00c, __VMLINUX_SYMBOL_STR(sock_release) },
	{ 0x8e082688, __VMLINUX_SYMBOL_STR(_raw_read_lock) },
	{ 0x6acb2d5d, __VMLINUX_SYMBOL_STR(sock_recvmsg) },
	{ 0x44b1d426, __VMLINUX_SYMBOL_STR(__dynamic_pr_debug) },
	{ 0xac852541, __VMLINUX_SYMBOL_STR(path_get) },
	{ 0x649a4c3b, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x3135ea14, __VMLINUX_SYMBOL_STR(kernel_read) },
	{ 0x8bdd5b5d, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xfa509c21, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0x8cdbe595, __VMLINUX_SYMBOL_STR(sock_sendmsg) },
	{ 0x5a921311, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0x6722f3bc, __VMLINUX_SYMBOL_STR(_raw_write_lock) },
	{ 0x167c5967, __VMLINUX_SYMBOL_STR(print_hex_dump) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xa202a8e5, __VMLINUX_SYMBOL_STR(kmalloc_order_trace) },
	{ 0x7b2d2ee9, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xcc5005fe, __VMLINUX_SYMBOL_STR(msleep_interruptible) },
	{ 0x90552a08, __VMLINUX_SYMBOL_STR(path_put) },
	{ 0xab7d6a08, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x1b9b2208, __VMLINUX_SYMBOL_STR(iov_iter_init) },
	{ 0xb3f7646e, __VMLINUX_SYMBOL_STR(kthread_should_stop) },
	{ 0xdaa79899, __VMLINUX_SYMBOL_STR(security_bprm_check_set_pss_filter) },
	{ 0xb6244511, __VMLINUX_SYMBOL_STR(sg_init_one) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x6df1aaf1, __VMLINUX_SYMBOL_STR(kernel_sigaction) },
	{ 0xc111fbdb, __VMLINUX_SYMBOL_STR(sock_create) },
	{ 0x987cdbcc, __VMLINUX_SYMBOL_STR(security_bprm_check_unset_pss_filter) },
	{ 0x8f99959, __VMLINUX_SYMBOL_STR(vfs_getattr) },
	{ 0x16692398, __VMLINUX_SYMBOL_STR(crypto_alloc_base) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "E73F8DFD1D1D57EEBD547C3");
