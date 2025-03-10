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

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=usbserial";

MODULE_ALIAS("usb:v067Bp2303d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v067Bp04BBd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v067Bp1234d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v067BpAAA2d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v067BpAAA0d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v067Bp23A3d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v067Bp23B3d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v067Bp23C3d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v067Bp23D3d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v067Bp23E3d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v067Bp23F3d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04BBp0A03d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04BBp0A0Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0557p2008d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0547p2008d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v056Ep5003d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v056Ep5004d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0EBAp1080d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0EBAp2080d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF7p0620d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0584pB000d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v2478p2008d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1453p4026d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0731p0528d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v6189p2068d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v11F7p02DFd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04E8p8001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v11F5p0001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v11F5p0003d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v11F5p0004d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v11F5p0005d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0745p0001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v078Bp1234d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10B5pAC70d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v079Bp0027d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0413p2101d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0E55p110Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0731p2003d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v050Dp0257d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v058Fp9720d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v12D1p1001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v11F6p2001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07AAp002Ad*dc*dsc*dp*ic*isc*ip*");
