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

At the core Saikodev relies on GCC built for cross-compiling Motorola 68000 code. Install marsdev for use of the toolchain.

Otherwise, it is expected that the typical essential build tools are available (host C compiler, Make, git, etc)

### Setting up a Project

Add Saikodev as a submodule to a new project:

```
git submodule add git@github.com:mikejmoffitt/saikodev
```

Copy the sample Makefile and linker script from the `templates/` directory for your platform of choice, or use it as a reference in creating your own.

From there, create your `src` directory.
