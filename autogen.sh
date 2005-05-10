#!/bin/sh

aclocal -I .
automake --foreign -a -c
autoconf
