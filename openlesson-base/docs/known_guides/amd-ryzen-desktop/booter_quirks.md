# Booter Quirks
Returning back to OpenLesson Base - you can now use the integrated configuration editor. Find the section labelled BOOTER.

Find and modify the following values under the "Quirks" section in booter.
       KEY       ENABLED
- DevirtualiseMmio: NO - If you have a TRx40 system, you should ENABLE this and follow these Dortania instructions: [KASLR-FIX](https://dortania.github.io/OpenCore-Install-Guide/extras/kaslr-fix.html)
- EnableUnwriteProtector: NO
- RebuildAppleMemoryMap: YES
- ResizeAppleGpuBars: -1 - If your firmware supports increasing GPU Bar Sizes (ResizableBar), set this to 0!
- SetupVirtualMap: YES - Except on X570, B550, A520, X470 and B450 boards which may require this to be disabled.
- SyncRuntimePermissions: YES