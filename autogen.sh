#!/bin/sh
libtoolize --copy --force
aclocal -I .
automake --foreign -a -c
autoconf
