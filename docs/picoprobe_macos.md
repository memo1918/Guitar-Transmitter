# Picoprobe MacOS

In the following the commands, tools and config files I used to debug a RPi Pico with an Mac M1 (2020) are described.

First install OpenOCD like descibed here: Page 62 - <https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf> - Building OpenOCD for Mac OS

Now instal the VSCode Plugin used for debugging: <https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug>.

And adjust two config files for VSCode:

`.vscode/launch.json`

```json
{
    "configurations": [
        {
            "name": "Pico Debug",
            "cwd": "${workspaceRoot}",
            "executable": "${command:cmake.launchTargetPath}",
            "request": "launch",
            "type": "cortex-debug",
            "servertype": "openocd",
            // This may need to be arm-none-eabi-gdb depending on your system
            "gdbPath" : "arm-none-eabi-gdb",
            "device": "RP2040",
            "configFiles": [
                "${workspaceRoot}/openocd/tcl/interface/cmsis-dap.cfg",
                "${workspaceRoot}/openocd/tcl/target/rp2040.cfg"
            ],
            "svdFile": "${workspaceRoot}/pico-sdk/src/rp2040/hardware_regs/rp2040.svd",
            "runToEntryPoint": "main",
            // Work around for stopping at main on restart
            "postRestartCommands": [
                "break main",
                "continue"
            ],
            "searchDir": ["openocd/tcl"],
        }
    ]
}
```

`.vscode/settings.json`

```json
{
    "cmake.generator": "Unix Makefiles",
    // These settings tweaks to the cmake plugin will ensure
    // that you debug using cortex-debug instead of trying to launch
    // a Pico binary on the host
    "cmake.statusbar.advanced": {
        "debug": {
            "visibility": "hidden"
        },
        "launch": {
            "visibility": "hidden"
        },
        "build": {
            "visibility": "default"
        },
        "buildTarget": {
            "visibility": "hidden"
        }
    },
    "cmake.buildBeforeRun": true,
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "cortex-debug.openocdPath": "${OPENOCD_PATH}/src/openocd"
}
```

Caused by the fact that I didnt run `make install` on the openocd folder, to add executables to path and so on, I use absolute Paths in my config files. So replace `${OPENOCD_PATH}` with the absolute path of the OpenOCD folder.

## Command Line

You can also run OpenOCD from Commandline by:

```bash
${OPENOCD_PATH}/src/openocd -f openocd/tcl/interface/cmsis-dap.cfg -c "adapter speed 5000" -f openocd/tcl/target/rp2040.cfg -s openocd/tcl
```

and connect to it using lldb

```bash
lldb
> gdb-remote 3333
> process plugin packet monitor reset
> exit
```

## Troubleshooting

Helpful commands:
<https://www.raspberrypi.com/documentation/microcontrollers/debug-probe.html#installing-gdb>

Debug Probe is working but cant connect to RPi: <https://raspberrypi.stackexchange.com/questions/141321/raspberry-pi-debug-probe-fails-with-error-failed-to-connect-multidrop-rp2040-d> (Also try plug out and in again)

If you get this error: `Error: CMSIS-DAP command CMD_INFO failed.`, it is likely that an openocd instance is already running and connected.
