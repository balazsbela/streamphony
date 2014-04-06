#!/bin/bash

srcdir=`dirname "$0"`
srcdir=`cd "$srcdir" && pwd`

(
cat << END
SOURCE_ROOT=$srcdir
BUILD_ROOT=$PWD
END
) > .qmake.cache
