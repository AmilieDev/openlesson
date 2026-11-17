# Misc
This covers the misc section of the config. You CANNOT forget this section.

This is optional but if you'd like to hide additional options in OpenCore, set `HideAuxiliary` to YES. You will need to press space to show macOS recovery and other auxiliary entries.

In the DEBUG section of misc:
       KEY       ENABLED
AppleDebug - YES
ApplePanic - YES
DisableWatchDog - YES
Target - 67 - THIS IS A NUMBER.

Security is pretty self explainatory and it CANNOT be skipped. Set the following values:
       KEY       ENABLED
AllowSetDefault - Yes
BlacklistAppleUpdate - Yes
ScanPolicy - 0
SecureBootModel - Default
Vault - Optional - this is CASE SENSITIVE. You need to make sure it says Optional.