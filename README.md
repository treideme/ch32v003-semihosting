# CH32V003 Semihosting Example

This example demonstrates how to use the semihosting feature of the RISCV using
a customized openocd port from WCH's openocd sources.

 * See [my blog](https://reidemeister.com/blog/2023.08.25) for more information.

## Building OpenOCD
 * Clone my patched [openocd repository](https://github.com/treideme/openocd-hacks)
```bash
git clone --recursive git@github.com:treideme/openocd-hacks.git
cd openocd-hacks
./bootstrap
./configure --enable-wlinke \
	--disable-ch347 \
	--prefix=/opt/wch-riscv-openocd/ \
	--program-prefix=wch-riscv- \
	--disable-werror

make
sudo make install

# test installation
/opt/wch-riscv-openocd/bin/wch-riscv-openocd --version
```

## Building the example
 * Install WCH's toolchain
 * Make sure it is in path
 * Install meson and ninja

```bash
# Configure the build
meson setup --cross-file cross.ini -Dbuildtype=debug build
# Build
ninja -C build

# Try semihosting IO with target
/opt/wch-riscv-openocd/bin/wch-riscv-openocd -f flash.cfg
# Watch the user interaction
...
# Terminate [CTRL]+[C] when happy
```
