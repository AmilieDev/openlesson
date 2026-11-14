# Want to make / port a guide?
All guides currently are ported directly from Dortania's guide over at https://dortania.github.io - it'd be great if you could port more of them over!

To create a new guide you need to go the following folder: openlesson-base/docs and in there modify cpu_families.json to add your new family. For example:

```        "match": {
            "cpuBrandContains": ["AMD Ryzen"],
            "formFactor": "desktop"
        },
            "preset": "amd-ryzen-desktop",
            "show": ["ACPI", "Booter/Quirks", "Kernel/Quirks"]
        },
```
        
In that vain, you should also add a new folder matching the name of the "preset" you made. In this case - make a new folder called amd-ryzen-desktop. You should add the following files:
- acpi.md
- booter_quirks.md
- kernel_quirks.md 

THEY MUST GO BY THIS NAME OR THEY WILL NOT WORK.

In there, using Markdown format the guides to follow the standard formatitng of the rest of the application - see amd-ryzen-desktop and its guides for inspiration / an example.
Do NOT modify "generic". Generic is the initial guide instances we utilize for general setup steps, such as downloading the macOS recovery environment and post setup preparation.