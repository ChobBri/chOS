#!/bin/bash

qemu-system-i386 -kernel kernel -append "terminal" -initrd ramdisk -soundhw sb16
