#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
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
	{ 0xae141548, "module_layout" },
	{ 0x73f48ce0, "gpio_export" },
	{ 0x7cbead0b, "gpio_request" },
	{ 0xe8b8056d, "__register_chrdev" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xfe990052, "gpio_free" },
	{ 0xe61a6d2f, "gpio_unexport" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0x960661f4, "gpio_get_value" },
	{ 0x65d6d0f0, "gpio_direction_input" },
	{ 0xc40f91c2, "gpio_set_value" },
	{ 0xf2a644fb, "copy_from_user" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0xb1f975aa, "unlock_kernel" },
	{ 0x3656bf5a, "lock_kernel" },
	{ 0xb72397d5, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=driverfox";

