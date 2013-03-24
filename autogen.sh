#!/bin/sh
glibtoolize --copy --force
aclocal -I .
automake --foreign -a -c
autoconf
