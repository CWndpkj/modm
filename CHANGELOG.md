# Changelog

The purpose of this changelog is to give a good overview of the changes in modm.
It is an editorialized version of the git history, with features, PRs and fixes
chosen for their importance to modm users as judged by the maintainers.
This means that not every small commit makes it in here.

<details>
<summary>Changelog format</summary>

The release summaries are formatted as:

> #### yyyy-mm-dd: yyyyqn release
>
> Breaking changes: [list]  
> Features: [list]  
> Fixes: [list]  
> New development boards: [list]  
> New device drivers: [list]  
> Known bugs: [list]  
> Contributors: [list]
>
> PR #[GitHub] -> [release tag].

With a more detailed changelog below with this format:

> ##### yyyy-mm-dd: [summary]
>
> [detailed summary]
>
> PR #[GitHub] -> [sha1] (with [level] impact (on [scope])).  
> Tested in hardware by [contributor].

Note that `-> [sha1]` means "all changes up to and including this commit" which
needs to be interpreted in the context of the git history.
This usually corresponds to the changes in the Pull Request on GitHub though.

This changelog also labels the changes with an impact and usually also scopes
them to the relevant subset of modm code or targets.
This describes the anticipated impact of these changes on users of modm and has
four levels:

1. No impact mentioned: No influence. Adds new features and does not touch existing code.
2. Low impact: No expected influence. Modifies modm in a backwards compatible way.
3. Medium impact: Likely influence. Modifies modm in a non-backwards compatible way.
4. High impact: Definite influence. Breaks the API within the scope.

High impact changes are explicitly called out in the release notes for you to
pay attention to. Medium impact changes are also worth looking at.

</details>

<!--releases-->

## 2024-07-01: 2024q2 release

This release covers everything from 2024-04-01 and has been tested with avr-gcc
v12.2.0 from Upstream and arm-none-eabi-gcc v12.2.1 from xpack.

Breaking changes:

- The STM32 UART peripheral moves the buffer configuration from lbuild to C++.
- Renames `modm::fiber::yield()` to `modm::this_fiber::yield()`.
- Renames `modm::fiber::sleep()` to `modm::this_fiber::sleep_for()`.

Features:

- Added `:platform:cortex-m:linkerscript.flash_reserved` option to reserve space
  at the end of FLASH.
- Add DMA capabilities to STM32F0/G0 ADC.
- Implement full atomics interface for AVR and STM32.
- Implement C++ concurrency interface for fibers.
- Add fiber stack overflow detection on ARMv8-M.

Integrated Projects:

- nlohmann/json added at v3.11.3.

Fixes:

- Communicate `:platform:cortex-m:linkerscript.flash_offset` to dfu-util tool.
- Fix flash section erasing on STM32G4.
- Fix static initialization guard implementation.

New device drivers:

- AS5600 absolute encoder driver as [`modm:driver:as5600`][].

Known bugs:

