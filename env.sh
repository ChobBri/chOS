#!/bin/bash

CURRENT_DIR="$(pwd)"
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd $SCRIPT_DIR/..
ARM_GNU_TOOLCHAIN=$(pwd)/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin
PATH=$PATH:$ARM_GNU_TOOLCHAIN

cd $CURRENT_DIR