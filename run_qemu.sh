#!/bin/bash

qemu-system-i386 -kernel kernel -append "video" -initrd ramdisk
