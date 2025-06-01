# SAIKODEV

Saikodev is a development kit that provides an embededded ASM/C/C++ environment for various 68000-based platforms. The name is derived from the "315" that leads many Sega IC part numbers.

Saikodev is derived from MDK, which originally only targeted the Mega Drive / Genesis platform.

## Platforms

Saikodev is supported for the following hardware:

* Sega Mega Drive / Genesis
* Sega System C / C2
* Sega System 16
* Sega System 18
* Sega System 24
* Capcom CPS2

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

In your project, just include `sai.h`.

`target.h` defines values for the `SAI_TARGET` variable, which is defined by the Make rules. Based on that variable, `sai.h` will include headers relevant to the chosen target platform. As a user, you should not need to include specific files beyond `sai.h`.

## Working with Assembly

The appeal of the Motorola 68000 CPU is that it is new enough to reasonably run code emitted from a compiler (C, even C++) while also having an ISA that is pleasant and relatively straightforward to work with when writing assembly code directly. Much of Saikodev's support code is written in assembly language to maintain a light footprint, but from the start it was a goal to maintain reasonable interoperability with the C and C++ programming languages.

As a result of this goal, the Saikodev header files may be included in either C or assembly sources, and will define symbols and structures that can be used from either context. All assembly code (with the `.a68` suffix) is run through the C preprocessor, as well as an `awk` script that formats more "normal" assembly in the way that GAS wants to consume. You may write hexidecimal numbers with the `$` prefix as is common in just about every other 68000 assembler, indicate registers without the `%` prefix, use `|` for bitwise OR operations, and use `;` for comments.

Include headers in assembly sources just as you would for C sources:

```
#include "sai/sai.h"
```

## Saikodev Directory Structure

Saikodev has some code that is common to all platforms and then a lot that is specific to others.


