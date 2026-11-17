This guide was ported from [Dortania's OpenCore Install Guide](https://dortania.github.io/OpenCore-Install-Guide/prerequisites.html) - See here for the original source!

Before we begin we need to make something clear:
- Do NOT use configurators like OpenCore Simplify, OCAT or 1401. They WILL mangle your work and break what you've made.
- You MUST edit the config.plist or it WILL NOT work.
- OpenCore has NO default properties. All properties must be filled in or it will not work.

Please download the following tools before beginning:
- [GenSMBIOS](https://github.com/corpnewt/GenSMBIOS) (Will be integrated into OpenLesson at a later date)

# ACPI
Assuming you know your hardware, you can begin by gathering your ACPI files. Unfortunately, at current OpenLesson cannot automatically manage your ACPI and Kexts. It is advised to use the tool [ProperTree](https://github.com/corpnewt/ProperTree) for now.

- [SSDT-EC-USBX](https://dortania.github.io/Getting-Started-With-ACPI/) - All users should download this to fix the embedded controller and USB power.
- [SSDT-CPUR](https://github.com/naveenkrdy/Misc/blob/master/SSDTs/SSDT-CPUR.dsl) - Only users with **B550 and A520** motherboards should use this kext. It fixes CPU definitions. Do NOT use it if you do not have one of those motherboards.

For now, click SAVE on the configuration file on the right of your screen, and open ProperTree - once in ProperTree press Control/Cmd+O and open the .plist either at ~/OLB/config.plist or C:/Users/[user]/OLB/config.plist. Once you have it open, please press Ctrl/Cmd + R to automatically generate the needed definitions.

You can now ignore the Delete, Patch and Quirks options on the rest of this section.