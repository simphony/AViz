# Generated automatically from Makefile.in by configure.
# Makefile.in  -- AViz 
# 
#Copyright (C) 2001 - 2003  Geri Wagner
#
#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2
#of the License, or (at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
#
#Contact address: Computational Physics Group, Dept. of Physics,
#                 Technion. 32000 Haifa Israel
#		  gery@tx.technion.ac.il
#
########################################################################

CFLAGS = -O2
LDFLAGS =

########################################################################

SHELL = /bin/sh

srcdir = .
top_srcdir = .
aviz_srcdir = ./src

prefix = /usr/local
exec_prefix = ${prefix}
binprefix =
manprefix =

bindir = $(exec_prefix)/bin

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
TMAKE = /usr/bin/tmake

SUBDIRS = src man doc
INSTALLSUBDIRS = man doc

TARGET = aviz
BINTARGET = $(aviz_srcdir)/$(TARGET)

AVIZ_MAKEFILE = $(aviz_srcdir)/Makefile

$(BINTARGET): $(AVIZ_MAKEFILE)
	cd $(aviz_srcdir); make; cd ..

$(AVIZ_MAKEFILE):
	cd $(aviz_srcdir); $(TMAKE) aviz.pro > Makefile; cd ..

all: $(AVIZ_MAKEFILE) $(BINTARGET)
	for d in $(SUBDIRS); do	(cd $$d && $(MAKE) $@); done

install: $(AVIZ_MAKEFILE) $(BINTARGET)
	$(INSTALL_PROGRAM) $(aviz_srcdir)/$(TARGET) $(bindir)
	chmod a+rx $(bindir)/$(TARGET)
	for d in $(INSTALLSUBDIRS); do	(cd $$d && $(MAKE) $@); done

uninstall:
	rm -f $(bindir)/$(TARGET)
	for d in $(INSTALLSUBDIRS); do	(cd $$d && $(MAKE) $@); done

# Standard GNU clean* targets:
maintainer-clean: clean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	for d in $(SUBDIRS); do	(cd $$d && $(MAKE) $@); done

mostlyclean: clean
	for d in $(SUBDIRS); do	(cd $$d && $(MAKE) $@); done

distclean: clean
	for d in $(SUBDIRS); do	(cd $$d && $(MAKE) $@); done
	rm -f Makefile config.cache config.status

clean:
	for d in $(SUBDIRS); do	(cd $$d && $(MAKE) $@); done
	rm -f .*~ *~ config.log
	rm -f $(aviz_srcdir)/Makefile

realclean: clean

# GNU autoconf(1) targets:
Makefile: Makefile.in config.status
	$(SHELL) config.status
config.status: configure
	$(SHELL) config.status --recheck
configure: configure.in
	cd $(srcdir); autoconf
