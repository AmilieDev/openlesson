# Kernel Quirks
This is a smaller section, but you should now change these quirks in order to get a working kext.

Go to the EMULATE section and change:
- DummyPowerManagement to YES

This patch section NEEDS to be done. It CANNOT be ignore or macOS will NOT boot.

Download the patches.plist from [AMD_VANILLA](https://github.com/AMD-OSX/AMD_Vanilla), and open patches.plist in ProperTree. Similarly, click SAVE at the top right and open this config in there too, as you did for ACPI.
Once you have these, right click the PATCH section, INSIDE the Kernel section in your main config.plist and DELETE it.  go to the config with AMD patches, and right click them and copy them - then paste them into the main config.plist.

You need to modify four patches, all of which are named algrey - force cpuid_cores_per_package. You only need to replace the Replace value. You can do this here, or in ProperTree.
`B8000000 0000` => `B8 <core count> 0000 0000`
`BA000000 0000` => `BA <core count> 0000 0000`
`BA000000 0090` => `BA <core count> 0000 0090`
`BA000000 00` => `BA <core count> 0000 00`

If you also wish to run Golden Gate eventually, find all instances of 25.99.99 in patches and bump it up to 27.99.99.

Now find the QUIRKS section under the kernel options and change these:
       KEY       ENABLED
PanicNoKextDump - YES
PowerTimeoutKernelPanic - YES
ProvideCurrentCpuInfo - YES - IT WILL NOT BOOT WITHOUT THIS ENABLED.
XhciPortLimit - YES - Disable this if you are running macOS 11.3 or later.
