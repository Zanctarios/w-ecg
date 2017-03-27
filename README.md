## Summary

Code used for the CC1310, SPI, and UART.

## Peripherals Exercised

SPI0
UART0

## Application Design Details

This examples is the same as the Empty_Minimal example except many
development and debug features are enabled. For example:

* Kernel Idle task
* Stack overflow checking
* Logs or Asserts are enabled

> The ROM is being used in this example. This is controlled
> by the following lines in the *.cfg* file:

> ```
    var ROM = xdc.useModule('ti.sysbios.rom.ROM');
    if (Program.cpu.deviceName.match(/CC26/)) {
        ROM.romName = ROM.CC2650;
    }
    else if (Program.cpu.deviceName.match(/CC13/)) {
        ROM.romName = ROM.CC1350;
    }
```

> Since the kernel in the ROM is being used, there is no logging or assert
checking done by the kernel.

> For IAR users using any SensorTag(STK) Board, the XDS110 debugger must be
selected with the 4-wire JTAG connection within your projects' debugger
configuration.

## References

* For GNU and IAR users, please read the following website for details
  about enabling [semi-hosting](http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting)
  in order to view console output.

* Please refer to the Memory Footprint Reduction section in the
TI-RTOS User Guide *spruhd4.pdf* for a complete and detailed list of the
differences between the empty minimal and empty projects.
