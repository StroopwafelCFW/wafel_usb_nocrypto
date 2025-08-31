# wafel_usb_nocrypto

This plugin will disable encryption for the first real usb storage device connected

## How to use

- Copy the `4usbmlc.ipx` to `sd:/wiiu/ios_plugins` and optionally `/storage_slc/sys/hax/ios_plugins`
- Make sure you have only the USB device connected you want to install to
- If you use the usb partition plugin, make sure to to also put that in `sd:/wiiu/ios_plugins`
- Rebuild the MLC with the wafel_mlc_setup. **This will format the USB storage if it is detected for whatever reason as unformatted**

## Building

```bash
export STROOPWAFEL_ROOT=/path/too/stroopwafel-repo
make
```
