# Generated automatically from Makefile.in by configure.
# Makefile.in  -- AViz 
# 
#Copyright (C) 2001  Geri Wagner
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
top_srcdir = ..
aviz_srcdir = ../src

prefix = /usr/local
man_prefix = 
manprefix =
mandir = $(prefix)/man/man1

TARGET = aviz

POD2MAN = /usr/bin/pod2man
OPTION = -center " "

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

PODS = $(TARGET).pod
MANS = $(TARGET).1

all:	$(MANS)

$(MANS): $(PODS) 
	$(POD2MAN) $(OPTION) $(PODS) > $(MANS)

install: $(MANS) 
	$(INSTALL_DATA) $(MANS) $(mandir)

uninstall:
	cd $(mandir) && rm -f $(MANS)

# Standard GNU clean* targets:
maintainer-clean: clean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f *.1 

mostlyclean: clean

distclean: clean
	rm -f Makefile

clean:
	rm -f .*~ *~

# Additional clean* targets:
realclean: distclean

# GNU autoconf(1) targets:
Makefile: Makefile.in $(top_srcdir)/config.status
	cd $(top_srcdir); $(SHELL) config.status
