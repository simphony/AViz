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

TARGET = aviz

prefix = /usr/local
doc_prefix = 
docprefix =
docdirroot= $(prefix)/share
docdir = $(docdirroot)/$(TARGET)

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644


HTMLS = $(TARGET).html \
	index.html \
	node1.html \
	node2.html \
	node3.html \
	node4.html \
	node5.html \
	node6.html \
	node7.html \
	node8.html \
	node9.html \
	node10.html \
	node11.html \
	node12.html \
	node13.html \
	node14.html \
	node15.html \
	node16.html \
	node17.html \
	node18.html \
	node19.html \
	node20.html \
	node21.html \
	node22.html \
	node23.html \
	node24.html \
	node25.html \
	node26.html \
	node27.html \
	node28.html \
	node29.html \
	node30.html \
	node31.html \
	node32.html \
	node33.html \
	node34.html \
	node35.html \
	node36.html \
	node37.html \
	node38.html \
	node39.html \
	node40.html \
	node41.html \
	node42.html \
	node43.html \
	node44.html \
	node45.html \
	node46.html \
	node47.html \
	node48.html \
	node49.html \
	node50.html \
	node51.html \
	node52.html \
	node53.html \
	node54.html \
	node55.html \
	node56.html 

PSS = $(TARGET).ps

all:	$(TARGET).ps

$(TARGET).ps: 	$(TARGET).tex
	latex $(TARGET)
	latex $(TARGET)
	latex2html -no_subdir $(TARGET)
	dvips -f $(TARGET) > $(TARGET).ps

install: 
	$(INSTALL_DATA) -d $(docdir)
	$(INSTALL_DATA) $(PSS) $(docdir)
	$(INSTALL_DATA) $(HTMLS) $(docdir)
	chmod a+rx $(docdir)
	chmod a+rx $(docdirroot)

uninstall:
	rm -rf $(docdir)

# Standard GNU clean* targets:
maintainer-clean: clean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f *.1 

mostlyclean: clean

distclean: clean
	rm -f Makefile

clean:
	rm -f .*~ *~ *.dvi *.aux *.log *.toc

# Additional clean* targets:
realclean: distclean

# GNU autoconf(1) targets:
Makefile: Makefile.in $(top_srcdir)/config.status
	cd $(top_srcdir); $(SHELL) config.status
