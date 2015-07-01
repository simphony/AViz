AViz
====

AViz is a tool to graphically display results of atomistic simulations. 
These results typically consist of large data sets of coordinates, where the
coordinates represent atoms or spins in three-dimensional space.

AViz lets you view the data sets, and you can easily produce stills and 
sequences of stills ready to be turned into animations. 

AViz is coded using the Qt toolkit and the OpenGL graphics library.  It 
runs on UNIX-systems on which Qt and OpenGL is installed.

AViz makes use of various SoQt widgets developed by Coin (www.coin3d.org), but 
does not use Coin's alternative implementation of the Inventor 3D graphics library.  

AViz is developed at the Computational Physics Group, Technion, Haifa (Israel).


Building
--------

Aviz requires the following:
  - Qt >= 4.8  (with opengl support)
  - X11
  - PNG development library (i.e. libpng12-dev)

To build avis, run::

  cd src
  qmake aviz.pro
  make

An executable binary called aviz will then be created

Documenation
------------

Documentation in HTML format and PostScript can be found in the doc directory

A man page can be installed.  See the makefile in the man directory for more information

License
-------

This software is available under GNU General Public License from http://phycomp.technion.ac.il/aviz.
More information, updates, and a User's Manual can be found at the same location.

Development
-----------

AViz was developed by Geri Wagner (gery.wagner@swissrisk.com>) and 
Adham Hashibon (adham.hashibon@iwm.fraunhofer.de) at the Computational Physics 
Group, Technion, Haifa Israel.  For questions and suggestions, and if 
you would like to contribute to the development, please contact 
Joan Adler at phr76ja@tx.technion.ac.il
