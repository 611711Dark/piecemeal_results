# Makefile for Kernel module  projects
# for cross-compilation on x86 and native compilation on arm
# the compilation mode is auto-detected from the current machine architecture
# if x86_64, cross-compilation is infered and the beaglebone toolchain used
# if other, native compilation is infered and the native toolchain is used

arch=$(shell uname -m)

kversion=4.14.108-ti-rt-r113
linaroversion=6.5.0-2018.12-x86_64
ifeq ($(arch),x86_64)
    ## Values for the toolchain in cross-compilation mode (x86->arm)
	kdir=/opt/beaglebone/linux-${kversion}
	toolc=${kdir}/gcc-linaro-${linaroversion}_arm-linux-gnueabihf/bin
	export CROSS_COMPILE=${toolc}/arm-linux-gnueabihf-
	export ARCH:=arm
else
	## Values for the toolchain in native compilation mode (arm->arm)
	kdir=/usr/src/linux-headers-${kversion}
	export CROSS_COMPILE=
endif

# Name of the kernel module
modname=hellodev
obj-m:=$(modname).o

# Compile the module using the kernel source Makefile
$(modname).ko: $(modname).c
	make -C $(kdir) M=$(PWD) modules

# Remove generated files
clean:
	make -C $(kdir)  M=$(PWD) clean

# The copy target is only useful when cross-compiling
ifeq ($(arch),x86_64)
copy: $(modname).ko
	scp $(modname).ko beaglebone:
endif

.PHONY: clean copy