- STM32F723 is missing support to use built-in USB HS PHY. See [#1171][].
- OpenOCD cannot enable SWO on STM32H7 targets. See [#1079][].
- Compiling Protothreads and Resumable Functions is broken on GCC13. See [#1012][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Akos Becsey ([@becseya][]) 🎉
- Christopher Durand ([@chris-durand][])
- Daniel Waldhäusl
- Dima Barsky ([@kapacuk][])
- Hannes Ellinger ([@el-han][]) 🎉
- Henrik Hose ([@hshose][])
- Niklas Hauser ([@salkinium][])
- Thomas Sommer ([@TomSaw][])

PR [#1175][] -> [2024q2][].

<details>
<summary>Detailed changelog</summary>

#### 2024-06-24: Move buffer configuration from lbuild to C++ for STM32

The buffer configuration is now implemented in C++.
Please replace any `modm::platform::U(s)artN` type with:

```cpp
using namespace modm::platform;
// Using only hardware buffers for Tx and Rx (both 1 symbol)
using UartN = BufferedUart<U(s)artHalN>;
// using only TX software buffer
using UartN = BufferedUart<U(s)artHalN, UartTxBuffer<512>>;
// using only RX software buffer
using UartN = BufferedUart<U(s)artHalN, UartRxBuffer<256>>;
// using both TX and RX software buffers
using UartN = BufferedUart<U(s)artHalN, UartTxBuffer<512>, UartRxBuffer<256>>;
```

PR [#1156][] -> [cf7769b][].  
Tested in hardware by [@kapacuk][] and [@salkinium][].

#### 2024-06-17: Add AS5600 absolute encoder driver

PR [#1166][] -> [55c88ba][].  
Tested in hardware by [@tomsaw][].

#### 2024-06-16: Add fiber stack overflow detection on ARMv8-M

PR [#1168][] -> [6be3199][].  
Tested in hardware by [@salkinium][].

#### 2024-05-20: Implement concurrency interface for fibers

PR [#1026][] -> [39a9f4d][].  
Tested in hardware by [@salkinium][].

#### 2024-05-20: Implement all atomics for AVR and STM32

PR [#1164][] -> [8a924f9][].  
Tested in hardware by [@salkinium][].

#### 2024-05-19: Add DMA capabilities to STM32F0/G0 ADC

PR [#1136][] -> [901a3dc][].  
Tested in hardware by [@victorandrehc][].

#### 2024-05-19: Fix STM32G4 flash erase

PR [#1146][] -> [71becdc][].  
Tested in hardware by [@hshose][].

</details>


## 2024-04-01: 2024q1 release

This release covers everything from 2024-01-01 and has been tested with avr-gcc
v12.2.0 from Upstream and arm-none-eabi-gcc v12.2.1 from xpack.

Breaking changes:

- TinyUSB v0.16 implements a DFU class in addition to a DFU *runtime* class.
  Therefore the `modm:tinyusb:config:device.dfu` option was renamed to
  `device.dfu_rt` to align with the TinyUSB naming.
- TinyUSB v0.16 improves support for using two USB ports at the same time. A
  differnt port can now be chosen for device and host classes. In addition, the
  HS peripheral may be operated in FS mode, therefore the `modm:tinyusb:speed`
  option was renamed to `modm:tinyusb:max-speed` since it now has a different
  meaning.
- Remove the separate `hosted-rpi` Raspberry Pi target in favor of plain
  `hosted-linux`. Its GPIO implementation based on wiringPi is removed.

Features:

- Add USB High-Speed support to all development boards with ULPI interface.
- Catch accidental use of Newlib's printf implementation.
- Fibers are now implemented for ARM64, albeit only on Linux and macOS.
- Add BDMA driver for STM32H7.
- DMA support for ADC driver on STM32F3 and STM32G4.
- Support for ADC conversion sequences on STM32F3 and STM32G4.
- Add Prescaler+Counter constexpr algorithm and use it in `Iwdg::initialize()`.
- Add PWM modes, break input and comparator to Timer driver on STM32.
- CMake generator now works with multiple lbuild repositories.
- Allow setting I2C interrupt priority for STM32 and SAMx7x.

Integrated Projects:

- TinyUSB upgraded to v0.16.0.
- STM32G0 headers upgraded to v1.4.4.
- STM32G4 headers upgraded to v1.2.3.
- STM32H5 headers upgraded to v1.2.0.
- STM32H7 headers upgraded to v1.10.4.
- STM32L5 headers upgraded to v1.0.6.
- STM32U5 headers upgraded to v1.4.0.
- STM32WB headers upgraded to v1.12.1.

Fixes:

- Fix `Rcc::enable()` for STM32H7 comparator.
- Fix Timer 23 and 24 counter size on STM32H7.
- Fix ADC asynchronous clock on STM32H7.
- Fix DMA enable on STM32G0.
- Fix reconfiguring DMAMUX requests.
- Fix ADC sampling time configuration on STM32F3.
- Separate ULPI clock from USB clock on STM32 to allow using OTGHS in FS mode.
- Stack alignment underneath promise in fiber tasks.
- Move `Iwdg` driver to `modm::platform::Iwdg` namespace.
- Fix flash page size for large F103, F105 and F107 devices.

New development boards:

- DISCO-F401VC as [`modm:disco-f401vc`][].
- NUCLEO-G070RB as [`modm:nucleo-g070rb`][].
- DISCO-F411VE as [`modm:disco-f411ve`][].

New device drivers:

- AS5047 absolute encoder driver as [`modm:driver:as5047`][].

Known bugs:

- OpenOCD cannot enable SWO on STM32H7 targets. See [#1079][].
- Compiling Protothreads and Resumable Functions is broken on GCC13. See [#1012][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Carl Treudler ([@cajt][])
- Christopher Durand ([@chris-durand][])
- Daniel Waldhäusl
- Dima Barsky ([@kapacuk][]) 🎉
- Henrik Hose ([@hshose][])
- Michael Jossen ([@Javask][]) 🎉
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])
- Thomas Sommer ([@TomSaw][])
- Victor Costa ([@victorandrehc][])

PR [#1153][] -> [2024q1][].

<details>
<summary>Detailed changelog</summary>

#### 2024-03-15: Allow I2C interrupt priority to be set

PR [#1143][] -> [734de07][] and PR [#1148][] -> [e1d8a17][].  

#### 2024-03-09: Adapt CMake generator to multiple lbuild repositories

PR [#1140][] -> [1a89fbe][].  
Tested in hardware by [@Javask][].

#### 2024-03-08: Extend Timer features for STM32G0

PR [#1134][] -> [a371df6][].  
Tested in hardware by [@victorandrehc][].

#### 2024-03-08: Add AS5047 absolute encoder driver

PR [#1138][] -> [dbfd93b][].  
Tested in hardware by [@hshose][].

#### 2024-03-06: Add DISCO-F411VE board support

PR [#1135][] -> [009bb06][].  
Tested in hardware by [@tomsaw][].

#### 2024-02-06: Use Prescaler+Counter algorithm for `Iwdg::initialize()`

PR [#1127][] -> [23036e3][].  
Tested in hardware by [@salkinium][].

#### 2024-02-05: Add DMA and conversion sequence support to STM32F3/G4 ADC

PR [#1126][] -> [fe4cbc5][].  
Tested in hardware by [@chris-durand][].

#### 2024-02-02: Add NUCLEO-G070RB board support

PR [#1125][] -> [6de70ba][].  
Tested in hardware by [@chris-durand][].

#### 2024-02-01: Add BDMA driver for STM32H7

PR [#1115][] -> [2e40ab4][].  
Tested in hardware by [@chris-durand][].

#### 2024-01-21: Implement Fibers for ARM64

New targets with `-arm64` suffix, `-x86_64` is implicit for backwards
compatibility:

- `hosted-linux-arm64`
- `hosted-darwin-arm64`

Removed `hosted-rpi` target, as wiringPi is deprecated and thus the target lost
its only peripheral.

PR [#1113][] -> [623a13b][].  
Tested in hardware by [@salkinium][] (Apple Silicon M2) and [@rleh][] (Raspberry Pi).

#### 2024-01-14: Catch Newlib's printf usage

This previously lead to cryptic linking failures due to a missing heap
implementation. Now any use of `printf` without including the `modm:printf`
module will lead to a linking failure with a proper error description.

PR [#1120][] -> [7318c28][].  
Tested by [@salkinium][].

#### 2024-01-05: Upgrade TinyUSB to v0.16.0

PR [#1116][] -> [1f210c1][].  
Tested in hardware by [@salkinium][] with **high** impact on TinyUSB configuration.

#### 2024-01-05: Add DISCO-F401VC board support

PR [#1117][] -> [2381c61][].  
Tested in hardware by [@cajt][].

#### 2024-01-04: Miscellaneous fixes for STM32H7 and STM32G0

PR [#1114][] -> [8bcbe25][].  
Tested in hardware by [@chris-durand][].

</details>


## 2024-01-01: 2023q4 release

This release covers everything from 2023-10-01 and has been tested with avr-gcc
v12.2.0 from Upstream and arm-none-eabi-gcc v12.2.1 from xpack.

Features:

- STM32H7 SPI driver with DMA support.

Integrated Projects:

- ETL upgraded to v20.38.10.
- LVGL upgraded to v8.3.11.
- Nanopb upgraded to v0.4.8.
- STM32G0 headers upgraded to v1.4.3.
- STM32F4 headers upgraded to v2.6.9.
- STM32U5 headers upgraded to v1.3.1.

Fixes:

- Fix SPI clocks on Nucleo-H723ZG boards.
- Do not require protothreads to use SPI with fibers.
- Place main stack in DMA-able SRAM on STM32H7.

New device drivers:

- BMI088 IMU driver as [`modm:driver:bmi088`][].

Known bugs:

- Fibers are not implemented for ARM64 targets. See [#1111][].
- OpenOCD cannot enable SWO on STM32H7 targets. See [#1079][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Christopher Durand ([@chris-durand][])
- Jens Böckmann ([@jensboe][]) 🎉
- Niklas Hauser ([@salkinium][])
- Sergey Pluzhnikov ([@ser-plu][])

PR [#1112][] -> [2023q4][].

<details>
<summary>Detailed changelog</summary>

#### 2023-10-04: Add STM32H7 SPI driver with DMA support

PR [#1052][] -> [53796b0][].  
Tested in hardware by [@chris-durand][].

#### 2023-10-05: Add BMI088 driver

PR [#1052][] -> [a771042][].  
Tested in hardware by [@chris-durand][].

</details>


## 2023-10-01: 2023q3 release

This release covers everything from 2023-07-01 and has been tested with avr-gcc
v12.2.0 from Upstream and arm-none-eabi-gcc v12.2.1 from xpack.

Features:

- STM32G0 FD-CAN driver.
- Redirect printf to logger in BSPs that support it.

Integrated Projects:

- CMSIS-DSP upgraded to v1.15.0.
- ETL upgraded to v20.38.2.
- LVGL upgraded to v8.3.10.
- STM32C0 headers upgraded to v1.1.0.
- STM32H5 headers upgraded to v1.1.0.
- STM32L4 headers upgraded to v1.7.3.
- STM32U5 headers upgraded to v1.3.0.

Fixes:

- Place `.data` section in DMA-able memory on STM32H7.
- Correct identification register indices for HMC58x3 drivers.
- Fix spurious EXTI interrupt with shared IRQ.

New device drivers:

- IIM-42652 IMU family driver as [`modm:driver:ixm42xxx`][].
- Stacked SPI Flash driver as [`modm:driver:block.device:spi.stack.flash`][].
- SX128x LoRa driver as [`modm:driver:sx128x`][].
- QMC5883L magnetometer driver as [`modm:driver:qmc5883l`][].

Known bugs:

- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Alexander Solovets ([@mbait][]) 🎉
- Christopher Durand ([@chris-durand][])
- Klaus Schnass ([@klsc-zeat][]) 🎉
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])
- Rasmus Kleist ([@rasmuskleist][])

PR [#1088][] -> [2023q3][].

<details>
<summary>Detailed changelog</summary>

#### 2023-09-11: Add QMC5883L magnetometer driver

PR [#1063][] -> [7f30961][].  
Tested in hardware by [@mbait][].

#### 2023-09-02: Fix EXTI interrupts

PR [#1066][] -> [c9e5227][].  
Tested in hardware by [@chris-durand][].

#### 2023-08-15: Add SX128x LoRa driver

PR [#1050][] -> [517bd84][].  
Tested in hardware by [@rasmuskleist][].

#### 2023-08-13: Add Stacked SPI Flash driver

PR [#1054][] -> [ba23833][].  
Tested in hardware by [@rasmuskleist][].

#### 2023-07-27: Add ADC driver for STM32H7

Also adds support for injected conversions.

PR [#1049][] -> [4d69227][].  
Tested in hardware by [@chris-durand][].

#### 2023-07-26: Fix ADC driver for STM32G0

PR [#1053][] -> [5d03d53][].  
Tested in hardware by [@chris-durand][].

#### 2023-07-21: Add CAN driver for STM32G0

PR [#1051][] -> [a05cc62][].  
Tested in hardware by [@klsc-zeat][].

#### 2023-07-19: Add IIM-42652 IMU driver

PR [#1040][] -> [8012d82][].  
Tested in hardware by [@rasmuskleist][].

#### 2023-07-10: Place `.data` section into D1_SRAM on STM32H7

PR [#1048][] -> [027811f][].  
Tested in hardware by [@chris-durand][].

</details>


## 2023-07-01: 2023q2 release

This release covers everything from 2023-04-05 and has been tested with avr-gcc
v12.2.0 from Upstream and arm-none-eabi-gcc v12.2.1 from xpack.

Breaking changes:

- GCC12 requirement for C++23.
- `-fsingle-precision-constant` default compile flag has been removed.
- Removed deprecated `Timer::setPeriod(uint32_t)` in favor of`std::chrono` units.

Features:

- SAMx7x DAC, ADC, DMA, and CAN drivers.
- Enabled C++23 and C23.
- STM32 IWDG driver.
- Fibers are now backward compatible with protothreads and resumable functions.
- Support for STM32G0B/C devices with shared interrupts.

Integrated Projects:

- LVGL upgraded to v8.3.7.
- Pico-SDK upgraded to v1.5.1.
- STM32F1 headers upgraded to v4.3.4.
- STM32F2 headers upgraded to v2.2.6.
- STM32L1 headers upgraded to v2.3.3.
- CMSIS-DSP upgraded to v1.14.4.
- SAMx7x upgraded to v3.0.
- TinyUSB upgraded to v0.15.0.

Fixes:

- Moving average type selection.
- SysTick clock access from both cores on RP2040.
- FDCAN driver on STM32 tx message queue.
- STM32 I2C NACK flag is acknowledged for instances >1.
- Fix arithmetic overflow in `Timer::setPeriod` on STM32.
- Validate calculated CRC on MS5611 driver.

New development boards:

- Adafruit Feather-M4 as [`modm:feather-m4`][].

New device drivers:

- MAX31865 temperature sensor as [`modm:driver:max31865`][].
- Internal cycle counter as [`modm:driver:cycle_counter`][].
- MCP3008 ADC driver as [`modm:driver:mcp3008`][].

Known bugs:

- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Christopher Durand ([@chris-durand][])
- Daniel Waldhäusl 🎉
- Henrik Hose ([@hshose][])
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])
- Rasmus Kleist ([@rasmuskleist][])
- Sascha Schade ([@strongly-typed][])
- Sergey Pluzhnikov ([@ser-plu][])
- Thomas Rush ([@tarush53][])
- Victor Costa ([@victorandrehc][])
- Vivien Henry ([@lukh][])

PR [#1044][] -> [2023q2][].

<details>
<summary>Detailed changelog</summary>

#### 2023-06-20: Extend support for ELF2UF2 tool to STM32 and SAM

PR [#1038][] -> [b05df3e][].  
Tested in hardware by [@tarush53][].

#### 2023-06-09: Remove `-fsingle-precision-constant` compile flag

PR [#1037][] -> [2384756][] with medium-impact in floating point variables.  
Tested in hardware by [@salkinium][].

#### 2023-06-08: Add Adafruit Feather-M4 board support

PR [#1032][] -> [81b86be][].  
Tested in hardware by [@tarush53][].

#### 2023-06-07: Add support for STM32G0B/C devices

PR [#1036][] -> [768d749][].  
Tested in hardware by [@chris-durand][].

#### 2023-06-01: Add MCP3008 ADC driver

PR [#1028][] -> [eda224e][].  
Tested in hardware by [@chris-durand][].

#### 2023-05-19: Add complementary channels to TIM driver on STM32

PR [#1018][] -> [45ae68a][].  
Tested in hardware by [@ser-plu][].

#### 2023-05-15: Implement Protothreads/Resumables using Fibers

PR [#1001][] -> [45ae68a][].  
Tested in hardware by [@salkinium][].

#### 2023-05-13: Fix FDCAN transmission queue on STM32

PR [#1017][] -> [9d33843][].  
Tested in hardware by [@ser-plu][], [@chris-durand][], and [@rleh][].

#### 2023-05-09: Add MCAN driver for SAMx7x

PR [#955][] -> [bfafcd3][].  
Tested in hardware by [@rleh][].

#### 2023-05-05: Add IWDG driver for STM32

PR [#1009][] -> [d772940][].  
Tested in hardware by Daniel Waldhäusl.

#### 2023-05-03: Fix RP2040 multicore access to modm::Clock

PR [#1010][] -> [389a9c3][].  
Tested in hardware by [@salkinium][].

#### 2023-05-02: Add MAX31865 temperature sensor

PR [#993][] -> [65bbccf][].  
Tested in hardware by [@hshose][].

#### 2023-04-19: Add ADC driver for SAMx7x

PR [#998][] -> [c7c4c57][].  
Tested in hardware by [@chris-durand][].

#### 2023-04-12: Add DAC and DMA driver for SAMx7x

PR [#987][] -> [94580b4][].  
Tested in hardware by [@chris-durand][].

#### 2023-04-08: Fix STM32 SPI configuration while running

PR [#994][] -> [972b74b][].  
Tested in hardware by [@chris-durand][].

</details>


## 2023-04-06: 2023q1 release

This release covers everything from 2023-01-01 and has been tested with avr-gcc
v12.2.0 from Upstream and arm-none-eabi-gcc 12.2.rel1 from Arm.


Breaking changes:

- SAMV71 Xplained Ultra button polarity.

Features:

- External main clock on SAMx7x.
- SAMx7x SPI driver.
- SAM GpioInverted.
- CANFD large frame support.
- SAM SoftwareGpioPort.
- SAMx7x Timer/counter driver.
- SAMx7x PWM generator driver.
- GCC 12.
- Ability to coredump from GDB directly.
- JLink support.
- Support for STM32U5 devices.
- Repetition Counter on STM32 Advanced Control Timers.
- ADC scan mode, DMA mode and external trigger for STM32F1/F2/F37/F4/F7/L1.
- SAMx7x I2C driver.

Integrated Projects:

- ETL upgraded to v20.35.14.

Fixes:

- Multiple fixes for I²C NACK handling on STM32.
- On Nucleo-F103RB GpioB4 is now usable.
- Systick frequency on SAMx7x.
- Fix static member variable initialization.

New development boards:

- Nucleo-U575ZI-Q as [`modm:nucleo-u575zi-q`][].
- SAME70 Xplained as [`modm:board:same70-xplained`][].

New device drivers:

- LSM6DSO imu as [`modm:driver:lsm6dso`][].
- MS5837 pressure/temperature sensor as [`modm:driver:ms5837`][].
- AT24MAC402 I2C Eeprom as [`modm:driver:at24mac402`][].

Known bugs:

- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Christopher Durand ([@chris-durand][])
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])
- Victor Costa ([@victorandrehc][]) 🎉
- Sascha Schade ([@strongly-typed][])
- Vivien Henry ([@lukh][])
- Luiz Gili ([@lgili][]) 🎉
- Mattis Kieffer ([@mat-kie][]) 🎉
- Rasmus Kleist ([@rasmuskleist][])
- Sebastian Birke ([@se-bi][])

PR [#995][] -> [2023q1][].

<details>
<summary>Detailed changelog</summary>

#### 2023-04-08: Fix static member variable initialization

Fixes `__cxa_guard_acquire` to construct a static member variable only once!

PR [#995][] -> 2023q1.  
Tested in hardware by [@salkinium][].

#### 2023-04-06: Add SAMx7x I2C driver

Including an unittest and examples.

PR [#954][] -> [d1938eb][].  
Tested in hardware by [@chris-durand][].

#### 2023-04-06: Add AT24MAC402 I2C EEPROM driver

This also adds generic support for I2C eeproms with 8 bit addressing.

PR [#954][] -> [d1938eb][].  
Tested in hardware by [@chris-durand][].

#### 2023-04-01: Add ADC DMA mode and external trigger for STM32F1/F2/F37/F4/F7/L1

PR [#982][] -> [04ed0a5][].  
Tested in hardware by [@victorandrehc][].

#### 2023-03-27: Fix systick frequency on SAMx7x

PR [#986][] -> [589aea7][].  
Tested in hardware by [@chris-durand][].

#### 2023-03-19: Add support for Repetition Counter on STM32 Advanced Control Timers

PR [#981][] -> [740fd51][].  
Tested in hardware by [@victorandrehc][].

#### 2023-03-14: Add BSP for SAM E70 Xplained with multiple examples

PR [#969][] -> [a38feca][].  
Tested in hardware by [@lgili][].

#### 2023-03-14: Separate the rx handling part of the XPCC dispatcher

PR [#970][] -> [2273bae][].  
Tested in hardware by [@se-bi][].

#### 2023-03-13: Add STM32U5 support

Support for all devices of the new STM32U5 family with Cortex-M33 core.
UART and RCC platform drivers are adapted and a BSP for the Nucleo-U575ZI-Q
is also included.

PR [#843][] -> [8a3a20b][].  
Tested in hardware by [@rleh][].

#### 2023-03-13: Add ADC scan mode supprot for STM32F1/F2/F37/F4/F7/L1

PR [#976][] -> [08cd479][].  
Tested in hardware by [@victorandrehc][].

#### 2023-03-06: Refactor tooling and add JLink support

- Refactor modm_tools module and debug tooling
- Add JLink debug probe support
- Add ITM and RTT on NUCLEO-64 board

PR [#965][] -> [241b0d1][].  
Tested in hardware by [@salkinium][].

#### 2023-03-06: Enable GpioB4 on Nucleo-F103RB

PR [#967][] -> [416ced6][].  
Tested in hardware by [@strongly-typed][].

#### 2023-02-28: Always inline DMA IRQ handler to reduce IRQ latency on STM32

PR [#964][] -> [d982a85][].  
Tested in hardware by [@strongly-typed][].

#### 2023-02-20: Add ability to coredump from GDB directly

PR [#961][] -> [eb2748e][].  
Tested in hardware by [@salkinium][].

#### 2023-02-18: GCC 12 update

- Adapt code and build system to GCC12
- Split AVR Mega Pro unittests into three
- Fix function call in ADC examples

PR [#940][] -> [923f9c1][].  
Tested in hardware by [@salkinium][].

#### 2023-02-15: Add Nucleo-H723ZG OpenOCD config

PR [#960][] -> [3cee015][].  
Tested in hardware by [@mat-kie][] and [@rleh][].

#### 2023-02-07: Add support for PWM generator on SAMx7x

PR [#957][] -> [190bc78][].  
Tested in hardware by [@chris-durand][].

#### 2023-02-06: Add support for timer/counter on SAMx7x

PR [#956][] -> [95713ee][].  
Tested in hardware by [@chris-durand][].

#### 2023-02-05: Add MS5837 pressure/temperature sensor driver

PR [#942][] -> [8179e6b][].  
Tested in hardware by [@lukh][].

#### 2023-02-04: Add support for SoftwareGpioPort on SAM

Also adds support for inverted pins in GpioSet.

PR [#952][] -> [afdb5ba][].  
Tested in hardware by [@chris-durand][].

#### 2023-02-04: Add CANFD large frame support

PR [#882][] -> [e4b1a4a][].  
Tested in hardware by [@rasmuskleist][] / [@twast92][].

#### 2023-02-04: Multiple fixes for I²C on STM32

- Fix stm32-extended I2C driver NACK handling
- Start next I2C transaction also when previous failed with NACK
- Do not set invalid I2C_CR1_NOSTRETCH bit in master mode

PR [#947][] -> [ba61a34][] and PR [#951][] -> [c347f00][].  
Tested in hardware by [@chris-durand][].

#### 2023-02-01: Add ST LSM6DSO imu driver

PR [#950][] -> [13ea578][].  
Tested in hardware by [@rleh][].

#### 2023-01-16: Add PLL disable function on STM32

PR [#944][] -> [0f0505f][].  
Tested in hardware by [@chris-durand][].

#### 2023-01-12: Add support for GpioInverted on SAM

PR [#941][] -> [4f50d00][].  
Tested in hardware by [@chris-durand][].

#### 2023-01-12: Add SPI support for SAMx7x

PR [#938][] -> [c93dd2c][].  
Tested in hardware by [@chris-durand][].

#### 2023-01-03: Add external main clock support on SAMx7x

PR [#939][] -> [0259ad2][].  
Tested in hardware by [@chris-durand][].

</details>


## 2023-01-01: 2022q4 release

This release covers everything from 2022-10-01 and has been tested with avr-gcc
v10.2.0 from Upstream and arm-none-eabi-gcc 10.3-2022.07 from Arm.

Breaking changes:

- None.

Features:

- Basic ADC support for SAMx7x devices.

Integrated Projects:

- printf upgraded to v6.1.0.
- CMSIS-DSP upgraded to v1.14.2.
- TinyUSB upgraded to v0.14.0.
- FreeRTOS upgraded to 202212.
- FatFS upgraded to v0.15.
- LVGL upgraded to v8.3.4.
- NanoPB upgraded to v0.4.7.
- ETL upgraded to v20.35.6.
- STM32L5 headers upgraded to v1.12.0.
- STM32H7 headers upgraded to v1.10.3.

Fixes:

- STM32G4 ADC channel mapping.
- Initialize frame buffer memory in MonochromeGraphicDisplay.
- Only configure BDTR register for timers with complementary PWM.
- Fix SAMx7x b-variant USART defines.

New development boards:

- None.

New device drivers:

- None.

Known bugs:

- C++20 is not fully implemented by GCC10 yet, however modm does not use all
  features yet anyways. See [#326][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Christopher Durand ([@chris-durand][])
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])
- Sergey Pluzhnikov ([@ser-plu][]) 🎉

PR [#937][] -> [2022q4][].

<details>
<summary>Detailed changelog</summary>

#### 2022-12-04: Add basic SAMx7x ADC driver

PR [#935][] -> [7330500][].  
Tested in hardware by [@chris-durand][].

</details>


## 2022-10-01: 2022q3 release

This release covers everything from 2022-07-01 and has been tested with avr-gcc
v10.2.0 from Upstream and arm-none-eabi-gcc 10.3-2022.07 from Arm.

Breaking changes:

- `Uart` peripheral renamed to `Usart` on SAMG5 devices for consistency.
- `GCLK` peripheral has been refactored for all SAM devices.

Features:

- RP2040 external flash size option.
- RP2040 ADC driver.
- Resolution configuration for DS18B20 driver.
- Use GPIO signal to configure STM32 Timers.
- Support for SAMx5x and SAMx7x devices.

Integrated Projects:

- printf upgraded to v6.0.0.
- LVGL upgraded to v8.3.2.
- ETL upgraded to v20.34.0.
- Pico-SDK upgraded to v1.4.0.
- STM32G0 headers upgraded to v1.4.2.
- STM32F7 headers upgraded to v1.2.8.

Fixes:

- SpiMaster compilation with GCC11.
- Using RTT with multiple channels.
- Relocation of lbuild outpath in `modm:build:make` module.
- Deadlock RP2040 DMA driver due to counter overflow.
- Fix compilation of moving average for floating-point type.
- SAMD `setInput(InputType)` does not always reset to floating anymore.

New development boards:

- Feather-RP2040 as [`modm:board:feather-rp2040`][].
- ThingPlus-RP2040 as [`modm:board:thingplus-rp2040`][].
- SAMV71 Xplained Ultra as [`modm:board:samv71-xplained-ultra`][].
- SAME54 Xplained Pro as [`modm:board:same54-xplained-pro`][].
- SAMD21 Xplained Pro as [`modm:board:samd21-xplained-pro`][].

New device drivers:

- ADS7828 ADC as [`modm:driver:ads7828`][].
- MCP9902/3/4 temperature sensor as [`modm:driver:mcp990x`][].
- ST7789 display driver as [`modm:driver:st7789`][].

Known bugs:

- C++20 is not fully implemented by GCC10 yet, however modm does not use all
  features yet anyways. See [#326][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Artiom ([@Artiom9][]) 🎉
- Christopher Durand ([@chris-durand][])
- Jonas Kazem Andersen ([@JKazem][])
- Lucas Mösch ([@lmoesch][])
- Niklas Hauser ([@salkinium][])
- Nikolay Semenov ([@cocasema][])
- Raphael Lehmann ([@rleh][])
- Sascha Schade ([@strongly-typed][])
- Steven Macías ([@StevenMacias][])

PR [#917][] -> [2022q3][].

<details>
<summary>Detailed changelog</summary>

#### 2022-09-30: Add SAMx5x/x7x device support

Also adds SAMV71 Xplained Ultra and SAME54 Xplained Pro board support packages
and examples. For consistency with other SAM devices, the SAMG `Uart` peripheral
has been renamed to `Usart` in a breaking change. The `GCLK` peripheral has been
refactored for consistency across and to remove hard-coded design choices.

PR [#827][] -> [8230fef][].  
Tested in hardware by [@chris-durand][] with **high impact** on SAM devices.

#### 2022-08-23: Add ST7789 display driver

PR [#902][] -> [516b2b3][].  
Tested in hardware by [@cocasema][].

#### 2022-08-18: Add ThingPlus-RP2040 BSP

PR [#900][] -> [874c8d6][].  
Tested in hardware by [@cocasema][].

#### 2022-08-18: Add RP2040 ADC driver

PR [#899][] -> [490e868][].  
Tested in hardware by [@cocasema][].

#### 2022-08-17: Add Feather-RP2040 BSP

Also adds an option to set the flash size of the external flash chip.

PR [#898][] -> [cceb801][].  
Tested in hardware by [@cocasema][].

#### 2022-08-09: Add MCP9902/3/4 temperature sensor

PR [#894][] -> [0fd53a8][].  
Tested in hardware by [@chris-durand][].

#### 2022-07-23: Add Nanopb with SCons integration

PR [#657][] -> [6b5b4ce][].  
Tested in hardware by [@lmoesch][].

#### 2022-07-17: Add ADS7828 ADC driver

PR [#887][] -> [0cd2bc9][].  
Tested in hardware by [@JKazem][].

</details>


## 2022-07-01: 2022q2 release

This release covers everything from 2022-04-01 and has been tested with avr-gcc
v11.2.0 from upstream and arm-none-eabi-gcc 10.3-2021.10 from Arm.

Breaking changes:

- None.

Features:

- Custom OpenOCD binary path.
- IRQ handlers for RP2040.
- Expose transmission complete flag on STM32 UART.
- Half transfer handler for STM32 DMA.
- Timer `SlaveModeTrigger::Internal0`/`Internal3` for STM32F4.
- UI Button group refactoring.
- MovingAverage filter refactoring.
- Support for std::chrono in STM32 timer API.

Integrated Projects:

- ETL upgraded to v20.29.3.
- CrashCatcher updated to latest version.

Fixes:

- Fix STM32F7 linker script issue causing memory corruption on some devices.
- STM32H7 RCC clock output.
- CAN system clock frequencies for Nucleo-F446RE/Nucleo-F446ZE boards.
- Cortex-M ITM when `buffer.tx` option not set.
- `Rcc::disable()` on STM32 disabling unrelated clocks.
- Fix `GpioSampler` difference computation.
- Fix missing DMAable capability for DTCM RAM on STM32F7.

New development boards:

- Support for revision B-02 of the DISCO-F469NI development board as
  `modm:disco-f469ni:b-02`.

New device drivers:

- MAX31855 thermocouple-to-digital converter as `modm:driver:max31855`
- ADS101X ADC as `modm:driver:ads101x`
- MS5611 pressure sensor as `modm:driver:ms5611`

Known bugs:

- Cannot set breakpoints on specific lines in debug profile. See [#777][].
- C++20 is not fully implemented by GCC10 yet, however modm does not use all
  features yet anyways. See [#326][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special thank you to our first timers 🎉:

- Christopher Durand ([@chris-durand][])
- Jonas Kazem Andersen ([@JKazem][]) 🎉
- Niklas Hauser ([@salkinium][])
- Nikolay Semenov ([@cocasema][]) 🎉
- Raphael Lehmann ([@rleh][])
- Rasmus Kleist ([@rasmuskleist][])
- Sarah Vilete ([@sarahvilete][]) 🎉
- Sascha Schade ([@strongly-typed][])
- Sebastian Birke ([@se-bi][])
- Sebastian Tibor Bakonyvari ([@twast92][]) 🎉
- Thomas Rush ([@tarush53][]) 🎉
- Thomas Sommer ([@TomSaw][])
- Vivien Henry ([@lukh][])
- xgzeng 🎉

PR [#875][] -> [2022q2][].

<details>
<summary>Detailed changelog</summary>

#### 2022-06-31: Add support for std::chrono in STM32 timer API

PR [#873][] -> [c949daf][].

#### 2022-06-08: MovingAverage filter refactoring

And `reset()` function added.

PR [#872][] -> [7b5827f][].  
Tested by [@TomSaw][].

#### 2022-05-31: Fix Cortex-M ITM

ITM was previously not working when buffer.tx option was not set.

PR [#866][] -> [90774be][].  
Tested in hardware by [@xgzeng][].

#### 2022-05-21: UI Button group refactoring

PR [#864][] -> [55d5911][].  
Tested by [@TomSaw][].

#### 2022-05-11: Add support for revision b-02 of DISCO-F469NI development board

PR [#862][] -> [4885c53][].  
Tested in hardware by [@se-bi][], [@rleh][] and [@salkinium][].

#### 2022-05-08: Fix missing CAN system clock frequencies for Nucleo-F446RE/Nucleo-F446ZE boards

PR [#861][] -> [87b6405][].  
Tested in hardware by [@strongly-typed][].

#### 2022-05-07: Add Timer `SlaveModeTrigger::Internal0`/`Internal3` for STM32F4

PR [#859][] -> [c7bd876][].

#### 2022-05-04: Add half transfer handler for STM32 DMA

PR [#858][] -> [dab6c79][].

#### 2022-05-03: Add MS5611 pressure sensor driver

PR [#851][] -> [ab9bcee][].  
Tested in hardware by [@rasmuskleist][].

#### 2022-05-01: Expose transmission complete flag on STM32 UART

PR [#856][] -> [0e3d280][].  
Tested in hardware by [@strongly-typed][].

#### 2022-05-01: Add ADS101X ADC driver

PR [#854][] -> [596eafa][].  
Tested in hardware by [@JKazem][].

#### 2022-04-30: Fix STM32H7 RCC clock output

PR [#853][] -> [6e7c12f][].  
Tested in hardware by [@chris-durand][].

#### 2022-04-22: Add MAX31855 thermocouple-to-digital converter driver

PR [#850][] -> [2e34b11][].  
Tested in hardware by [@rasmuskleist][] / [@sarahvilete][].

#### 2022-04-18: Implement IRQ handlers for RP2040

PR [#848][] -> [599e0ba][].  
Tested in hardware by [@cocasema][].

#### 2022-04-13: Add support for custom OpenOCD binary path

PR [#846][] -> [98a2483][].  
Tested in hardware by [@cocasema][].

</details>


## 2022-04-01: 2022q1 release

This release covers everything from 2022-01-01 and has been tested with avr-gcc
v11.2.0 from upstream and arm-none-eabi-gcc 10.3-2021.10 from Arm.

Breaking changes:

- Removed all functions marked deprecated for >1 year.

Features:

- STM32L5 support.
- Raspberry Pi PR2040 support.
- Microchip SAM timer driver.
- Improved project and Doxygen documentation.
- STM32G4 internal flash driver.
- TinyUSB on STM32G4.

Integrated Projects:

- ETL upgraded to v20.27.1.
- TinyUSB upgraded to v0.13.0.
- LVGL upgraded to v8.2.0.

Fixes:

- Issues when using TinyUSB and FreeRTOS together solved.
- Fix TinyUSB crash when reading device descriptor.
- TinyUSB correctly chooses fullspeed port on STM32 by default.
- Fix SPI bug in handling of CPHA/CPOL on Microchip SAM.
- Read unique ID correctly on SAMG55.
- Fix GPIO peripheral connect method on Microchip SAM.
- Heap allocator defaults to Newlib on devices with one continuous RAM section.
- FreeRTOS now always supports static allocation.

New development boards:

- Support for newer revision (B-03) of the DISCO-F469NI development board
  as [`modm:disco-f469ni:b-03`][].
- Nucleo-L552ZE-Q as [`modm:board:nucleo-l552ze-q`][].
- RP-PICO as [`modm:board:rp-pico`][].

New device drivers:

- ADIS16470 inertial measurement unit driver as [`modm:driver:adis16470`][].

Known bugs:

- Cannot set breakpoints on specific lines in debug profile. See [#777][].
- C++20 is not fully implemented by GCC10 yet, however modm does not use all
  features yet anyways. See [#326][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special thank you to our first timers 🎉:

- Andrey Kunitsyn ([@andryblack][])
- Christopher Durand ([@chris-durand][])
- Jeff McBride ([@mcbridejc][])
- Kaelin Laundry ([@WasabiFan][])
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])
- Rasmus Kleist ([@rasmuskleist][]) 🎉
- Steven Macías ([@StevenMacias][])
- Thomas Sommer ([@TomSaw][])

PR [#842][] -> [2022q1][].

<details>
<summary>Detailed changelog</summary>

#### 2022-03-17: Enable flash driver for STM32G4

PR [#838][] -> [f7dff92][].  
Tested in hardware by [@rasmuskleist][].

#### 2022-02-28: I²C driver for RP2040

PR [#830][] -> [21ba120][].  
Tested in hardware by [@andryblack][] and [@rleh][].

#### 2022-02-14: Add configuration version for DISCO-F469NI

PR [#819][] -> [b18385c][].

#### 2022-02-09: Add driver for SAMG timer channels

PR [#823][] -> [f5cdf6a][].  
Tested in hardware by [@mcbridejc][].

#### 2022-02-09: Fix GPIO peripheral connect method on Atmel SAM

PR [#822][] -> [c868f59][].  
Tested in hardware by [@mcbridejc][].

#### 2022-02-08: Read unique ID correctly for SAMG55

PR [#824][] -> [845840e][].  
Tested in hardware by [@mcbridejc][].

#### 2022-02-08: Fix SPI bug in handling of CPHA/CPOL

PR [#821][] -> [ac46099][].  
Tested in hardware by [@mcbridejc][].

#### 2022-01-31: Fix issues when using TinyUSB and FreeRTOS together

PR [#816][] -> [3f3ff3d][].  
Tested in hardware by [@rleh][].

#### 2022-01-24: Add ADIS16470 inertial measurement unit driver

PR [#805][] -> [b153186][].  
Tested in hardware by [@rleh][].

#### 2022-01-24: Add Raspberry Pi RP2040 support

Support for the Raspberry Pi microcontroller RP2040 with two ARM Cortex M0+
cores.
This includes support platform drivers for clock, DMA, GPIO, UART, SPI,
USB (TinyUSB) and a board support package for the Raspberry Pi _Pico_
development board.

PR [#806][] -> [1c9c0b6][].  
Tested in hardware by [@andryblack][], [@rleh][] and [@salkinium][].

#### 2022-01-19: Add STM32L5 support

Support for all devices of the new STM32L5 family with Cortex-M33 core.
ADC, DMA, FreeRTOS and RCC platform drivers are adapted and a BSP for the
Nucleo-L552ZE-Q is also included.

PR [#800][] -> [187ddd8][].  
Tested in hardware by [@chris-durand][].

</details>


## 2022-01-01: 2021q4 release

This release covers everything from 2021-10-01 and has been tested with avr-gcc
v10.2.0 from Upstream and arm-none-eabi-gcc 10.3-2021.10 from Arm.

Breaking changes:

- `modm:driver:encoder_input.bitbang` has changed its API.
- `modm::Saturated` class has been completely refactored.

Features:

- STM32F1 internal flash driver.
- Use FreeRTOS with static memory by default.
- SAMG UART driver.
- SAMG ADC driver.
- STM32F4 Ethernet support via FreeRTOS+TCP.
- STM32F4 and STM32F7 DMA support.
- VSCode IDE support via `modm:ide:vscode`.
- Experimental support for fibers.
- Vector table remap into SRAM on Cortex-M0+ and STM32F0.
- STM32H7 DMA driver.

Integrated Projects:

- ETL added at v20.22.0.
- TinyUSB upgraded to v0.12.0.
- FreeRTOS upgraded to 202112.00.
- LVGL upgraded to v8.1.0.

Fixes:

- Remove heap dependency for static virtual destructors.
- Correct STM32G4/L4+ voltage scaling and boost mode.
- Move main stack to DMA-able memory for STM32F4.
- Linkerscript alignment issues for copy sections.
- Zero SRAM after uploading to prevent false positive RTT identifier.
- Off-by-One error in EXTI IRQ handler.
- DMA data fixes for STM32G0, L4, H7 and WL.
- Do not reset STM32 Advanced Timer RCR during configuration.

New development boards:

- NUCLEO-L053R8 as [`modm:board:nucleo-l053r8`][].

New device drivers:

- TMP12{1,3,5} temperature sensor as [`modm:driver:tmp12x`][].
- Encoder Input via STM32 Timer as [`modm:driver:encoder_input`][].
- MCP7941X RTC driver with EUI-48/64 and EEPROM as [`modm:driver:mcp7941x`][].
- ADS816X ADC diver as [`modm:driver:ads816x`][].

Known bugs:

- Cannot set breakpoints on specific lines in debug profile. See [#777][].
- C++20 is not fully implemented by GCC10 yet, however modm does not use all
  features yet anyways. See [#326][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special thank you to our first timers 🎉:

- Andrey Kunitsyn ([@andryblack][]) 🎉
- Christopher Durand ([@chris-durand][])
- Jacob Schultz Andersen ([@jasa][])
- Jeff McBride ([@mcbridejc][])
- Kaelin Laundry ([@WasabiFan][])
- Niklas Hauser ([@salkinium][])
- Odin Holmes ([@odinthenerd][]) 🎉
- Pavel Pletenev ([@ASMfreaK][])
- Raphael Lehmann ([@rleh][])
- Sebastian Birke ([@se-bi][])
- Steven Macías ([@StevenMacias][]) 🎉
- Thomas Sommer ([@TomSaw][])

PR [#797][] -> [2021q4][].

<details>
<summary>Detailed changelog</summary>

#### 2021-12-31: Add ADS816X ADC driver

PR [#796][] -> [3ba71c9][].  
Tested in hardware by [@rleh][].

#### 2021-12-31: Add STM32H7 DMA driver

PR [#772][] -> [e3c0321][].  
Tested in hardware by [@chris-durand][].

#### 2021-12-19: Add MCP7941X RTC driver

PR [#748][] -> [5a9ad25][].  
Tested in hardware by [@rleh][].

#### 2021-12-14: Refactor modm::Saturated

PR [#780][] -> [c0a8c51][].  
Tested in hardware by [@TomSaw][] with **high impact** on API.

#### 2021-11-18: Add vector table remap on Cortex-M0+ and STM32F0

PR [#773][] -> [fdbb45b][].  
Tested in hardware by [@chris-durand][].

#### 2021-11-15: Add encoder input driver via STM32 timers

PR [#775][] -> [d46c09d][].  
Tested in hardware by [@TomSaw][].

#### 2021-10-29: Add basic support for fibers

You must enable the hidden repository option `modm:__fibers` to use them.

PR [#743][] -> [3936a28][].  
Tested in hardware by [@salkinium][].

#### 2021-10-29: Add VSCode IDE integration

PR [#764][] -> [6b4d656][].  
Tested by [@rleh][] and [@salkinium][].

#### 2021-10-28: Add DMA driver for STM32F4 and STM32F7

PR [#629][] -> [5dcdf1d][].  
Tested in hardware by [@rleh][].

#### 2021-10-25: Fix linkerscript alignment issues

This fixes copying data sections with the wrong alignment.

PR [#763][] -> [b78acd5][].  
Tested in hardware by [@salkinium][] with **high impact** on `.fastcode` section.

#### 2021-10-23: Add SAMG ADC driver

PR [#753][] -> [82bc4a9][].  
Tested in hardware by [@mcbridejc][].

#### 2021-10-22: Add SAMG UART driver

PR [#761][] -> [6e9f000][].  
Tested in hardware by [@mcbridejc][].

#### 2021-10-21: Add NUCLEO-L053R8 board and example

PR [#756][] -> [0d6a937][].  
Tested in hardware by [@salkinium][].

#### 2021-10-21: Add TMP12x temperature sensor driver

PR [#757][] -> [624ce10][].  
Tested in hardware by [@chris-durand][].

#### 2021-10-19: FreeRTOS with static memory

Adds static allocation support when used without `modm:platform:heap` module.

PR [#750][] -> [9e50a16][].  
Tested in hardware by [@andryblack][] and [@salkinium][].

#### 2021-10-14: Weak operator delete

Static virtual C++ destructors can emit operator delete, this marks the
operators as weak which prevents pulling in Newlib.

PR [#747][] -> [387a625][].  
Tested in hardware by [@delphi][] and [@salkinium][].

#### 2021-10-12: Add ETL module

PR [#667][] -> [2ef7a29][].  
Tested in hardware by [@rleh][] and [@salkinium][].

#### 2021-10-10: Add STM32F1 flash driver

PR [#745][] -> [f4c7492][].  
Tested in hardware by [@delphi][].

</details>


## 2021-10-01: 2021q3 release

This release covers everything from 2021-07-01 and has been tested with avr-gcc
v10.2.0 from Upstream and arm-none-eabi-gcc 10.3-2021.07 from Arm.

Breaking changes:

- STM32 EXTI configuration moved into separate `modm:platform:exti` module.
- STM32F3 ADC flags API made consistent with rest of ADC implementations.
- AVR SPI mode 1/2 was swapped.
- Interface SPI mode 3/4 was swapped.
- Configuration of CMSIS-DSP module is now done via `<arm_math_local.h>` file.

Features:

- STM32H7 support.
- SAMG55 support with USB and SPI drivers.
- SAMV70 support.
- STM32F1 ADC calibration.
- Adding silicon revision to `modm:target` option with `/revN` suffix.
- `modm_initialize_platform()` hook called right after startup.
- Strict IRQ name validation: Never wonder again why your IRQ isn't called!
- Much faster lbuild generation of `modm:platform:gpio` due to fewer files.
- New `modm:platform:exti` module for STM32 with optional IRQ handlers.

Integrated Projects:

- LVGL upgraded to v8.0.2.
- TinyUSB upgraded to v0.11.0.
- FreeRTOS upgraded to 202107.
- CMSIS-DSP upgraded to v5.8.0.

Fixes:

- Longer modm::delay_ms implementation.
- Lower C++ binary size due to not using `atexit` function.
- SPI mode 3/4 was swapped in interface, mode 1/2 swapped on AVR.
- Enable FPU in assembly before startup script.
- Fix inconsistent flags API for STM32F3 ADC.
- Refactored GpioConnector to remove `template< Peripheral _ >` signal
  boilerplate.

New development boards:

- NUCLEO-H743ZI as [`modm:board:nucleo-h743zi`][].
- NUCLEO-H723ZG as [`modm:board:nucleo-h723zg`][].
- Smart Response XE as [`modm:board:srxe`][].
- SAM G55 Xplained Pro as [`modm:board:samg55-xplained-pro`][].
- DevEBox STM32H750VB as [`modm:board:devebox-stm32h750vb`][].

New device drivers:

- ST7586S display as [`modm:driver:st7586s`][].

Known bugs:

- C++20 is not fully implemented by GCC10 yet, however modm does not use all
  features yet anyways. See [#326][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Christopher Durand ([@chris-durand][])
- Daniel Krebs ([@daniel-k][])
- Henrik Hose ([@hshose][]) 🎉
- Jakob Riepler ([@XDjackieXD][]) 🎉
- Jeff McBride ([@mcbridejc][])
- Niklas Hauser ([@salkinium][])
- Thomas Sommer ([@TomSaw][])
- Tomasz Wasilczyk ([@twasilczyk][]) 🎉
- Valeriy Osipov ([@SgtPepperFTW][]) 🎉

PR [#742][] -> [2021q3][].

<details>
<summary>Detailed changelog</summary>

#### 2021-09-30: Upgrade CMSIS-DSP to v5.8.0

The `:cmsis:dsp` module options were removed in favor of letting the user
define them freely using a `<arm_math_local.h>` config file.

PR [#742][] -> [12bb41b][].  
Tested in hardware by [@salkinium][] with **medium impact** on config.

#### 2021-09-29: Refactor GPIO implementations

New implementation generates fewer files, which speeds up `lbuild build`
significantly for devices with many pins. EXTI implementation was moved into
its own module with an optional IRQ handler abstraction. Refactored signals
implementation to remove dragging around `template< Peripheral _ >` boilerplate.

PR [#686][] -> [30e24e6][].  
Tested in hardware by [@salkinium][] with **high impact** on EXTI code.

#### 2021-09-22: Add SAMV70 support

On custom hardware.

PR [#681][] -> [9036666][].  
Tested in hardware by [@twasilczyk][].

#### 2021-09-17: Strict IRQ name validation

Validates `MODM_ISR(name)` at compile time to you never declare the wrong
IRQ that won't get called.

PR [#685][] -> [6057873][].  
Tested in hardware by [@salkinium][] with low impact on `MODM_ISR(name)`.

#### 2021-09-16: Add DevEBox STM32H750VB board and example

PR [#678][] -> [b4eddeb][].  
Tested in hardware by [@hshose][].

#### 2021-09-10: Add SAMG55 SPI driver

PR [#680][] -> [f4d5d6c][].  
Tested in hardware by [@mcbridejc][].

#### 2021-09-09: Add SAMG55 USB driver via TinyUSB

PR [#679][] -> [93bba13][].  
Tested in hardware by [@mcbridejc][].

#### 2021-09-09: Add SAMG55 support

Also adds SAM G55 Xplained Pro board and example.

PR [#676][] -> [c148bf8][].  
Tested in hardware by [@mcbridejc][].

#### 2021-09-07: Add ST7586S display driver

PR [#673][] -> [2c22fae][].  
Tested in hardware by [@twasilczyk][].

#### 2021-08-23: Smart Response XE board and example

Adds support for the ATmega128RFA1 and the Smart Response XE board.

PR [#669][] -> [a173bde][].  
Tested in hardware by [@twasilczyk][].

#### 2021-07-15: Add STM32H7 support

Also adds NUCLEO-H743ZI and NUCLEO-H723ZG boards and examples.

PR [#652][] -> [80ed738][].  
Tested in hardware [@chris-durand][] and [@salkinium][].

</details>


## 2021-07-01: 2021q2 release

This release covers everything from 2021-04-01 and has been tested with avr-gcc
v10.1.0 from Upstream and arm-none-eabi-gcc 2020q4 from Arm.

Breaking changes:

- TinyUSB updated from v0.8.0 to v0.10.1 with breaking changes to their APIs.
- FreeRTOS configuration must be done by the user via the headers
  `<FreeRTOSConfigLocal.h>` and `<FreeRTOSIPConfigLocal.h>`.
- `modm::GraphicDisplay` API changed due to change in color management.
- New color types and API.
- New CMake build system has different API without top-level Makefile.

Features:

- STM32L0 support.
- Real-Time Transport (RTT) support via OpenOCD v0.11.
- FDCAN driver for STM32G4 (only for standard messages).
- DMA support for all STM32 devices, except F2/F4/F7.
- Better graphical display and color support.
- New CMake build system for better IDE integration.
- Top-level build scripts relicensed as BSD to allow easier adaptation.
- LPUART driver for STM32.
- DAC driver with DMA support for STM32.
- Non-allocating util classes `modm::inplace_any` and `modm::inplace_function`.

Integrated Projects:

- LVGL added at v8.0.1.
- Petit FatFs added at v0.03a.
- FatFs upgraded to v0.14b.
- TinyUSB upgraded to v0.10.1.
- FreeRTOS upgraded to 202012 LTS.

Fixes:

- OpenOCD ITM configuration for logging.
- Much more accurate nanosecond delay.
- realloc with NULL pointer calls malloc when using TLSF allocator.

New development boards:

- NUCLEO-L031K6 as [`modm:board:nucleo-l031k6`][].
- NUCLEO-F072RB as [`modm:board:nucleo-f072rb`][].
- NUCLEO-L496ZG-P as [`modm:board:nucleo-l496zg-p`][].
- NUCLEO-F091RC as [`modm:board:nucleo-f091rc`][].

New device drivers:

- SH1106 display as [`modm:driver:sh1106`][].
- STTS22H thermometer as [`modm:driver:stts22h`][].

Known bugs:

- C++20 is not fully implemented by GCC10 yet, however modm does not use all
  features yet anyways. See [#326][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors:

- Christopher Durand ([@chris-durand][])
- Jacob Schultz Andersen ([@jasa][])
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])
- Sascha Schade ([@strongly-typed][])
- Thomas Sommer ([@TomSaw][])

PR [#597][] -> [2021q2][].

<details>
<summary>Detailed changelog</summary>

#### 2021-06-16: More accurate modm::delay implementation

PR [#642][] -> [cc15b1a][].  
Tested in hardware by [@salkinium][].

#### 2021-06-07: Extend DMA driver for even more STM32 devices

PR [#632][] -> [8896b5b][].  
Tested in hardware by [@chris-durand][].

#### 2021-06-06: Add DAC driver with DMA support

PR [#632][] -> [9cbea26][].  
Tested in hardware by [@chris-durand][].

#### 2021-05-20: Refactor color implementation

PR [#616][] -> [a105072][] with **high impact** on color types.  
Tested by [@TomSaw][].

#### 2021-05-10: Add LPUART driver for STM32

PR [#614][] -> [897579e][].  
Tested in hardware by [@rleh][].

#### 2021-05-08: Add STTS22H temperature driver

PR [#544][] -> [9e7ec34][].  
Tested in hardware by [@chris-durand][].

#### 2021-05-08: New CMake build system

PR [#568][] -> [98b1337][] with **high impact** on CMake build systems.  
Tested by [@jasa][] and [@salkinium][].

#### 2021-05-07: Add SH1106 display driver

PR [#627][] -> [7d7490d][].  
Tested in hardware by [@TomSaw][].

#### 2021-04-22: Specialize modm::GraphicDisplay for monochrome and color

To enable optimizations for different color formats, the graphic display base
classes are refactored into monochrome and color classes.

PR [#604][] -> [295dbc3][] with medium impact on display drivers and graphics code.  
Tested in hardware by [@TomSaw][].

#### 2021-04-14: Adapt DMA driver for more STM32 devices

PR [#608][] -> [8ca2f35][].  
Tested in hardware by [@rleh][].

#### 2021-04-13: Add FDCAN driver for STM32G4

PR [#607][] -> [2d2199b][].  
Tested in hardware by [@chris-durand][].

#### 2021-04-12: Add RTT support via OpenOCD

PR [#610][] -> [1375ff1][].  
Tested in hardware by [@salkinium][].

#### 2021-04-11: Upgrade FreeRTOS to 202012 LTS

The default configuration modm provides changed, and now only provides the bare
minimum config and the remainder needs to be user-provided via the headers
`<FreeRTOSConfigLocal.h>` and `<FreeRTOSIPConfigLocal.h>`

PR [#566][] -> [1375ff1][] with **high impact** on configuration management.  
Tested in hardware by [@salkinium][].

#### 2021-04-06: Integrate Petit FatFs v0.03a and upgrade FatFs to v0.14a

PR [#566][] -> [1f5d06e][].  
Tested in hardware by [@salkinium][].

#### 2021-04-06: Upgrade TinyUSB to v0.9.0

TinyUSB is breaking their own API quite often, so existing applications must be
adapted. Any modm wrapper APIs are unaffected. See module docs for additional
customization via the `<tusb_config_local.h>` header.

PR [#566][] -> [1f5d06e][] with **high impact** on TinyUSB API.  
Tested in hardware by [@salkinium][].

#### 2021-04-06: Integrate LVGL v7.11 for Cortex-M

PR [#603][] -> [e0d1327][].  
Tested in hardware by [@rleh][] and [@salkinium][].

#### 2021-04-02: Add STM32L0 support

Also adds NUCLEO-L031K6 board and example.

PR [#600][] -> [0cf1c65][].  
Tested in hardware [@chris-durand][].

</details>


## 2021-04-01: 2021q1 release

This release covers everything from 2021-01-01 and has been tested with avr-gcc
v10.1.0 from Upstream and arm-none-eabi-gcc 2020q4 from Arm.

Breaking changes:

- `Uart::initialize()` arguments and behaviour changed on STM32.
- Complete rewrite of TCS34xx color sensor driver.

Features:

- Update AVR libstdc++ to C++20.
- Enable C++20 with GCC10.
- Ethernet support via FreeRTOS-TCP and LAN8720a.
- STM32: optional UART buffers per direction.
- Using relative paths in ELF files to easier archiving.
- Dark mode for Doxypress documentation.
- Makefile build system generator.
- Better Windows installation instructions.

Fixes:

- STM32F4/F7: Overdrive mode.
- STM32G4: TIM2 and TIM5 as 32-bit timer.
- STM32F0: ADC temperature calibration.
- STM32G4: ADC clock.
- SAMD21 Mini BSP fixes.
- Windows: Uploading via AvrDude.

New development boards:

- NUCLEO-F439ZI as [`modm:board:nucleo-f439zi`][].
- NUCLEO-F446ZE as [`modm:board:nucleo-f446ze`][].
- NUCLEO-F767ZI as [`modm:board:nucleo-f767zi`][].
- NUCLEO-L452RE as [`modm:board:nucleo-l452re`][].

New device drivers:

- MMC5603 compass as [`modm:driver:mmc5603`][].
- LAN8720a ethernet transceiver as [`modm:driver:lan8720a`][].
- STUSB4500 USB-PD controller as [`modm:driver:stusb4500`][].
- Bit-banged encoder input as [`modm:driver:encoder_input.bitbang`][].
- Touch2046/TSC2046 touchscreen controller as [`modm:driver:touch2046`][].

Known bugs:

- C++20 is not fully implemented by GCC10 yet, however modm does not use all
  features yet anyways. See [#326][].
- Nanosecond delay may be inaccurate. See [#495][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].
- `arm-none-eabi-gdb` TUI and GDBGUI interfaces are not supported on Windows.
  See [#591][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Cerem Cem ASLAN ([@ceremcem][]) 🎉
- Christopher Durand ([@chris-durand][])
- David Hebbeker ([@dhebbeker][])
- Jacob Schultz Andersen ([@jasa][]) 🎉
- Mike Wolfram ([@mikewolfram][])
- Nicolai Bruhn ([@nicoBruhn][]) 🎉
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])
- Sascha Schade ([@strongly-typed][])
- Thomas Sommer ([@TomSaw][]) 🎉
- Vivien Henry ([@lukh][])

PR [#597][] -> [2021q1][].

<details>
<summary>Detailed changelog</summary>

#### 2021-03-31: Add Touch2046/TSC2046 touchscreen controller

Also adds NUCLEO-L452RE board and example.

PR [#600][] -> [b1e5588][].  
Tested by [@rleh][].

#### 2021-03-26: Add Makefile build system generator

PR [#557][] -> [df47974][].  
Tested by [@salkinium][].

#### 2021-03-22: Add Bit-banged encoder input driver

PR [#580][] -> [df47974][].  
Tested in hardware by [@TomSaw][].

#### 2021-03-16: Rewrite TCS34xx color sensors

Breaking changes: Entire API was rewritten.

PR [#578][] -> [850b554][] with **high impact** on TCS34xx driver.  
Tested in hardware by [@TomSaw][].

#### 2021-03-06: Add STUSB4500 USB-PD controller driver

PR [#574][] -> [1a11b08][].  
Tested in hardware by [@rleh][].

#### 2021-03-06: Add NUCLEO-F439ZI board and example

PR [#571][] -> [165adf0][].  
Tested in hardware by [@rleh][].

#### 2021-02-26: Refactor SCons build system

Adds relative paths in ELF file.

PR [#558][] -> [1fc3805][].  
Tested by [@salkinium][].

#### 2021-02-23: Refactor UART HAL

Makes UART buffers individually optional by setting their size to zero.
Breaking changes:

- `Uart::initialize(IrqPriority, Parity)` -> `Uart::initialize(Parity, WordLength)`.
- Word length is not automatically set to 9-bits when setting parity anymore.
- `UartHal::setLastBitClockPulse(LastBitClockPulse)` removed.
- `UartHal::setSpiClock(SpiClock)` -> `UartHal::setSpiClock(SpiClock, LastBitClockPulse)`.
- You must now manually call `UartHal::{en,dis}ableOperation()` before modifying UartHal.

PR [#564][] -> [62b63f5][] with **high impact** on STM32 targets.  
Tested in hardware by [@salkinium][].

#### 2021-02-18: Add Ethernet support via FreeRTOS-TCP and LAN8720a

Also adds NUCLEO-F767ZI board.

PR [#466][] -> [d8be0a2][].  
Tested in hardware by [@mikewolfram][].

#### 2021-01-14: Add MMC5603 compass driver

PR [#556][] -> [fb2ff58][].  
Tested in hardware by [@salkinium][].

#### 2021-01-14: Fix STM32F0 ADC temperature sensor

PR [#526][] -> [3ecad35][] with low impact on STM32F0 targets.  
Tested in hardware by [@chris-durand][].

#### 2021-01-10: Add NUCLEO-F446ZE board and example

PR [#533][] -> [02b1571][].  
Tested in hardware by [@chris-durand][].

#### 2021-01-10: Enable C++20

PR [#521][] -> [8c322a2][] with low impact on all targets.  
Tested in hardware by all.

#### 2021-01-10: Update avr libstdc++ submodule to C++20 version

PR [#535][] -> [038657c][] with low impact on AVR targets.  
Tested in hardware by [@chris-durand][].

</details>


## 2021-01-01: Virtual 2020q4 release

This is a virtual release only for reference that covers everything from
2020-10-01.

Breaking changes:

- SAMD GPIO connector API is rewritten.

Features:

- Added a `.clang-format` for automated code styling.
- Update FatFS from v0.09 to v0.13.

Fixes:

- STM32F3: Fix missing HsiDiv2 in PllSource.
- FreeRTOS: Fix SystemCoreClock linking.
- Proper lookup of all IOStream `operator<<` overloads.

New development boards:

- NUCLEO-F334R8 as [`modm:board:nucleo-f334r8`][].
- NUCLEO-G431KB as [`modm:board:nucleo-g431kb`][].
- NUCLEO-G431RB as [`modm:board:nucleo-g431rb`][].

No new device drivers.

Known bugs:

- Nanosecond delay may be inaccurate. See [#495][].
- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].

Many thanks to all our contributors:

- Christopher Durand ([@chris-durand][])
- Erik Henriksson ([@henrikssn][])
- Jeff McBride ([@mcbridejc][])
- Mike Wolfram ([@mikewolfram][])
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])
- Sascha Schade ([@strongly-typed][])

<details>
<summary>Detailed changelog</summary>

#### 2020-12-16: Add NUCLEO-G431RB board and example

PR [#490][] -> [195f7e1][].  
Tested in hardware by [@rleh][].

#### 2020-11-16: Add NUCLEO-G431KB board and example

PR [#499][] -> [276f5b3][].  
Tested in hardware by [@mcbridejc][].

#### 2020-11-04: Update FatFs to v0.13

PR [#497][] -> [7df2e7d][] with **medium impact** on all targets.  
Tested in hardware by [@salkinium][].

#### 2020-10-07: Rewrite of SAMD GPIO connector API

PR [#482][] -> [dc56af2][] with **high impact** on SAMD targets.  
Tested in hardware by [@henrikssn][].

#### 2020-10-06: Add NUCLEO-F334R8 board and example

PR [#489][] -> [85eec34][].  
Tested in hardware by [@rleh][].

</details>


## 2020-10-01: Virtual 2020q3 release

This is a virtual release only for reference that covers everything from
2020-07-01.

No breaking changes.

Features:

- SAMD21 support with GPIO, UART, External Interrupt.
- A rewritten AMNB protocol.
- RaspberryPi support with GPIO (via Linux/WiringPi).
- Better Windows support incl. CI.
- Compliance with CMSIS SystemCoreClock interface.
- TinyUSB integration!

Fixes:

- STM32F3: Fix `Adc::isConversionFinished()`.
- STM32: Add missing BDTR register to TIM14.
- STM32: Fix RCC clock enable for Ethernet.
- STM32L1: Fix CMSIS Header selection for -A variants.
- STM32F0/G0: Add support for remapped pins.
- FreeRTOS: Make malloc thread-safe.

New development boards:

- NUCLEO-F334R8 as [`modm:board:nucleo-f334r8`][].
- Feather-M0 as [`modm:board:feather-m0`][].
- SAMD21 Mini as [`modm:board:samd21-mini`][].
- Raspberry Pi v3 as [`modm:board:raspberrypi`][].
- DevEBox STM32F4xx as [`modm:board:devebox-stm32f4xx`][].
- STM32_F32VE as [`modm:board:stm32_f32ve`][].

New device drivers:

- LSM6DS33 compass and gyroscope as [`modm:driver:lsm6ds33`][].
- LIS3MDL compass as [`modm:driver:lis3mdl`][].
- GPIO Sampler (STM32-only) as [`modm:driver:gpio-sampler`][].
- IS31FL3733 LED matrix driver as [`modm:driver:is31fl3733`][].
- ILI9341 Parallel/SPI display driver as [`modm:driver:ili9341`][].

Known bugs:

- STM32F7: D-Cache not enabled by default. See [#485][].
- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Benjamin Carrick ([@nesos][])
- Christopher Durand ([@chris-durand][])
- delphi ([@delphi][])
- Erik Henriksson ([@henrikssn][]) 🎉
- Lukas Güldenstein ([@gueldenstone][]) 🎉
- Mike Wolfram ([@mikewolfram][])
- Niklas Hauser ([@salkinium][])
- Pavel Pletenev ([@ASMfreaK][])
- Raphael Lehmann ([@rleh][])
- Sascha Schade ([@strongly-typed][])
- Vivien Henry ([@lukh][])

<details>
<summary>Detailed changelog</summary>

#### 2020-09-30: Integrate TinyUSB for STM32 and SAMD

PR [#478][] -> [3072005][].  
Tested in hardware by [@salkinium][].

#### 2020-09-19: Add ILI9341 Parallel/SPI display driver

PR [#437][] -> [43f32e6][].  
Tested in hardware by [@mikewolfram][] and [@delphi][].

#### 2020-09-18: Update FreeRTOS and make malloc thread-safe

PR [#475][] -> [cb82eec][] with low impact in ARM Cortex-M targets.  
Tested in hardware by [@strongly-typed][] and [@salkinium][].

#### 2020-09-18: Add GPIO remap support for STM32F0/G0

PR [#471][] -> [48d73dc][] with low impact on STM32 targets.  
Tested in hardware by [@chris-durand][] and [@salkinium][].

#### 2020-09-12: Add DevEBox STM32F4xx board and examples

PR [#462][] -> [8082f69][].  
Tested in hardware by [@strongly-typed][].

#### 2020-09-07: Add RaspberryPi support

Wraps `<wiringPi.h>` for GPIO driver.

PR [#444][] -> [d3496a3][].  
Tested in hardware by [@henrikssn][].

#### 2020-08-30: Add FreeRTOS+TCP module

PR [#448][] -> [821677b][].  
Tested in hardware by [@mikewolfram][].

#### 2020-08-15: Add IS31FL3733 LED matrix driver

PR [#445][] -> [9b6aeee][].  
Tested in hardware by [@salkinium][].

#### 2020-07-31: Add GPIO Sampler driver for STM32

PR [#96][] -> [9d8bbfa][].  
Tested in hardware by [@salkinium][].

#### 2020-07-20: Add rewrite of AMNB protocol

PR [#395][] -> [8820d6b][].  
Tested in hardware by [@salkinium][].

#### 2020-07-20: Add SAMD21 External Interrupt driver

PR [#436][] -> [cbce428][].  
Tested in hardware by [@henrikssn][].

#### 2020-07-08: Add SAMD21 basic UART driver

PR [#430][] -> [04688bc][].  
Tested in hardware by [@henrikssn][].

#### 2020-07-07: Add LIS3MDL compass driver

PR [#428][] -> [0994a55][].  
Tested in hardware by [@nesos][].

#### 2020-07-05: Add SAMD21 modm port

Also adds Feather-M0 and SAMD21 Mini BSPs.

PR [#194][] -> [d2d38a0][].  
Tested in hardware by [@salkinium][] and [@henrikssn][].

#### 2020-07-02: Add LSM6DS33 compass and gyroscope driver

PR [#426][] -> [e233708][].  
Tested in hardware by [@nesos][].

</details>


## 2020-07-01: Virtual 2020q2 release

This is a virtual release only for reference that covers everything from
2020-04-01.

Breaking changes:

- Move FCPU option to `modm:platform:core:f_cpu` lbuild option.
- `modm::Clock` reimplemented using `std::chrono`.
- ZeroMQ support removed as ZeroMQ is unmaintained.
- STM32 DMA driver rewritten for only STM32L4.

Features:

- `modm::delay()` is now `std::chrono` compatible.
- `modm::PreciseClock` with microsecond resolution.
- Containers derive from `std::iterator` to work with STL algorithms.
- Simple (stateless) CRC8, CRC16, CRC32 implementations.
- `.ramcode` linker section for Cortex-M.
- Use linkerscript via lbuild option.
- Simple Flash driver for STM32G0/F4.
- Basic DAC driver for STM32F4.

Fixes:

- Fix `<cmath>` in AVR libstdc++.
- Cortex-M: Explicitly set the stack pointer on startup, since some bootloaders
  forget to load the stackpointer from the image.
- TLSF `free()` ignores null pointers.
- STM32F0/F3: Correct Pll Source selection.

New development boards:

- NUCLEO-F303RE as [`modm:board:nucleo-f303re`][].
- Mega 2560 PRO as [`modm:board:mega-2560-pro`][].

New device drivers:

- LP503x LED driver as [`modm:driver:lp503x`][].
- Sx1276 LoRa Modem as [`modm:driver:sx1276`][].
- Bit-bang encoder output as [`modm:driver:encoder.output`][].

Known bugs:

- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Benjamin Carrick ([@nesos][]) 🎉
- Christopher Durand ([@chris-durand][])
- Felix Petriconi ([@FelixPetriconi][]) 🎉
- Jeff McBride ([@mcbridejc][]) 🎉
- Kaelin Laundry ([@WasabiFan][])
- Mike Wolfram ([@mikewolfram][])
- Niklas Hauser ([@salkinium][])
- Pavel Pletenev ([@ASMfreaK][])
- Philipp Graf ([@luxarf][]) 🎉
- Raphael Lehmann ([@rleh][])
- Sascha Schade ([@strongly-typed][])
- Vivien Henry ([@lukh][]) 🎉

<details>
<summary>Detailed changelog</summary>

#### 2020-06-19: Add basic DAC driver for STM32F4

PR [#420][] -> [78d18f6][].  
Tested in hardware by [@mcbridejc][].

#### 2020-06-14: Add bit-banged encoder output

PR [#153][] -> [21af57b][].  
Tested in hardware by [@rleh][].

#### 2020-06-13: Add Sx1276 LoRa Modem driver

PR [#418][] -> [7d1f7cc][].  
Tested in hardware by [@nesos][].

#### 2020-06-11: Add Flash driver for STM32G0/F4

PR [#417][] -> [9e285db][].  
Tested in hardware by [@salkinium][].

#### 2020-05-12: Add LP503x LED driver

PR [#366][] -> [6e5ebf4][].  
Tested in hardware by [@chris-durand][].

#### 2020-05-12: Add NUCLEO-F746ZG board and example

PR [#396][] -> [c38550a][].  
Tested in hardware by [@FelixPetriconi][].

#### 2020-05-12: Add NUCLEO-F746ZG board and example

PR [#396][] -> [c38550a][].  
Tested in hardware by [@FelixPetriconi][].

#### 2020-05-08: Add DMA-capable SPI driver

PR [#400][] -> [80a9c66][] with low impact on STM32 targets.  
Tested in hardware by [@mikewolfram][].

#### 2020-05-06: Rewrite DMA implementation for STM32L4

The previous DMA implementation is replaced by a new STM32L4-only one.

PR [#371][] -> [0217a19][] with **high impact** on STM32 targets.  
Tested in hardware by [@mikewolfram][].

#### 2020-04-25: Add Mega 2560 PRO board and example

PR [#382][] -> [11ffe92][].  
Tested in hardware by [@chris-durand][].

#### 2020-04-16: Add NUCLEO-F303RE board and example

PR [#376][] -> [141aa71][].  
Tested in hardware by [@luxarf][].

#### 2020-04-09: Rewrite modm::Clock and add modm::PreciseClock

Highly efficient implementation via SysTick on Cortex-M and TC0 on AVR.
Breaking changes:

- `modm::Clock` now using `std::chrono` compatible interface.

PR [#217][] -> [c63a536][] with **high impact** on AVR and STM32 targets.  
Tested in hardware by [@salkinium][].

#### 2020-04-06: std::chrono compatible modm::delay

PR [#374][] -> [b010775][] with low impact on all targets.  
Tested in hardware by [@salkinium][].

</details>


## 2020-04-01: Virtual 2020q1 release

This is a virtual release only for reference that covers everything from
2020-01-01.

Breaking changes:

- `modm_assert()` arguments, behavior and assertion handler arguments.
- STM32: `Rcc::enablePll(PllFactors&)` arguments.

Features:

- Doxypress support for API docs generation.
- Support for C11/C++11 atomics on ARMv6-M.
- Make heap module optional with linker failure when accidentally calling malloc.

Fixes:

- I2C NACK handling on STM32.
- Static initialization guard for C++.

No new development boards.

New device drivers:

- CAT24AAxx I2C EEPROM as [`modm:driver:cat24aa`][].

Known bugs:

- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].

Many thanks to all our contributors:

- Christopher Durand ([@chris-durand][])
- David Hebbeker ([@dhebbeker][])
- Mike Wolfram ([@mikewolfram][])
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])
- Sascha Schade ([@strongly-typed][])

<details>
<summary>Detailed changelog</summary>

#### 2020-03-30: Add HX711 scale driver

PR [#372][] -> [d21e991][].  
Tested in hardware by [@strongly-typed][].

#### 2020-03-19: Move heap into own module

`modm:platform:heap` is optional and will cause the linker to fail if malloc is
called without this module.

PR [#364][] -> [b8648be][] with **medium impact** on Cortex-M targets.  
Tested in hardware by [@salkinium][].

#### 2020-03-19: Pass PLL configuration as struct

Breaking changes:

- Single arguments of `Rcc::enable()` now passed as struct.

PR [#361][] -> [e3ba913][] with **high impact** on STM32 targets.  
Tested in hardware by [@rleh][].

#### 2020-03-19: Refactor modm_assert interface and implementation

Breaking changes:

- `modm_assert` signature.
- Additional `modm_assert_*` functions with new behaviors.
- Assertion handler signature and arguments.

PR [#351][] -> [23ec952][] with **high impact** on all targets.  
Tested in hardware by [@salkinium][].

#### 2020-03-13: Implement C11 atomics on ARMv6-M

PR [#346][] -> [5332765][].  
Tested in hardware by [@salkinium][].

#### 2020-03-12: Add C++ RTTI and Exception support

As lbuild options in the `modm:stdc++` module.

PR [#343][] -> [4ff604f][].  
Tested in hardware by [@salkinium][].

#### 2020-01-20: Add CAT24AAxx I2C EEPROM driver

PR [#321][] -> [b721551][].  
Tested in hardware by [@mikewolfram][].

</details>


## 2020-01-01: Virtual 2019q4 release

This is a virtual release only for reference that covers everything from
2019-10-01.

Breaking changes:

- `modm:target` option requires extended identifiers.

Features:

- STM32G4 support!
- DFU programmer support.
- STM32G0: Add ADC driver.

Fixes:

- NUCLEO-F303K8: SystemClock for USART1 and I2C1.
- STM32: I2C NACK interrupt.

New development boards:

- NUCLEO-G474RE as [`modm:board:nucleo-g474re`][].
- Black Pill F401 as `modm:board:mini-f401`
- Black Pill F411 as `modm:board:mini-f411`

New device drivers:

- SK9822 LED driver (via APA102) as [`modm:driver:sk9822`][].

Known bugs:

- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].
- Generating modm on Windows creates paths with `\` that are not compatible with
  Unix. See [#310][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Benjamin Weps ([@nesos][])
- Mike Wolfram ([@mikewolfram][]) 🎉
- Niklas Hauser ([@salkinium][])
- Raphael Lehmann ([@rleh][])

<details>
<summary>Detailed changelog</summary>

#### 2019-12-19: Add STM32G0 ADC driver and SK9822 LED driver

PR [#318][] -> [fb21f62][].  
Tested in hardware by [@salkinium][].

#### 2019-10-20: Update of modm-devices

Breaking changes: `modm:target` option requires longer identifier, however, it
will give you a list of ambiguous targets to choose from.

PR [#306][] -> [62ccc26][] with **high impact** in all targets.  
Tested in hardware by [@salkinium][].

#### 2019-10-12: Add Mini-F4x1 STM32 boards and examples

PR [#302][] -> [64d177a][] with low impact in STM32 targets.  
Tested in hardware by [@rleh][].

#### 2019-10-09: Add STM32G4 support

Also adds NUCLEO-G474RE board and examples.

PR [#287][] -> [afaea7f][] with low impact in STM32 targets.  
Tested in hardware by [@rleh][].

</details>


## 2019-10-01: Virtual 2019q3 release

This is a virtual release only for reference that covers everything from
2019-07-01.

Breaking changes:

- Modularized CMSIS-DSP integration.
- Refactored CMake build script generator.
- Refactored NRF24 radio driver.
- Removal of deprecated TIPC XPCC backend.
- Removal of deprecated SDL display driver.

Features:

- Remove hardcoded OpenOCD programmer for some boards.
- AVR: size reporting.
- CMake: Support multi-repository builds.
- `HARDWARE_INIT` helper macro for linker section.

Fixes:

- STM32: Acknowledge external interrupt flags.
- STM32: Shared ADC interrupt.

New device drivers:

- BNO055 IMU driver as [`modm:driver:bno055`][].
- TLC594x PWM driver as [`modm:driver:tlc594x`][].
- APA102 RGB LED driver as [`modm:driver:apa102`][].
- SK6812 RGB LED driver as [`modm:driver:sk6812`][].

Known bugs:

- `lbuild build` and `lbuild clean` do not remove all previously generated files
  when the configuration changes. See [#285][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Benjamin Weps ([@nesos][])
- Carl Treudler ([@cajt][])
- Jörg Hoffmann ([@19joho66][]) 🎉
- Kaelin Laundry ([@WasabiFan][]) 🎉
- Linas Nikiperavicius ([@linasnikis][]) 🎉
- Niklas Hauser ([@salkinium][])
- Pavel Pletenev ([@ASMfreaK][])
- Sascha Schade ([@strongly-typed][])

<details>
<summary>Detailed changelog</summary>


#### 2019-08-07: Add APA102 and SK6812 LED drivers

PR [#267][] -> [72d5ae9][].  
Tested in hardware by [@salkinium][].

#### 2019-07-24: Refactor NRF24 radio driver

PR [#253][] -> [22867e0][].  
Tested in hardware by [@danielk][].

#### 2019-07-15: Refactor CMake build script generator

PR [#254][] -> [00471ca][].  
Tested in hardware by [@salkinium][].

#### 2019-07-15: Add TLC594x PWM driver

PR [#242][] -> [b570d07][].  
Tested in hardware by [@linasnikis][].

#### 2019-07-11: Add BNO055 IMU driver

PR [#191][] -> [a8edbe8][].  
Tested in hardware by [@salkinium][].

#### 2019-07-11: Modularize CMSIS-DSP

Breaking changes: You need to choose the required submodules.

PR [#241][] -> [022a60a][].  
Tested in hardware by [@salkinium][].

#### 2019-07-11: Remove hardcoding of OpenOCD programmer

PR [#240][] -> [afbd533][].  
Tested in hardware by [@salkinium][].

</details>


## 2019-07-01: Virtual 2019q2 release

This is a virtual release only for reference that covers everything from
2019-04-01.

Breaking changes:

- Upgrade to AWS a:FreeRTOS v10 with generic port to all Cortex-M devices.
- `modm::endl` does not flush the IOStream anymore.

Features:

- New printf implementation with better overall syntax support.
- Simple CRC32 implementation.
- Cortex-M: CrashCatcher integration for post-mortem debugging of HardFaults.
- Cortex-M: ITM driver with UART interface.
- Cortex-M: GNU Build ID integration.

Fixes:

- AVR: UART driver for some devices.

New development boards:

- Arduino NANO as [`modm:board:arduino-nano`][].
- NUCLEO-F446RE as [`modm:board:nucleo-f446re`][].

New device drivers:

- WS2812 RGB LED driver as [`modm:driver:ws2812`][].

Many thanks to all our contributors:

- Antal Szabó ([@Sh4rK][])
- Niklas Hauser ([@salkinium][])
- Pavel Pletenev ([@ASMfreaK][])
- Sascha Schade ([@strongly-typed][])
- Sebastian Birke ([@se-bi][])

<details>
<summary>Detailed changelog</summary>

#### 2019-06-06: Add NUCLEO-F446RE board and examples

PR [#228][] -> [9381fd0][].  
Tested in hardware by [@se-bi][].

#### 2019-06-06: Add GNU Build ID to firmware

PR [#219][] -> [a607613][].  
Tested in hardware by [@salkinium][].

#### 2019-05-26: Add ITM driver

PR [#218][] -> [47adfd6][].  
Tested in hardware by [@salkinium][].

#### 2019-05-14: Integrate CrashCatcher

Removes previous hard fault LED toggling in favor of much better solution.

PR [#210][] -> [4ab28fe][] with low impact on STM32 targets.  
Tested in hardware by [@salkinium][].

#### 2019-05-09: Upgrade to AWS a:FreeRTOS v10

Breaking changes: Previous port only worked on STM32F4 with our own patches.

PR [#198][] -> [66c0868][] with **high impact** on STM32F4 targets.  
Tested in hardware by [@salkinium][].

#### 2019-05-03: Refactor printf implementation

PR [#199][] -> [4ce1a47][] with low impact on all targets.  
Tested in hardware by [@salkinium][].

#### 2019-04-06: Add WS2812 RGB LED driver

PR [#190][] -> [a6b4186][].  
Tested in hardware by [@salkinium][].

</details>


## 2020-04-01: Virtual 2019q1 release

This is a virtual release only for reference that covers everything from
2019-01-01.

Breaking changes:

- All inheritable lbuild board configs are moved from `modm:board:*` to `modm:*`.
- Split `modm:platform:core` module into `modm:platform:cortex-m` module.
- Refactor SysTick and RCC module.

Features:

- Black Magic Probe upload support.
- STM32: RCC driver with enable()/disable() functions for clocks.
- Cortex-M: Add option to link with flash offset for bootloaders.
- STM32G0 support!

Fixes:

- HD44780: 4-bit initialization.
- NUCLEO-F303K8: Fix SystemClock for CAN.

New device drivers:

- PAT9125EL motion sensor driver as [`modm:driver:pat9125el`][].
- MAX7219 LED matrix driver as [`modm:driver:max7219`][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Christopher Durand ([@chris-durand][])
- Fabian Greif ([@dergraaf][])
- Niklas Hauser ([@salkinium][])
- Patrick Servello ([@patrick--][]) 🎉
- Pavel Pletenev ([@ASMfreaK][]) 🎉
- Raphael Lehmann ([@rleh][])
- Sascha Schade ([@strongly-typed][])
- Sebastian Birke ([@se-bi][])
- Thomas Figueroa ([@OperativeF][]) 🎉
- Zawadniak Pedro ([@PDR5][]) 🎉

<details>
<summary>Detailed changelog</summary>


#### 2019-02-06: Add MAX7219 LED matrix driver

PR [#187][] -> [5dd598c][].  
Tested in hardware by [@dergraaf][].

#### 2019-03-26: Add MAX7219 LED matrix driver

PR [#187][] -> [5dd598c][].  
Tested in hardware by [@dergraaf][].

#### 2019-03-06: Add STM32G0 support

PR [#167][] -> [399a533][].  
Tested in hardware by [@salkinium][].

#### 2019-03-06: Refactor SysTick and RCC module

Breaking changes:

- `modm:platform:clock.cortex` -> `modm:platform:systick`.
- `Rcc::setFlashLatency(Frequency);` -> `Rcc::setFlashLatency<Frequency>();`.

PR [#180][] -> [c7b35ca][] with **high impact** on STM32 targets.  
Tested in hardware by [@salkinium][].

#### 2019-02-28: Add RCC driver

PR [#177][] -> [4a82a94][] with low impact on STM32 targets.  
Tested in hardware by [@salkinium][].

#### 2019-02-28: Move common Cortex-M code into own module

Breaking changes: Some options move to `modm:platform:cortex-m` module.

PR [#176][] -> [dee5ea2][] with **high impact** on STM32 targets.  
Tested in hardware by [@salkinium][].

#### 2019-02-20: Move lbuild board configs to repo namespace

Breaking changes: Config inheritance renamed from `modm:board:*` to `modm:*` .

PR [#171][] -> [564effa][] with **high impact** on all targets.  
Tested by [@salkinium][].

#### 2019-01-04: Add PAT9125EL motion sensor driver

PR [#136][] -> [4f25cdf][].  
Tested in hardware by [@chris-durand][].

</details>


## 2019-01-01: Virtual 2018q4 release

This is a virtual release only for reference that covers everything from
2018-10-01.

No breaking changes.

Features:

- SCons: Multi-repository build script generation.
- AVR: Add libstdc++
- Doxygen: Refactor Doxygen groups to match lbuild structure.
- STM32F0/F3/F7/L0: Add I2C driver.
- STM32L4: Add SPI (via USART) driver.
- STM32F0: Add ADC driver.

Fixes:

- STM32: Timer dead time register mask.
- STM32L4: ADC driver fixed for L4x2.

New device drivers:

- PCA9548A I2C multiplex driver as [`modm:driver:pca9548a`][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Antal Szabó ([@Sh4rK][])
- Benjamin Weps ([@nesos][]) 🎉
- Carl Treudler ([@cajt][])
- Christopher Durand ([@chris-durand][])
- Fabian Greif ([@dergraaf][])
- Jörg Hoffmann ([@19joho66][]) 🎉
- Kaelin Laundry ([@WasabiFan][]) 🎉
- Linas Nikiperavicius ([@linasnikis][]) 🎉
- Mike Wolfram ([@mikewolfram][]) 🎉
- Niklas Hauser ([@salkinium][])
- Niklas Meyer ([@Zweistein885][]) 🎉
- Patrick Servello ([@patrick--][]) 🎉
- Pavel Pletenev ([@ASMfreaK][]) 🎉
- Raphael Lehmann ([@rleh][])
- Sascha Schade ([@strongly-typed][])
- Sebastian Birke ([@se-bi][])
- Thomas Figueroa ([@OperativeF][]) 🎉
- Zawadniak Pedro ([@PDR5][]) 🎉

<details>
<summary>Detailed changelog</summary>

#### 2019-11-26: Add PCA9548A I2C multiplex driver

PR [#81][] -> [77ae899][].  
Tested in hardware by [@strongly-typed][].

#### 2019-11-26: Add ADC driver for STM32F0

PR [#132][] -> [af2b352][].  
Tested in hardware by [@cajt][].

#### 2019-11-10: Add USART-SPI driver for STM32L4

PR [#122][] -> [544f6d3][].  
Tested in hardware by [@chris-durand][].

#### 2019-11-06: Add I2C driver for STM32F0/F3/F7/L0

PR [#118][] -> [e46e7df][].  
Tested in hardware by [@chris-durand][].

#### 2019-10-15: Refactor Doxygen groups to match lbuild structure

PR [#102][] -> [feb1f3c][].  
Tested by [@salkinium][].

#### 2019-10-10: Add libstdc++ subset for AVR

PR [#82][] -> [eba68a4][].  
Tested in hardware by [@chris-durand][].

</details>


## 2018-10-01: Virtual 2018q3 release

This is a virtual release only for reference that covers everything from
2018-07-01.

Breaking changes:

- Move build script generator modules from `modm:build.*` to `modm:build:*` .
- Move XPCC generator to own module `modm:communication:xpcc:generator`.

Features:

- Generate LED tables via lbuild.
- STM32F3/L4: Add comparator driver.
- Enable lbuild module for SAB2.
- OpenOCD customization via `openocd.cfg` file.
- Improved GDB TUI debugging with automatic background OpenOCD start/stop.
- A new Homepage!
- A lot of new module documentation.

Fixes:

- STM32: Compilation of ADC driver.
- ELF debug section cleanup.

New development boards:

- NUCLEO-F042K6 as [`modm:board:nucleo-f042k6`][].

Many thanks to all our contributors.
A special shoutout to first timers 🎉:

- Amarok McLion ([@amarokmclion][]) 🎉
- Antal Szabó ([@Sh4rK][])
- Benjamin Weps ([@nesos][]) 🎉
- Carl Treudler ([@cajt][])
- Christopher Durand ([@chris-durand][])
- Fabian Greif ([@dergraaf][])
- Jörg Hoffmann ([@19joho66][]) 🎉
- Kaelin Laundry ([@WasabiFan][]) 🎉
- Linas Nikiperavicius ([@linasnikis][]) 🎉
- Niklas Hauser ([@salkinium][])
- Niklas Meyer ([@Zweistein885][]) 🎉
- Patrick Servello ([@patrick--][]) 🎉
- Pavel Pletenev ([@ASMfreaK][]) 🎉
- Raphael Lehmann ([@rleh][])
- Sascha Schade ([@strongly-typed][])
- Sascha Schade (strongly-typed) 🎉
- Sebastian Birke ([@se-bi][]) 🎉
- Thomas Figueroa ([@OperativeF][]) 🎉
- Zawadniak Pedro ([@PDR5][]) 🎉

<details>
<summary>Detailed changelog</summary>

#### 2019-08-08: Move XPCC generator to own module

PR [#58][] -> [cbbf3f6][].  
Tested by [@salkinium][].

#### 2019-07-28: Add NUCLEO-F042K6 board

PR [#43][] -> [fd7b7a3][].  
Tested in hardware by [@cajt][].

#### 2019-07-17: Add STM32F3/L4 comparator driver.

PR [#41][] -> [10fdc3f][].  
Tested in hardware by [@rleh][].

</details>


<!--/releases-->

## 2018q2 release

The predecessor to modm was [xpcc](https://github.com/roboterclubaachen/xpcc),
please check the [xpcc changelog][xpcc_changelog] for releases up to 2018q2.

During the transition from xpcc to modm from 2016 to 2018 there were many
breaking changes with an overview available in the [porting guide from xpcc to
modm][porting_guide].

Please note that contributions from xpcc were continuously ported to modm.

[porting_guide]: https://github.com/modm-io/modm/blob/c45d0a68/docs/porting_xpcc_to_modm.md
[xpcc_changelog]: https://github.com/roboterclubaachen/xpcc/blob/develop/CHANGELOG.md

<!--links-->

[2021q1]: https://github.com/modm-io/modm/releases/tag/2021q1
[2021q2]: https://github.com/modm-io/modm/releases/tag/2021q2
[2021q3]: https://github.com/modm-io/modm/releases/tag/2021q3
[2021q4]: https://github.com/modm-io/modm/releases/tag/2021q4
[2022q1]: https://github.com/modm-io/modm/releases/tag/2022q1
[2022q2]: https://github.com/modm-io/modm/releases/tag/2022q2
[2022q3]: https://github.com/modm-io/modm/releases/tag/2022q3
[2022q4]: https://github.com/modm-io/modm/releases/tag/2022q4
[2023q1]: https://github.com/modm-io/modm/releases/tag/2023q1
[2023q2]: https://github.com/modm-io/modm/releases/tag/2023q2
[2023q3]: https://github.com/modm-io/modm/releases/tag/2023q3
[2023q4]: https://github.com/modm-io/modm/releases/tag/2023q4
[2024q1]: https://github.com/modm-io/modm/releases/tag/2024q1
[2024q2]: https://github.com/modm-io/modm/releases/tag/2024q2

[@19joho66]: https://github.com/19joho66
[@ASMfreaK]: https://github.com/ASMfreaK
[@Artiom9]: https://github.com/Artiom9
[@FelixPetriconi]: https://github.com/FelixPetriconi
[@JKazem]: https://github.com/JKazem
[@Javask]: https://github.com/Javask
[@OperativeF]: https://github.com/OperativeF
[@PDR5]: https://github.com/PDR5
[@SgtPepperFTW]: https://github.com/SgtPepperFTW
[@Sh4rK]: https://github.com/Sh4rK
[@StevenMacias]: https://github.com/StevenMacias
[@TomSaw]: https://github.com/TomSaw
[@WasabiFan]: https://github.com/WasabiFan
[@XDjackieXD]: https://github.com/XDjackieXD
[@Zweistein885]: https://github.com/Zweistein885
[@amarokmclion]: https://github.com/amarokmclion
[@andryblack]: https://github.com/andryblack
[@becseya]: https://github.com/becseya
[@cajt]: https://github.com/cajt
[@ceremcem]: https://github.com/ceremcem
[@chris-durand]: https://github.com/chris-durand
[@cocasema]: https://github.com/cocasema
[@daniel-k]: https://github.com/daniel-k
[@danielk]: https://github.com/danielk
[@delphi]: https://github.com/delphi
[@dergraaf]: https://github.com/dergraaf
[@dhebbeker]: https://github.com/dhebbeker
[@el-han]: https://github.com/el-han
[@gueldenstone]: https://github.com/gueldenstone
[@henrikssn]: https://github.com/henrikssn
[@hshose]: https://github.com/hshose
[@jasa]: https://github.com/jasa
[@jensboe]: https://github.com/jensboe
[@kapacuk]: https://github.com/kapacuk
[@klsc-zeat]: https://github.com/klsc-zeat
[@lgili]: https://github.com/lgili
[@linasnikis]: https://github.com/linasnikis
[@lmoesch]: https://github.com/lmoesch
[@lukh]: https://github.com/lukh
[@luxarf]: https://github.com/luxarf
[@mat-kie]: https://github.com/mat-kie
[@mbait]: https://github.com/mbait
[@mcbridejc]: https://github.com/mcbridejc
[@mikewolfram]: https://github.com/mikewolfram
[@nesos]: https://github.com/nesos
[@nicoBruhn]: https://github.com/nicoBruhn
[@odinthenerd]: https://github.com/odinthenerd
[@patrick--]: https://github.com/patrick--
[@rasmuskleist]: https://github.com/rasmuskleist
[@rleh]: https://github.com/rleh
[@salkinium]: https://github.com/salkinium
[@sarahvilete]: https://github.com/sarahvilete
[@se-bi]: https://github.com/se-bi
[@ser-plu]: https://github.com/ser-plu
[@strongly-typed]: https://github.com/strongly-typed
[@tarush53]: https://github.com/tarush53
[@tomsaw]: https://github.com/tomsaw
[@twasilczyk]: https://github.com/twasilczyk
[@twast92]: https://github.com/twast92
[@victorandrehc]: https://github.com/victorandrehc
[@xgzeng]: https://github.com/xgzeng

[`modm:board:arduino-nano`]: https://modm.io/reference/module/modm-board-arduino-nano
[`modm:board:devebox-stm32f4xx`]: https://modm.io/reference/module/modm-board-devebox-stm32f4xx
[`modm:board:devebox-stm32h750vb`]: https://modm.io/reference/module/modm-board-devebox-stm32h750vb
[`modm:board:feather-m0`]: https://modm.io/reference/module/modm-board-feather-m0
[`modm:board:feather-rp2040`]: https://modm.io/reference/module/modm-board-feather-rp2040
[`modm:board:mega-2560-pro`]: https://modm.io/reference/module/modm-board-mega-2560-pro
[`modm:board:nucleo-f042k6`]: https://modm.io/reference/module/modm-board-nucleo-f042k6
[`modm:board:nucleo-f072rb`]: https://modm.io/reference/module/modm-board-nucleo-f072rb
[`modm:board:nucleo-f091rc`]: https://modm.io/reference/module/modm-board-nucleo-f091rc
[`modm:board:nucleo-f303re`]: https://modm.io/reference/module/modm-board-nucleo-f303re
[`modm:board:nucleo-f334r8`]: https://modm.io/reference/module/modm-board-nucleo-f334r8
[`modm:board:nucleo-f439zi`]: https://modm.io/reference/module/modm-board-nucleo-f439zi
[`modm:board:nucleo-f446re`]: https://modm.io/reference/module/modm-board-nucleo-f446re
[`modm:board:nucleo-f446ze`]: https://modm.io/reference/module/modm-board-nucleo-f446ze
[`modm:board:nucleo-f767zi`]: https://modm.io/reference/module/modm-board-nucleo-f767zi
[`modm:board:nucleo-g431kb`]: https://modm.io/reference/module/modm-board-nucleo-g431kb
[`modm:board:nucleo-g431rb`]: https://modm.io/reference/module/modm-board-nucleo-g431rb
[`modm:board:nucleo-g474re`]: https://modm.io/reference/module/modm-board-nucleo-g474re
[`modm:board:nucleo-h723zg`]: https://modm.io/reference/module/modm-board-nucleo-h723zg
[`modm:board:nucleo-h743zi`]: https://modm.io/reference/module/modm-board-nucleo-h743zi
[`modm:board:nucleo-l031k6`]: https://modm.io/reference/module/modm-board-nucleo-l031k6
[`modm:board:nucleo-l053r8`]: https://modm.io/reference/module/modm-board-nucleo-l053r8
[`modm:board:nucleo-l452re`]: https://modm.io/reference/module/modm-board-nucleo-l452re
[`modm:board:nucleo-l496zg-p`]: https://modm.io/reference/module/modm-board-nucleo-l496zg-p
[`modm:board:nucleo-l552ze-q`]: https://modm.io/reference/module/modm-board-nucleo-l552ze-q
[`modm:board:raspberrypi`]: https://modm.io/reference/module/modm-board-raspberrypi
[`modm:board:rp-pico`]: https://modm.io/reference/module/modm-board-rp-pico
[`modm:board:samd21-mini`]: https://modm.io/reference/module/modm-board-samd21-mini
[`modm:board:samd21-xplained-pro`]: https://modm.io/reference/module/modm-board-samd21-xplained-pro
[`modm:board:same54-xplained-pro`]: https://modm.io/reference/module/modm-board-same54-xplained-pro
[`modm:board:same70-xplained`]: https://modm.io/reference/module/modm-board-same70-xplained
[`modm:board:samg55-xplained-pro`]: https://modm.io/reference/module/modm-board-samg55-xplained-pro
[`modm:board:samv71-xplained-ultra`]: https://modm.io/reference/module/modm-board-samv71-xplained-ultra
[`modm:board:srxe`]: https://modm.io/reference/module/modm-board-srxe
[`modm:board:stm32_f32ve`]: https://modm.io/reference/module/modm-board-stm32_f32ve
[`modm:board:thingplus-rp2040`]: https://modm.io/reference/module/modm-board-thingplus-rp2040
[`modm:disco-f401vc`]: https://modm.io/reference/module/modm-disco-f401vc
[`modm:disco-f411ve`]: https://modm.io/reference/module/modm-disco-f411ve
[`modm:disco-f469ni:b-03`]: https://modm.io/reference/module/modm-disco-f469ni-b-03
[`modm:driver:adis16470`]: https://modm.io/reference/module/modm-driver-adis16470
[`modm:driver:ads7828`]: https://modm.io/reference/module/modm-driver-ads7828
[`modm:driver:ads816x`]: https://modm.io/reference/module/modm-driver-ads816x
[`modm:driver:apa102`]: https://modm.io/reference/module/modm-driver-apa102
[`modm:driver:as5047`]: https://modm.io/reference/module/modm-driver-as5047
[`modm:driver:as5600`]: https://modm.io/reference/module/modm-driver-as5600
[`modm:driver:at24mac402`]: https://modm.io/reference/module/modm-driver-at24mac402
[`modm:driver:block.device:spi.stack.flash`]: https://modm.io/reference/module/modm-driver-block-device-spi-stack-flash
[`modm:driver:bmi088`]: https://modm.io/reference/module/modm-driver-bmi088
[`modm:driver:bno055`]: https://modm.io/reference/module/modm-driver-bno055
[`modm:driver:cat24aa`]: https://modm.io/reference/module/modm-driver-cat24aa
[`modm:driver:cycle_counter`]: https://modm.io/reference/module/modm-driver-cycle_counter
[`modm:driver:encoder.output`]: https://modm.io/reference/module/modm-driver-encoder-output
[`modm:driver:encoder_input.bitbang`]: https://modm.io/reference/module/modm-driver-encoder_input-bitbang
[`modm:driver:encoder_input`]: https://modm.io/reference/module/modm-driver-encoder_input
[`modm:driver:gpio-sampler`]: https://modm.io/reference/module/modm-driver-gpio-sampler
[`modm:driver:ili9341`]: https://modm.io/reference/module/modm-driver-ili9341
[`modm:driver:is31fl3733`]: https://modm.io/reference/module/modm-driver-is31fl3733
[`modm:driver:ixm42xxx`]: https://modm.io/reference/module/modm-driver-ixm42xxx
[`modm:driver:lan8720a`]: https://modm.io/reference/module/modm-driver-lan8720a
[`modm:driver:lis3mdl`]: https://modm.io/reference/module/modm-driver-lis3mdl
[`modm:driver:lp503x`]: https://modm.io/reference/module/modm-driver-lp503x
[`modm:driver:lsm6ds33`]: https://modm.io/reference/module/modm-driver-lsm6ds33
[`modm:driver:lsm6dso`]: https://modm.io/reference/module/modm-driver-lsm6dso
[`modm:driver:max31865`]: https://modm.io/reference/module/modm-driver-max31865
[`modm:driver:max7219`]: https://modm.io/reference/module/modm-driver-max7219
[`modm:driver:mcp3008`]: https://modm.io/reference/module/modm-driver-mcp3008
[`modm:driver:mcp7941x`]: https://modm.io/reference/module/modm-driver-mcp7941x
[`modm:driver:mcp990x`]: https://modm.io/reference/module/modm-driver-mcp990x
[`modm:driver:mmc5603`]: https://modm.io/reference/module/modm-driver-mmc5603
[`modm:driver:ms5837`]: https://modm.io/reference/module/modm-driver-ms5837
[`modm:driver:pat9125el`]: https://modm.io/reference/module/modm-driver-pat9125el
[`modm:driver:pca9548a`]: https://modm.io/reference/module/modm-driver-pca9548a
[`modm:driver:qmc5883l`]: https://modm.io/reference/module/modm-driver-qmc5883l
[`modm:driver:sh1106`]: https://modm.io/reference/module/modm-driver-sh1106
[`modm:driver:sk6812`]: https://modm.io/reference/module/modm-driver-sk6812
[`modm:driver:sk9822`]: https://modm.io/reference/module/modm-driver-sk9822
[`modm:driver:st7586s`]: https://modm.io/reference/module/modm-driver-st7586s
[`modm:driver:st7789`]: https://modm.io/reference/module/modm-driver-st7789
[`modm:driver:stts22h`]: https://modm.io/reference/module/modm-driver-stts22h
[`modm:driver:stusb4500`]: https://modm.io/reference/module/modm-driver-stusb4500
[`modm:driver:sx1276`]: https://modm.io/reference/module/modm-driver-sx1276
[`modm:driver:sx128x`]: https://modm.io/reference/module/modm-driver-sx128x
[`modm:driver:tlc594x`]: https://modm.io/reference/module/modm-driver-tlc594x
[`modm:driver:tmp12x`]: https://modm.io/reference/module/modm-driver-tmp12x
[`modm:driver:touch2046`]: https://modm.io/reference/module/modm-driver-touch2046
[`modm:driver:ws2812`]: https://modm.io/reference/module/modm-driver-ws2812
[`modm:feather-m4`]: https://modm.io/reference/module/modm-feather-m4
[`modm:nucleo-g070rb`]: https://modm.io/reference/module/modm-nucleo-g070rb
[`modm:nucleo-u575zi-q`]: https://modm.io/reference/module/modm-nucleo-u575zi-q

[#1001]: https://github.com/modm-io/modm/pull/1001
[#1009]: https://github.com/modm-io/modm/pull/1009
[#1010]: https://github.com/modm-io/modm/pull/1010
[#1012]: https://github.com/modm-io/modm/pull/1012
[#1017]: https://github.com/modm-io/modm/pull/1017
[#1018]: https://github.com/modm-io/modm/pull/1018
[#1026]: https://github.com/modm-io/modm/pull/1026
[#1028]: https://github.com/modm-io/modm/pull/1028
[#102]: https://github.com/modm-io/modm/pull/102
[#1032]: https://github.com/modm-io/modm/pull/1032
[#1036]: https://github.com/modm-io/modm/pull/1036
[#1037]: https://github.com/modm-io/modm/pull/1037
[#1038]: https://github.com/modm-io/modm/pull/1038
[#1040]: https://github.com/modm-io/modm/pull/1040
[#1044]: https://github.com/modm-io/modm/pull/1044
[#1048]: https://github.com/modm-io/modm/pull/1048
[#1049]: https://github.com/modm-io/modm/pull/1049
[#1050]: https://github.com/modm-io/modm/pull/1050
[#1051]: https://github.com/modm-io/modm/pull/1051
[#1052]: https://github.com/modm-io/modm/pull/1052
[#1053]: https://github.com/modm-io/modm/pull/1053
[#1054]: https://github.com/modm-io/modm/pull/1054
[#1063]: https://github.com/modm-io/modm/pull/1063
[#1066]: https://github.com/modm-io/modm/pull/1066
[#1079]: https://github.com/modm-io/modm/pull/1079
[#1088]: https://github.com/modm-io/modm/pull/1088
[#1111]: https://github.com/modm-io/modm/pull/1111
[#1112]: https://github.com/modm-io/modm/pull/1112
[#1113]: https://github.com/modm-io/modm/pull/1113
[#1114]: https://github.com/modm-io/modm/pull/1114
[#1115]: https://github.com/modm-io/modm/pull/1115
[#1116]: https://github.com/modm-io/modm/pull/1116
[#1117]: https://github.com/modm-io/modm/pull/1117
[#1120]: https://github.com/modm-io/modm/pull/1120
[#1125]: https://github.com/modm-io/modm/pull/1125
[#1126]: https://github.com/modm-io/modm/pull/1126
[#1127]: https://github.com/modm-io/modm/pull/1127
[#1134]: https://github.com/modm-io/modm/pull/1134
[#1135]: https://github.com/modm-io/modm/pull/1135
[#1136]: https://github.com/modm-io/modm/pull/1136
[#1138]: https://github.com/modm-io/modm/pull/1138
[#1140]: https://github.com/modm-io/modm/pull/1140
[#1143]: https://github.com/modm-io/modm/pull/1143
[#1146]: https://github.com/modm-io/modm/pull/1146
[#1148]: https://github.com/modm-io/modm/pull/1148
[#1153]: https://github.com/modm-io/modm/pull/1153
[#1156]: https://github.com/modm-io/modm/pull/1156
[#1164]: https://github.com/modm-io/modm/pull/1164
[#1166]: https://github.com/modm-io/modm/pull/1166
[#1168]: https://github.com/modm-io/modm/pull/1168
[#1171]: https://github.com/modm-io/modm/pull/1171
[#1175]: https://github.com/modm-io/modm/pull/1175
[#118]: https://github.com/modm-io/modm/pull/118
[#122]: https://github.com/modm-io/modm/pull/122
[#132]: https://github.com/modm-io/modm/pull/132
[#136]: https://github.com/modm-io/modm/pull/136
[#153]: https://github.com/modm-io/modm/pull/153
[#167]: https://github.com/modm-io/modm/pull/167
[#171]: https://github.com/modm-io/modm/pull/171
[#176]: https://github.com/modm-io/modm/pull/176
[#177]: https://github.com/modm-io/modm/pull/177
[#180]: https://github.com/modm-io/modm/pull/180
[#187]: https://github.com/modm-io/modm/pull/187
[#190]: https://github.com/modm-io/modm/pull/190
[#191]: https://github.com/modm-io/modm/pull/191
[#194]: https://github.com/modm-io/modm/pull/194
[#198]: https://github.com/modm-io/modm/pull/198
[#199]: https://github.com/modm-io/modm/pull/199
[#210]: https://github.com/modm-io/modm/pull/210
[#217]: https://github.com/modm-io/modm/pull/217
[#218]: https://github.com/modm-io/modm/pull/218
[#219]: https://github.com/modm-io/modm/pull/219
[#228]: https://github.com/modm-io/modm/pull/228
[#240]: https://github.com/modm-io/modm/pull/240
[#241]: https://github.com/modm-io/modm/pull/241
[#242]: https://github.com/modm-io/modm/pull/242
[#253]: https://github.com/modm-io/modm/pull/253
[#254]: https://github.com/modm-io/modm/pull/254
[#267]: https://github.com/modm-io/modm/pull/267
[#285]: https://github.com/modm-io/modm/pull/285
[#287]: https://github.com/modm-io/modm/pull/287
[#302]: https://github.com/modm-io/modm/pull/302
[#306]: https://github.com/modm-io/modm/pull/306
[#310]: https://github.com/modm-io/modm/pull/310
[#318]: https://github.com/modm-io/modm/pull/318
[#321]: https://github.com/modm-io/modm/pull/321
[#326]: https://github.com/modm-io/modm/pull/326
[#343]: https://github.com/modm-io/modm/pull/343
[#346]: https://github.com/modm-io/modm/pull/346
[#351]: https://github.com/modm-io/modm/pull/351
[#361]: https://github.com/modm-io/modm/pull/361
[#364]: https://github.com/modm-io/modm/pull/364
[#366]: https://github.com/modm-io/modm/pull/366
[#371]: https://github.com/modm-io/modm/pull/371
[#372]: https://github.com/modm-io/modm/pull/372
[#374]: https://github.com/modm-io/modm/pull/374
[#376]: https://github.com/modm-io/modm/pull/376
[#382]: https://github.com/modm-io/modm/pull/382
[#395]: https://github.com/modm-io/modm/pull/395
[#396]: https://github.com/modm-io/modm/pull/396
[#400]: https://github.com/modm-io/modm/pull/400
[#417]: https://github.com/modm-io/modm/pull/417
[#418]: https://github.com/modm-io/modm/pull/418
[#41]: https://github.com/modm-io/modm/pull/41
[#420]: https://github.com/modm-io/modm/pull/420
[#426]: https://github.com/modm-io/modm/pull/426
[#428]: https://github.com/modm-io/modm/pull/428
[#430]: https://github.com/modm-io/modm/pull/430
[#436]: https://github.com/modm-io/modm/pull/436
[#437]: https://github.com/modm-io/modm/pull/437
[#43]: https://github.com/modm-io/modm/pull/43
[#444]: https://github.com/modm-io/modm/pull/444
[#445]: https://github.com/modm-io/modm/pull/445
[#448]: https://github.com/modm-io/modm/pull/448
[#462]: https://github.com/modm-io/modm/pull/462
[#466]: https://github.com/modm-io/modm/pull/466
[#471]: https://github.com/modm-io/modm/pull/471
[#475]: https://github.com/modm-io/modm/pull/475
[#478]: https://github.com/modm-io/modm/pull/478
[#482]: https://github.com/modm-io/modm/pull/482
[#485]: https://github.com/modm-io/modm/pull/485
[#489]: https://github.com/modm-io/modm/pull/489
[#490]: https://github.com/modm-io/modm/pull/490
[#495]: https://github.com/modm-io/modm/pull/495
[#497]: https://github.com/modm-io/modm/pull/497
[#499]: https://github.com/modm-io/modm/pull/499
[#521]: https://github.com/modm-io/modm/pull/521
[#526]: https://github.com/modm-io/modm/pull/526
[#533]: https://github.com/modm-io/modm/pull/533
[#535]: https://github.com/modm-io/modm/pull/535
[#544]: https://github.com/modm-io/modm/pull/544
[#556]: https://github.com/modm-io/modm/pull/556
[#557]: https://github.com/modm-io/modm/pull/557
[#558]: https://github.com/modm-io/modm/pull/558
[#564]: https://github.com/modm-io/modm/pull/564
[#566]: https://github.com/modm-io/modm/pull/566
[#568]: https://github.com/modm-io/modm/pull/568
[#571]: https://github.com/modm-io/modm/pull/571
[#574]: https://github.com/modm-io/modm/pull/574
[#578]: https://github.com/modm-io/modm/pull/578
[#580]: https://github.com/modm-io/modm/pull/580
[#58]: https://github.com/modm-io/modm/pull/58
[#591]: https://github.com/modm-io/modm/pull/591
[#597]: https://github.com/modm-io/modm/pull/597
[#600]: https://github.com/modm-io/modm/pull/600
[#603]: https://github.com/modm-io/modm/pull/603
[#604]: https://github.com/modm-io/modm/pull/604
[#607]: https://github.com/modm-io/modm/pull/607
[#608]: https://github.com/modm-io/modm/pull/608
[#610]: https://github.com/modm-io/modm/pull/610
[#614]: https://github.com/modm-io/modm/pull/614
[#616]: https://github.com/modm-io/modm/pull/616
[#627]: https://github.com/modm-io/modm/pull/627
[#629]: https://github.com/modm-io/modm/pull/629
[#632]: https://github.com/modm-io/modm/pull/632
[#642]: https://github.com/modm-io/modm/pull/642
[#652]: https://github.com/modm-io/modm/pull/652
[#657]: https://github.com/modm-io/modm/pull/657
[#667]: https://github.com/modm-io/modm/pull/667
[#669]: https://github.com/modm-io/modm/pull/669
[#673]: https://github.com/modm-io/modm/pull/673
[#676]: https://github.com/modm-io/modm/pull/676
[#678]: https://github.com/modm-io/modm/pull/678
[#679]: https://github.com/modm-io/modm/pull/679
[#680]: https://github.com/modm-io/modm/pull/680
[#681]: https://github.com/modm-io/modm/pull/681
[#685]: https://github.com/modm-io/modm/pull/685
[#686]: https://github.com/modm-io/modm/pull/686
[#742]: https://github.com/modm-io/modm/pull/742
[#743]: https://github.com/modm-io/modm/pull/743
[#745]: https://github.com/modm-io/modm/pull/745
[#747]: https://github.com/modm-io/modm/pull/747
[#748]: https://github.com/modm-io/modm/pull/748
[#750]: https://github.com/modm-io/modm/pull/750
[#753]: https://github.com/modm-io/modm/pull/753
[#756]: https://github.com/modm-io/modm/pull/756
[#757]: https://github.com/modm-io/modm/pull/757
[#761]: https://github.com/modm-io/modm/pull/761
[#763]: https://github.com/modm-io/modm/pull/763
[#764]: https://github.com/modm-io/modm/pull/764
[#772]: https://github.com/modm-io/modm/pull/772
[#773]: https://github.com/modm-io/modm/pull/773
[#775]: https://github.com/modm-io/modm/pull/775
[#777]: https://github.com/modm-io/modm/pull/777
[#780]: https://github.com/modm-io/modm/pull/780
[#796]: https://github.com/modm-io/modm/pull/796
[#797]: https://github.com/modm-io/modm/pull/797
[#800]: https://github.com/modm-io/modm/pull/800
[#805]: https://github.com/modm-io/modm/pull/805
[#806]: https://github.com/modm-io/modm/pull/806
[#816]: https://github.com/modm-io/modm/pull/816
[#819]: https://github.com/modm-io/modm/pull/819
[#81]: https://github.com/modm-io/modm/pull/81
[#821]: https://github.com/modm-io/modm/pull/821
[#822]: https://github.com/modm-io/modm/pull/822
[#823]: https://github.com/modm-io/modm/pull/823
[#824]: https://github.com/modm-io/modm/pull/824
[#827]: https://github.com/modm-io/modm/pull/827
[#82]: https://github.com/modm-io/modm/pull/82
[#830]: https://github.com/modm-io/modm/pull/830
[#838]: https://github.com/modm-io/modm/pull/838
[#842]: https://github.com/modm-io/modm/pull/842
[#843]: https://github.com/modm-io/modm/pull/843
[#846]: https://github.com/modm-io/modm/pull/846
[#848]: https://github.com/modm-io/modm/pull/848
[#850]: https://github.com/modm-io/modm/pull/850
[#851]: https://github.com/modm-io/modm/pull/851
[#853]: https://github.com/modm-io/modm/pull/853
[#854]: https://github.com/modm-io/modm/pull/854
[#856]: https://github.com/modm-io/modm/pull/856
[#858]: https://github.com/modm-io/modm/pull/858
[#859]: https://github.com/modm-io/modm/pull/859
[#861]: https://github.com/modm-io/modm/pull/861
[#862]: https://github.com/modm-io/modm/pull/862
[#864]: https://github.com/modm-io/modm/pull/864
[#866]: https://github.com/modm-io/modm/pull/866
[#872]: https://github.com/modm-io/modm/pull/872
[#873]: https://github.com/modm-io/modm/pull/873
[#875]: https://github.com/modm-io/modm/pull/875
[#882]: https://github.com/modm-io/modm/pull/882
[#887]: https://github.com/modm-io/modm/pull/887
[#894]: https://github.com/modm-io/modm/pull/894
[#898]: https://github.com/modm-io/modm/pull/898
[#899]: https://github.com/modm-io/modm/pull/899
[#900]: https://github.com/modm-io/modm/pull/900
[#902]: https://github.com/modm-io/modm/pull/902
[#917]: https://github.com/modm-io/modm/pull/917
[#935]: https://github.com/modm-io/modm/pull/935
[#937]: https://github.com/modm-io/modm/pull/937
[#938]: https://github.com/modm-io/modm/pull/938
[#939]: https://github.com/modm-io/modm/pull/939
[#940]: https://github.com/modm-io/modm/pull/940
[#941]: https://github.com/modm-io/modm/pull/941
[#942]: https://github.com/modm-io/modm/pull/942
[#944]: https://github.com/modm-io/modm/pull/944
[#947]: https://github.com/modm-io/modm/pull/947
[#950]: https://github.com/modm-io/modm/pull/950
[#951]: https://github.com/modm-io/modm/pull/951
[#952]: https://github.com/modm-io/modm/pull/952
[#954]: https://github.com/modm-io/modm/pull/954
[#955]: https://github.com/modm-io/modm/pull/955
[#956]: https://github.com/modm-io/modm/pull/956
[#957]: https://github.com/modm-io/modm/pull/957
[#960]: https://github.com/modm-io/modm/pull/960
[#961]: https://github.com/modm-io/modm/pull/961
[#964]: https://github.com/modm-io/modm/pull/964
[#965]: https://github.com/modm-io/modm/pull/965
[#967]: https://github.com/modm-io/modm/pull/967
[#969]: https://github.com/modm-io/modm/pull/969
[#96]: https://github.com/modm-io/modm/pull/96
[#970]: https://github.com/modm-io/modm/pull/970
[#976]: https://github.com/modm-io/modm/pull/976
[#981]: https://github.com/modm-io/modm/pull/981
[#982]: https://github.com/modm-io/modm/pull/982
[#986]: https://github.com/modm-io/modm/pull/986
[#987]: https://github.com/modm-io/modm/pull/987
[#993]: https://github.com/modm-io/modm/pull/993
[#994]: https://github.com/modm-io/modm/pull/994
[#995]: https://github.com/modm-io/modm/pull/995
[#998]: https://github.com/modm-io/modm/pull/998

[00471ca]: https://github.com/modm-io/modm/commit/00471ca
[009bb06]: https://github.com/modm-io/modm/commit/009bb06
[0217a19]: https://github.com/modm-io/modm/commit/0217a19
[022a60a]: https://github.com/modm-io/modm/commit/022a60a
[0259ad2]: https://github.com/modm-io/modm/commit/0259ad2
[027811f]: https://github.com/modm-io/modm/commit/027811f
[02b1571]: https://github.com/modm-io/modm/commit/02b1571
[038657c]: https://github.com/modm-io/modm/commit/038657c
[04688bc]: https://github.com/modm-io/modm/commit/04688bc
[04ed0a5]: https://github.com/modm-io/modm/commit/04ed0a5
[08cd479]: https://github.com/modm-io/modm/commit/08cd479
[0994a55]: https://github.com/modm-io/modm/commit/0994a55
[0cd2bc9]: https://github.com/modm-io/modm/commit/0cd2bc9
[0cf1c65]: https://github.com/modm-io/modm/commit/0cf1c65
[0d6a937]: https://github.com/modm-io/modm/commit/0d6a937
[0e3d280]: https://github.com/modm-io/modm/commit/0e3d280
[0f0505f]: https://github.com/modm-io/modm/commit/0f0505f
[0fd53a8]: https://github.com/modm-io/modm/commit/0fd53a8
[10fdc3f]: https://github.com/modm-io/modm/commit/10fdc3f
[11ffe92]: https://github.com/modm-io/modm/commit/11ffe92
[12bb41b]: https://github.com/modm-io/modm/commit/12bb41b
[1375ff1]: https://github.com/modm-io/modm/commit/1375ff1
[13ea578]: https://github.com/modm-io/modm/commit/13ea578
[141aa71]: https://github.com/modm-io/modm/commit/141aa71
[165adf0]: https://github.com/modm-io/modm/commit/165adf0
[187ddd8]: https://github.com/modm-io/modm/commit/187ddd8
[190bc78]: https://github.com/modm-io/modm/commit/190bc78
[195f7e1]: https://github.com/modm-io/modm/commit/195f7e1
[1a11b08]: https://github.com/modm-io/modm/commit/1a11b08
[1a89fbe]: https://github.com/modm-io/modm/commit/1a89fbe
[1c9c0b6]: https://github.com/modm-io/modm/commit/1c9c0b6
[1f210c1]: https://github.com/modm-io/modm/commit/1f210c1
[1f5d06e]: https://github.com/modm-io/modm/commit/1f5d06e
[1fc3805]: https://github.com/modm-io/modm/commit/1fc3805
[21af57b]: https://github.com/modm-io/modm/commit/21af57b
[21ba120]: https://github.com/modm-io/modm/commit/21ba120
[2273bae]: https://github.com/modm-io/modm/commit/2273bae
[22867e0]: https://github.com/modm-io/modm/commit/22867e0
[23036e3]: https://github.com/modm-io/modm/commit/23036e3
[2381c61]: https://github.com/modm-io/modm/commit/2381c61
[2384756]: https://github.com/modm-io/modm/commit/2384756
[23ec952]: https://github.com/modm-io/modm/commit/23ec952
[241b0d1]: https://github.com/modm-io/modm/commit/241b0d1
[276f5b3]: https://github.com/modm-io/modm/commit/276f5b3
[295dbc3]: https://github.com/modm-io/modm/commit/295dbc3
[2c22fae]: https://github.com/modm-io/modm/commit/2c22fae
[2d2199b]: https://github.com/modm-io/modm/commit/2d2199b
[2e34b11]: https://github.com/modm-io/modm/commit/2e34b11
[2e40ab4]: https://github.com/modm-io/modm/commit/2e40ab4
[2ef7a29]: https://github.com/modm-io/modm/commit/2ef7a29
[3072005]: https://github.com/modm-io/modm/commit/3072005
[30e24e6]: https://github.com/modm-io/modm/commit/30e24e6
[387a625]: https://github.com/modm-io/modm/commit/387a625
[389a9c3]: https://github.com/modm-io/modm/commit/389a9c3
[3936a28]: https://github.com/modm-io/modm/commit/3936a28
[399a533]: https://github.com/modm-io/modm/commit/399a533
[39a9f4d]: https://github.com/modm-io/modm/commit/39a9f4d
[3ba71c9]: https://github.com/modm-io/modm/commit/3ba71c9
[3cee015]: https://github.com/modm-io/modm/commit/3cee015
[3ecad35]: https://github.com/modm-io/modm/commit/3ecad35
[3f3ff3d]: https://github.com/modm-io/modm/commit/3f3ff3d
[416ced6]: https://github.com/modm-io/modm/commit/416ced6
[43f32e6]: https://github.com/modm-io/modm/commit/43f32e6
[45ae68a]: https://github.com/modm-io/modm/commit/45ae68a
[47adfd6]: https://github.com/modm-io/modm/commit/47adfd6
[4885c53]: https://github.com/modm-io/modm/commit/4885c53
[48d73dc]: https://github.com/modm-io/modm/commit/48d73dc
[490e868]: https://github.com/modm-io/modm/commit/490e868
[4a82a94]: https://github.com/modm-io/modm/commit/4a82a94
[4ab28fe]: https://github.com/modm-io/modm/commit/4ab28fe
[4ce1a47]: https://github.com/modm-io/modm/commit/4ce1a47
[4d69227]: https://github.com/modm-io/modm/commit/4d69227
[4f25cdf]: https://github.com/modm-io/modm/commit/4f25cdf
[4f50d00]: https://github.com/modm-io/modm/commit/4f50d00
[4ff604f]: https://github.com/modm-io/modm/commit/4ff604f
[516b2b3]: https://github.com/modm-io/modm/commit/516b2b3
[517bd84]: https://github.com/modm-io/modm/commit/517bd84
[5332765]: https://github.com/modm-io/modm/commit/5332765
[53796b0]: https://github.com/modm-io/modm/commit/53796b0
[544f6d3]: https://github.com/modm-io/modm/commit/544f6d3
[55c88ba]: https://github.com/modm-io/modm/commit/55c88ba
[55d5911]: https://github.com/modm-io/modm/commit/55d5911
[564effa]: https://github.com/modm-io/modm/commit/564effa
[589aea7]: https://github.com/modm-io/modm/commit/589aea7
[596eafa]: https://github.com/modm-io/modm/commit/596eafa
[599e0ba]: https://github.com/modm-io/modm/commit/599e0ba
[5a9ad25]: https://github.com/modm-io/modm/commit/5a9ad25
[5d03d53]: https://github.com/modm-io/modm/commit/5d03d53
[5dcdf1d]: https://github.com/modm-io/modm/commit/5dcdf1d
[5dd598c]: https://github.com/modm-io/modm/commit/5dd598c
[6057873]: https://github.com/modm-io/modm/commit/6057873
[623a13b]: https://github.com/modm-io/modm/commit/623a13b
[624ce10]: https://github.com/modm-io/modm/commit/624ce10
[62b63f5]: https://github.com/modm-io/modm/commit/62b63f5
[62ccc26]: https://github.com/modm-io/modm/commit/62ccc26
[64d177a]: https://github.com/modm-io/modm/commit/64d177a
[65bbccf]: https://github.com/modm-io/modm/commit/65bbccf
[66c0868]: https://github.com/modm-io/modm/commit/66c0868
[6b4d656]: https://github.com/modm-io/modm/commit/6b4d656
[6b5b4ce]: https://github.com/modm-io/modm/commit/6b5b4ce
[6be3199]: https://github.com/modm-io/modm/commit/6be3199
[6de70ba]: https://github.com/modm-io/modm/commit/6de70ba
[6e5ebf4]: https://github.com/modm-io/modm/commit/6e5ebf4
[6e7c12f]: https://github.com/modm-io/modm/commit/6e7c12f
[6e9f000]: https://github.com/modm-io/modm/commit/6e9f000
[71becdc]: https://github.com/modm-io/modm/commit/71becdc
[72d5ae9]: https://github.com/modm-io/modm/commit/72d5ae9
[7318c28]: https://github.com/modm-io/modm/commit/7318c28
[7330500]: https://github.com/modm-io/modm/commit/7330500
[734de07]: https://github.com/modm-io/modm/commit/734de07
[740fd51]: https://github.com/modm-io/modm/commit/740fd51
[768d749]: https://github.com/modm-io/modm/commit/768d749
[77ae899]: https://github.com/modm-io/modm/commit/77ae899
[78d18f6]: https://github.com/modm-io/modm/commit/78d18f6
[7b5827f]: https://github.com/modm-io/modm/commit/7b5827f
[7d1f7cc]: https://github.com/modm-io/modm/commit/7d1f7cc
[7d7490d]: https://github.com/modm-io/modm/commit/7d7490d
[7df2e7d]: https://github.com/modm-io/modm/commit/7df2e7d
[7f30961]: https://github.com/modm-io/modm/commit/7f30961
[8012d82]: https://github.com/modm-io/modm/commit/8012d82
[8082f69]: https://github.com/modm-io/modm/commit/8082f69
[80a9c66]: https://github.com/modm-io/modm/commit/80a9c66
[80ed738]: https://github.com/modm-io/modm/commit/80ed738
[8179e6b]: https://github.com/modm-io/modm/commit/8179e6b
[81b86be]: https://github.com/modm-io/modm/commit/81b86be
[821677b]: https://github.com/modm-io/modm/commit/821677b
[8230fef]: https://github.com/modm-io/modm/commit/8230fef
[82bc4a9]: https://github.com/modm-io/modm/commit/82bc4a9
[845840e]: https://github.com/modm-io/modm/commit/845840e
[850b554]: https://github.com/modm-io/modm/commit/850b554
[85eec34]: https://github.com/modm-io/modm/commit/85eec34
[874c8d6]: https://github.com/modm-io/modm/commit/874c8d6
[87b6405]: https://github.com/modm-io/modm/commit/87b6405
[8820d6b]: https://github.com/modm-io/modm/commit/8820d6b
[8896b5b]: https://github.com/modm-io/modm/commit/8896b5b
[897579e]: https://github.com/modm-io/modm/commit/897579e
[8a3a20b]: https://github.com/modm-io/modm/commit/8a3a20b
[8a924f9]: https://github.com/modm-io/modm/commit/8a924f9
[8bcbe25]: https://github.com/modm-io/modm/commit/8bcbe25
[8c322a2]: https://github.com/modm-io/modm/commit/8c322a2
[8ca2f35]: https://github.com/modm-io/modm/commit/8ca2f35
[901a3dc]: https://github.com/modm-io/modm/commit/901a3dc
[9036666]: https://github.com/modm-io/modm/commit/9036666
[90774be]: https://github.com/modm-io/modm/commit/90774be
[923f9c1]: https://github.com/modm-io/modm/commit/923f9c1
[9381fd0]: https://github.com/modm-io/modm/commit/9381fd0
[93bba13]: https://github.com/modm-io/modm/commit/93bba13
[94580b4]: https://github.com/modm-io/modm/commit/94580b4
[95713ee]: https://github.com/modm-io/modm/commit/95713ee
[972b74b]: https://github.com/modm-io/modm/commit/972b74b
[98a2483]: https://github.com/modm-io/modm/commit/98a2483
[98b1337]: https://github.com/modm-io/modm/commit/98b1337
[9b6aeee]: https://github.com/modm-io/modm/commit/9b6aeee
[9cbea26]: https://github.com/modm-io/modm/commit/9cbea26
[9d33843]: https://github.com/modm-io/modm/commit/9d33843
[9d8bbfa]: https://github.com/modm-io/modm/commit/9d8bbfa
[9e285db]: https://github.com/modm-io/modm/commit/9e285db
[9e50a16]: https://github.com/modm-io/modm/commit/9e50a16
[9e7ec34]: https://github.com/modm-io/modm/commit/9e7ec34
[a05cc62]: https://github.com/modm-io/modm/commit/a05cc62
[a105072]: https://github.com/modm-io/modm/commit/a105072
[a173bde]: https://github.com/modm-io/modm/commit/a173bde
[a371df6]: https://github.com/modm-io/modm/commit/a371df6
[a38feca]: https://github.com/modm-io/modm/commit/a38feca
[a607613]: https://github.com/modm-io/modm/commit/a607613
[a6b4186]: https://github.com/modm-io/modm/commit/a6b4186
[a771042]: https://github.com/modm-io/modm/commit/a771042
[a8edbe8]: https://github.com/modm-io/modm/commit/a8edbe8
[ab9bcee]: https://github.com/modm-io/modm/commit/ab9bcee
[ac46099]: https://github.com/modm-io/modm/commit/ac46099
[af2b352]: https://github.com/modm-io/modm/commit/af2b352
[afaea7f]: https://github.com/modm-io/modm/commit/afaea7f
[afbd533]: https://github.com/modm-io/modm/commit/afbd533
[afdb5ba]: https://github.com/modm-io/modm/commit/afdb5ba
[b010775]: https://github.com/modm-io/modm/commit/b010775
[b05df3e]: https://github.com/modm-io/modm/commit/b05df3e
[b153186]: https://github.com/modm-io/modm/commit/b153186
[b18385c]: https://github.com/modm-io/modm/commit/b18385c
[b1e5588]: https://github.com/modm-io/modm/commit/b1e5588
[b4eddeb]: https://github.com/modm-io/modm/commit/b4eddeb
[b570d07]: https://github.com/modm-io/modm/commit/b570d07
[b721551]: https://github.com/modm-io/modm/commit/b721551
[b78acd5]: https://github.com/modm-io/modm/commit/b78acd5
[b8648be]: https://github.com/modm-io/modm/commit/b8648be
[ba23833]: https://github.com/modm-io/modm/commit/ba23833
[ba61a34]: https://github.com/modm-io/modm/commit/ba61a34
[bfafcd3]: https://github.com/modm-io/modm/commit/bfafcd3
[c0a8c51]: https://github.com/modm-io/modm/commit/c0a8c51
[c148bf8]: https://github.com/modm-io/modm/commit/c148bf8
[c347f00]: https://github.com/modm-io/modm/commit/c347f00
[c38550a]: https://github.com/modm-io/modm/commit/c38550a
[c63a536]: https://github.com/modm-io/modm/commit/c63a536
[c7b35ca]: https://github.com/modm-io/modm/commit/c7b35ca
[c7bd876]: https://github.com/modm-io/modm/commit/c7bd876
[c7c4c57]: https://github.com/modm-io/modm/commit/c7c4c57
[c868f59]: https://github.com/modm-io/modm/commit/c868f59
[c93dd2c]: https://github.com/modm-io/modm/commit/c93dd2c
[c949daf]: https://github.com/modm-io/modm/commit/c949daf
[c9e5227]: https://github.com/modm-io/modm/commit/c9e5227
[cb82eec]: https://github.com/modm-io/modm/commit/cb82eec
[cbbf3f6]: https://github.com/modm-io/modm/commit/cbbf3f6
[cbce428]: https://github.com/modm-io/modm/commit/cbce428
[cc15b1a]: https://github.com/modm-io/modm/commit/cc15b1a
[cceb801]: https://github.com/modm-io/modm/commit/cceb801
[cf7769b]: https://github.com/modm-io/modm/commit/cf7769b
[d1938eb]: https://github.com/modm-io/modm/commit/d1938eb
[d21e991]: https://github.com/modm-io/modm/commit/d21e991
[d2d38a0]: https://github.com/modm-io/modm/commit/d2d38a0
[d3496a3]: https://github.com/modm-io/modm/commit/d3496a3
[d46c09d]: https://github.com/modm-io/modm/commit/d46c09d
[d772940]: https://github.com/modm-io/modm/commit/d772940
[d8be0a2]: https://github.com/modm-io/modm/commit/d8be0a2
[d982a85]: https://github.com/modm-io/modm/commit/d982a85
[dab6c79]: https://github.com/modm-io/modm/commit/dab6c79
[dbfd93b]: https://github.com/modm-io/modm/commit/dbfd93b
[dc56af2]: https://github.com/modm-io/modm/commit/dc56af2
[dee5ea2]: https://github.com/modm-io/modm/commit/dee5ea2
[df47974]: https://github.com/modm-io/modm/commit/df47974
[e0d1327]: https://github.com/modm-io/modm/commit/e0d1327
[e1d8a17]: https://github.com/modm-io/modm/commit/e1d8a17
[e233708]: https://github.com/modm-io/modm/commit/e233708
[e3ba913]: https://github.com/modm-io/modm/commit/e3ba913
[e3c0321]: https://github.com/modm-io/modm/commit/e3c0321
[e46e7df]: https://github.com/modm-io/modm/commit/e46e7df
[e4b1a4a]: https://github.com/modm-io/modm/commit/e4b1a4a
[eb2748e]: https://github.com/modm-io/modm/commit/eb2748e
[eba68a4]: https://github.com/modm-io/modm/commit/eba68a4
[eda224e]: https://github.com/modm-io/modm/commit/eda224e
[f4c7492]: https://github.com/modm-io/modm/commit/f4c7492
[f4d5d6c]: https://github.com/modm-io/modm/commit/f4d5d6c
[f5cdf6a]: https://github.com/modm-io/modm/commit/f5cdf6a
[f7dff92]: https://github.com/modm-io/modm/commit/f7dff92
[fb21f62]: https://github.com/modm-io/modm/commit/fb21f62
[fb2ff58]: https://github.com/modm-io/modm/commit/fb2ff58
[fd7b7a3]: https://github.com/modm-io/modm/commit/fd7b7a3
[fdbb45b]: https://github.com/modm-io/modm/commit/fdbb45b
[fe4cbc5]: https://github.com/modm-io/modm/commit/fe4cbc5
[feb1f3c]: https://github.com/modm-io/modm/commit/feb1f3c

<!--/links-->
