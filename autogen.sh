#!/bin/sh
libtoolize --copy --force || exit 1
aclocal -I .
automake --foreign -a -c
autoconf
