#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <wafel/dynamic.h>
#include <wafel/utils.h>
#include <wafel/trampoline.h>
#include <wafel/ios/thread.h>
#include <wafel/patch.h>
#include <wafel/ios/svc.h>
#include <wafel/ios/prsh.h>
#include <wafel/hai.h>

#include "sal.h"
#include "wfs.h"

static void *usb_first_server_handle;
static bool usb_handle_set = 0;
static void ums_attach_hook(trampoline_state *regs){
    if(usb_handle_set)
        return;
    FSSALAttachDeviceArg *attach_arg = (FSSALAttachDeviceArg*)regs->r[0];
    usb_first_server_handle = attach_arg->server_handle;
    usb_handle_set = true;
    debug_printf("usb_first_server_handle: %p\n", usb_first_server_handle);
}

static void wfs_initDeviceParams_exit_hook(trampoline_state *regs){
    WFS_Device *wfs_device = (WFS_Device*)regs->r[5];
    FSSALDevice *sal_device = FSSAL_LookupDevice(wfs_device->handle);
    void *server_handle = sal_device->server_handle;
    debug_printf("wfs_initDeviceParams_exit_hook server_handle: %p\n", server_handle);
    if(usb_handle_set && server_handle == usb_first_server_handle) {
        wfs_device->crypto_key_handle = WFS_KEY_HANDLE_NOCRYPTO;
    }
}

// This fn runs before everything else in kernel mode.
// It should be used to do extremely early patches
// (ie to BSP and kernel, which launches before MCP)
// It jumps to the real IOS kernel entry on exit.
__attribute__((target("arm")))
void kern_main()
{
    // Make sure relocs worked fine and mappings are good
    debug_printf("we in here plugin kern %p\n", kern_main);

    debug_printf("init_linking symbol at: %08x\n", wafel_find_symbol("init_linking"));

    trampoline_hook_before(0x1077eea8, ums_attach_hook);
    
    trampoline_hook_before(0x107435f4, wfs_initDeviceParams_exit_hook);
}

// This fn runs before MCP's main thread, and can be used
// to perform late patches and spawn threads under MCP.
// It must return.
void mcp_main()
{
    // Make sure relocs worked fine and mappings are good
	debug_printf("we in here plugin MCP %p\n", mcp_main);

    debug_printf("done\n");
}