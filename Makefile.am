SUBDIRS = src svgview include
ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libwxsvg.pc
EXTRA_DIST     = libwxsvg.pc.in

run: all
	$(MAKE) -C svgview run
