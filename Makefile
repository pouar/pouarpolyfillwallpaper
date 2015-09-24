# example Makefile
#
SHELL = /bin/sh
INSTALL = /usr/bin/install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

DIRS = pouarquick pouarwallpaper
# the sets of directories to do various things in
BUILDDIRS = $(DIRS:%=build-%)
INSTALLDIRS = $(DIRS:%=install-%)
UNINSTALLDIRS = $(DIRS:%=uninstall-%)
CLEANDIRS = $(DIRS:%=clean-%)
DISTCLEANDIRS = $(DIRS:%=distclean-%)
TESTDIRS = $(DIRS:%=test-%)

all: pouarquick/Makefile $(BUILDDIRS)
pouarquick/Makefile:
	cd pouarquick;qmake-qt5
$(DIRS): pouarquick/Makefile $(BUILDDIRS)
$(BUILDDIRS): pouarquick/Makefile
	$(MAKE) -C $(@:build-%=%)


install: $(INSTALLDIRS) all
$(INSTALLDIRS):
	$(MAKE) -C $(@:install-%=%) install

test: $(TESTDIRS) all
$(TESTDIRS): 
	$(MAKE) -C $(@:test-%=%) test

clean: $(CLEANDIRS)
$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean
distclean: $(DISTCLEANDIRS)
$(DISTCLEANDIRS): 
	$(MAKE) -C $(@:distclean-%=%) distclean

uninstall: $(UNINSTALLDIRS)
$(UNINSTALLDIRS): 
	$(MAKE) -C $(@:uninstall-%=%) uninstall

.PHONY: subdirs $(DIRS)
.PHONY: subdirs $(BUILDDIRS)
.PHONY: subdirs $(INSTALLDIRS)
.PHONY: subdirs $(UNINSTALLDIRS)
.PHONY: subdirs $(TESTDIRS)
.PHONY: subdirs $(CLEANDIRS)
.PHONY: subdirs $(DISTCLEANDIRS)
.PHONY: all

