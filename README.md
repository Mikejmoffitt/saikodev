# SaikoDev

Saikodev is a development kit that provides an embededded ASM/C/C++ environment for various 68000-based platforms. The name is derived from the "315" that leads many Sega IC part numbers.

Saikodev is derived from MDK, which originally only targeted the Mega Drive / Genesis platform.

## Objectives

### Non-Crusty
Aside from the compiler, there should be efficient and robust boot code, clean interoperability between C, C++, and Assembler code, build rules and linker scripts that can be included and used as-is, and good sample projects that show how to integrate it. No build artifacts in the root directory, no hokey "build.bat" build system, no piles of magic numbers making things work.

### Quick Start
I would like to remove as much friction as possible when starting a new game project for old harware. I have spent a considerable amount of time reimplementing similar yet different build environments, support functions, and basic I/O code for home console and arcade platforms. I can then begin to focus on how I will prepare data for use, such as graphics and sound, followed by stage data or whatever.

As so much is similar between these, I'd like to get my code in one place and share it piecewise such that I can more or less select a target and at least get my basic "Hello World" up and running without much thought at all.

### Assembly Integration
Much of my source has C headers, with some inline functions, and then assembler code for implementations and memory definitions. Towards the sisyphean goal of cleaner software engineering, I want to avoid repeating myself whenever possible. Nearly all of my headers are bilingual, in that they can be directly included (as in #include) in either C or Assembler source files. I run the C Preprocessor on all assembler sources for this purpose. This means that #define statements, macros, and even symbol declarations come from the same file for both. When reasonable I also have struct definitions so that the assembler source is legible and more robust against refactor down the line.

### Assembly Expectations
The GCC assembler has some good points, but it brings with it one very strong merit above all else: it emits linkable object files, with sections, relocation and all that. Other than that, we can do parallel assembly, declare static variabels that don't pollute the namespace, and use the C preprocessor - there are good things about it.
However, at the end of the day, its main job is to consume the output from a compiler, and hand-written code seems to be more of an afterthought. Specifically, on the 68k side, there are a few chief frustrations for me:

* Comments are either C style (`/* */`), C++ style (`//`), or pound (`#`). A semicolon (`;`) is a typical comment character and I prefer it (though I have seen `*` and `#`)
* Registers require a % prefix
* Hexidecimal numbers only accept the C "0x" prefix, instead of the very common "$"
* Local labels are limited to numbered names
* Harsh limits on pointer math outside of the basic four operations for symbols not located in the same compilation unit (linker limitation)

Not all of these points can be resolved, but some can be.

Register prefix requirements can be disabled by an option passed to the assembler. Local labels are an unfortunate loss but the privatization of named labels somewhat makes up for it with smaller compilation units. The pointer math limits are horrible but again if the code is designed with this in mind we can squeeze by.

As for comments and the hexidecimal prefix, I handle these with a crude `gawk` command to preprocess the assembler source before passing it on.

### Light on the Processor and the Programmer
I want my support code to be convenient, but I don't want it to become a burden to the software it supports. My boot code is reasonably small, and the regular frame update code that runs the small functions specific to each harwdare are hand-written in assembly. I've tried to cleanly implement functionality that everything will need (input reading, sprite transfer/page flip, palette mangement) so that repeated implementation won't be needed, and I hope these implementations are compact and flexible enough for most users.

### Make Tools Available
Even if the hardware is booting, it's frustrating to find that convenient tools may not exist for painless conversion of graphics data, or that there are no sound engines for the audio hardware. While games generally require bespoke tooling, whenever I've made something that's useful enough to be used more than once, I'd prefer to integrate it or otherwise make it easy to use for other projects sharing the same framework (see Velella)

### Don't Overdo It
It's tempting to design not only a development "kit" but instead a whole *engine*, promising to run the same code with the same assets across many platforms. When working with old hardware like this, it is necessary to design with the constraints and strengths of the hardware in mind. The supported platforms vary greatly in their graphical capabilities and sound hardware. Working with a lowest common denominator could allow for a universal engine to be viable, but I think that would hamper the quality of anything made that targets it. In addition, the convenience of a generalized engine comes with consequences in complexity, and there is no room for that on hardware like this.

As a result, while I want to write support code to make it easy to talk to the hardware, I am firmly not attempting to make any sort of universal sprite or background code that promises to work on all of the platforms below. I'd rather write usable platform-specific code and aim to make the interfaces similar if it helps, and stop at that point.

## Comparisons against Similar Devkits
Saikodev is not the only m68k-gcc centered development option available. Popular alternatives include

* [SGDK](https://github.com/Stephane-D/SGDK)
* [Marsdev](https://github.com/andwn/marsdev)
* [MDK](https://github.com/mikejmoffitt/MDK) (to be deprecated)
* [SGCC](https://gendev.spritesmind.net/page-sgcc.html)
* Plain old assembler

I will compare these options with Saikodev while trying to remain objective and kind with regards to the (free, underappreciated) efforts of others.

Common points with most of these is the presence of a C/C++ compiler, sample code, and usability on relatively modern computers.

SGDK is feature-rich with support out of the box for semi-automatic conversion of graphical and audio assets, as well as support functions to cover ground for a lot of potential use cases. In exchange for that it may be a little heavier in size and speed at some points. It is often distributed in binary format as the majority of users are on Windows and expect to download an archive that's good to go; a much older version of GCC is often employed.

Marsdev provides boot code and sample hardware interaction code not only for Megadrive but also for 32X (and recently, a little X68000). If SGDK is full of features and is one of the larger options, then Marsdev is rather one of the smaller and more barebones options. Like Saikodev it expects you to build GCC (using the convenient submodule maintained by the author) or bring your own copy, and as such the toolchain itself will be up to date.

SGCC is DOS based and the tooling is distributed in binary format. I do not have experience using this and with the other options available I have not felt compelled to try. Some of the files are older than I am.

MDK is an earlier attempt of mine towards goals similar to Saikodev, but I have elected to make a clean start and move away from it.

## Platforms

Saikodev is supported for the following hardware:

* Sega Mega Drive / Genesis
* Sega System C / C2
* Sega System 16
* Sega System 18
* Sega System 24
* Capcom CPS2
* Atlus 013/038

## Environment and Usage

Saikodev was designed for use with Linux, but ought to work without particular issues on MacOS or Windows (via WSL).

### Prerequisites

It is expected that the typical essential build tools are available (host C compiler, Make, git, etc)

On debian and other similar distributions, these may be installed with the following command (with install privileges)

```
apt install build-essential git make texinfo wget gawk
```

### Building Motorola 68000 Toolchain

At the core Saikodev relies on m68k-elf-gcc for cross-compiling Motorola 68000 code. [m68k-gcc-toolchain](https://github.com/andwn/m68k-gcc-toolchain) simplifies compiling and installing GCC for M68000 so it is the recommended process.

This only must be done once, and the cloned repository may be deleted afterwards.

```
git clone git@github.com:andwn/m68k-gcc-toolchain
cd m68k-gcc-toolchain
make -j$(nproc)
sudo make install
```

By default this will install the toolchain to the default location at `/opt/toolchains/m68k-elf`, and the sample Makefiles included with Saikodev assume this is the case. If you want another directory, read the toolchain install script's documentation before installing and adjust the project Makefiles accordingly to point at the alternate location.

## Using in a Project

### Submodule

Add Saikodev as a submodule to a new project:

```
git submodule add git@github.com:mikejmoffitt/saikodev
```

By doing it this way, it is easy to update Saikodev to a newer version, and reduce unnecessary duplication of files. When cloning your project's repository or doing a `pull` I recommend adding `--recurse-submodules`:

```
git clone <git repo url> --recurse-submodules
```

### Project Structure

Saikodev has sample projects for various platforms that may be used as a template. Ultimately the different components are:

* Project Makefile
* Sourced platform sources, rules and definitions
* Linker script

The project's Makefile can be copied from one of the sample directories. It sources a platform-specific file containing Make rules that define the target platform and sources necessary boot and support code. It then points to a linker script for which default templates are provided. You can either point to the existing linker script as recommended, or copy it and modify it to suit your needs as part of your project itself. Finally, the Makefile defines sources and the various build rules necessary for your project.

### In the Source

In your project, just include `sai/sai.h`.

`target.h` defines values for the `SAI_TARGET` variable, which is defined by the Make rules. Based on that variable, `sai.h` will include headers relevant to the chosen target platform. As a user, you should not need to include specific files beyond `sai.h`.

## Working with Assembly

The appeal of the Motorola 68000 CPU is that it is new enough to reasonably run code emitted from a compiler (C, even C++) while also having an ISA that is pleasant and relatively straightforward to work with when writing assembly code directly. Much of Saikodev's support code is written in assembly language to maintain a light footprint, but from the start it was a goal to maintain reasonable interoperability with the C and C++ programming languages.

As a result of this goal, the Saikodev header files may be included in either C or assembly sources, and will define symbols and structures that can be used from either context. All assembly code (with the `.a68` suffix) is run through the C preprocessor, as well as an `awk` script that formats more "normal" assembly in the way that GAS wants to consume. You may write hexidecimal numbers with the `$` prefix as is common in just about every other 68000 assembler, indicate registers without the `%` prefix, use `|` for bitwise OR operations, and use `;` for comments.

Include headers in assembly sources just as you would for C sources:

```
#include "sai/sai.h"
```

## Saikodev Directory Structure

Saikodev has some code that is common to all platforms and then a lot that is specific to others. The code lives in `sai/`.

### Common

#### `sai`

Interactions with `sai.h` generally come in the form of 

1) Calling `sai_init()` in `main()`, and
2) Calling `sai_finish()` after a frame is done to wait for vblank and trigger the processing associated with the end of a frame.

There is also `sai_irq_vbl_wait()` which will allow the user to wait for vertical blank manually before entering the rest of `sai_finish()`.

`sai_init()` clears memories of various peripherals besides the main CPU, sets up registers to sensible defaults, and initializes system data structures (memory queues, inputs, etc). What is initialized varies by platform.

`sai_finish()` differs in implementation based on the target platform, but generally follows this sort of structure:

* Wait for the end of active display (vertical blank)
* Process queued DMA commands
* Terminate sprite list / flip display lists
* Update / copy palette data
* Update I/O registers (watchdog, etc)
* Read inputs, calculate pos/neg edges

The exact ordering of some (namely the relationship between blanking, DMA, and palettes) varies by the needs of the platform. For example, CPS/CPS2 already has palette DMA, so the palette queue is processed *before* blanking, while on Sega 16 and Atlus hardware palette writes are immediate and thus the palette queue is processed right when blanking begins.

#### `input`

Input polling is more or less the same for all platforms, with the actual buttons and bit values available differing slightly. `input.h` exposes the functions involved in input polling, as well as the global state of the input data. An array of SaiInput structs is available, with one element per available player / seat. Reading `g_sai_in[0].pos` gets you a bitfield containing positive edges (button pushes) for player 1, for example.
Generally reading from the `g_sai_in` array is the only meaningful interaction the programmer needs to have with `input.h` as the functions are called from `sai.a68`.

The sister file `input_bits.h` defines the bitfields for the buttons / switches that are available. While the available buttons and names change a little based on the target platform, generally inputs like `SAI_BTN_UP` or `SAI_BTN_START` will be there for all of them. In the case of Megadrive, where there is a six-button pad with named buttons, we have  ABC/XYZ names available; on the arcade platforms I just count up from A. MODE is only on Megadrive; COIN is only on arcade platforms, etc. The bit values also differ based on what was efficient to implement based on the hardware, but if you stick to the named values this ought to not cause any trouble.

#### `irq`

All platforms have a vertical blank interrupt, and some platforms offer additional ones (raster interrupt, timers, etc). Callbacks can be registered for each type, with or without register protection. Vertical blank waiting is handled by a macro exposed in `irq.h` that handles clearing a wait flag, but otherwise irq handler routines are dispatched in the same way. Register protection is enabled by using the upper (physically disused) byte of the address as a field for flags.

#### `macro`

A few really basic macros are exposed here that I want to be available in both C and Assembler contexts. A few are for improving code legibility and reducing opportunities for bugs. In addition, some assembler macros support the boot code, which does not make any use of RAM until the C runtime init begins, opting instead for a limited register-based return structure.

#### `memmap`

The memory map definitions for the target platform are included by `memmap.h`.

#### `palcmd`
Palette commands are batched here. Both copy and fill commands are supported. This is for platforms that do not have DMA functionality, where we'd like to remain in control of where in the raster palette data is actually updated.

#### `target`
Definitions for the different target platform names are proided here.

### Platform-Specific

Platform code lives in subdirectories based on the name; For example, Megadrive code is in `md/`. I will outline here modules that reoccur between different platforms.

### `Sources.mk`

The source files needed for the platform specified are defined here. There are times when one platform might bring in code that is shared with another, as is the case with CPS and CPS2, for example.

#### `boot`

Every platform's entry point is defined here. A register-based single-level call return structure is maintained to allow the boot code to use subroutines and remain relatively clean without actually using work RAM. This is to allow for future expansion of more involved startup hardware tests, which can be beneficial for testing and maintaining more complex arcade hardware.

The boot code does a "minimal" init of the peripherals of interest, performs a basic work RAM check, then jumps to the C runtime startup.

### `hw`

The memory map is provided by way of defines that set the base address for peripherals and memory regions. More precise locations within those base values are generally defined in the headers that use them rather than in the base memory map itself. This way, some modules can be reused between hardware variants without modification within. System 18, Megadrive, and C/C2 share the same VDP code.

### `io`

Inputs (and limited non-audiovisual output) is managed in `io.h`, providing at minimum an init function and a function to update input sources. This is what writes to the data structures exposed by `input.h`.

### `irq`

The callbacks for the different interrupts offered are exposed here. You may simply set them to a value of your choice, but please mind the conventions described in `sai/irq.h`.

### `pal`

Macros for defining palette entry words as R, G, B elements are exposed, as well as the memory set aside as a palette buffer (if relevant to the platform). Inline functions are present for manipulating palette entries or copying palette "rows" of data. On some platforms, these functions modify a palette buffer in memory and schedule a transfer to color RAM at vertical blank, while others schedule a DMA. Through use of `sai_pal_load()` the usage can remain virtually unchanged no matter which platform is being used.

### Linker Scripts

Linker scripts live in `sai/ld`, rather than in hardware-specific directories. In the case of platforms like CPS and MD there is no meaningful boundary between "hardware" and "platform", but for arcade chipsets that were used across many boards with small changes, it is necessary to make this distinction. ESP Ra.De. and Guwange both use the Atlus chipset, but the memory map is not the same, and this matters to the linker script more than any other part of the project.



