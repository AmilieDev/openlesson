# Downloading macOS
Before we may create an EFI we need to select a version of macOS to download. For this guide, we currently only support Tahoe.
That is due to nuances in software, and hardware - and the reason that it is simply easier to support one OS.

When downloading Tahoe, it'll download to ~/OLB or C:/Users/[user]/OLB under the folder com.apple.recovery.boot. You will need to move this folder onto your EFI USB stick.
We will create the USB stick after downloading macOS.

Simply click the below button to download macOS and it'll do the hard part for you. It can take up to a few hours depending on your internet connection.

# Formatting your USB drive on Windows
In order to format your drive on Windows, download a tool called Rufus from https://rufus.ie/ and plug in your USB. Once you have done that you should select the following options:

- Set BOOT SELECTION to Non Bootable
- Set FILESYSTEM to Large FAT32 (or regular FAT32 if large is unavailable)
- Set PARTAITION SCHEME to GPT
- Set TARGET SYSTEM to BIOS or UEFI
- Name it whatever you'd like.
- Click READY.

Once you have a working USB via Rufus, simply drag com.apple.recovery.boot from C:/Users/[user]/OLB to the root of the USB stick.

# Formatting your USB drive on macOS
In order to format your USB drive on macOS open the tool Disk Utility program. From there - plug your USB stick in and select the following options:

- Select the USB in the side bar (on the left).
- Click ERASE.
- Confirm you want to do this.
- Set the filesystem you are erasing to to be FAT32.
- Name it anything.
- Click ERASE.

Once that is complete, drag over com.apple.recovery.boot from ~/OLB to your USB stick.

# Formatting your USB drive on Linux
In order to format your USB drive on Linux - open your Terminal Emulator of choice and run the following command: lsblk.

With that command, identify your USB stick - it usually starts with sda, sdb or similar.

You should then run the following command:
- sudo umount /dev/[USB location]
- sudo mkfs.fat -F 32 /dev/[usb location]
- sudo mkdir -p /mnt/oc-usb
- sudo mount /dev/[usb location + partition id] /mnt/oc-usb

Once that is complete, drag over com.apple.recovery.boot from ~/OLB to your USB stick.