#!/bin/sh

SRC_DIR=$(pwd)
find ${SRC_DIR} -iname "*.[ch]" | xargs clang-format-6.0 -i