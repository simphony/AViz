
#
# Change the following as appropriate.
#
Summary: AViz Atomic Vizualization software
Name: aviz
Version: 5.6
Release: 0
Source: aviz-5.6.tar.gz
Distribution: Haifa Technion Israel Institute of Technology
Packager: Computational Physics Group
Copyright: Computational Physics Group,  Haifa Technion Israel Institute of Technology
Vendor: Computational Physics Group
URL: http://phycomp.technion.ac.il/~aviz

#
# The setting of the Group tag should be picked from the list
# of values pre-defined by Red Hat in the file /usr/doc/rpm-<version>/groups.
# The value is similar to a directory path (e.g. "Networking/Daemons").
#
Group: Applications/Scientific

%description
The AViz Atomic Vizualization software is used to visualize data sets 
generated in atomistic simulations.  In such simulations one usually 
operates with a number of particles, each of which representing an atom. 
The result of the simulation typically consists of data files 
containing the coordinates of all the simulated atoms.  

AViz can be used to render the simlulated atoms as little spheres, cubes etc.  
The data format is extremely simple, essentially just an ASCII-list of 
atom coordinates.  AViz has features to represent spins and oriented 
molecules.  AViz makes it very easy to generate animations based on a 
sequence of coordinate files.

This distribution installs the application in /usr/local/bin, html doc 
files in in /usr/local/share/aviz, and a man page in /usr/local/man/man1.

For a source installation, the tmake and qt packages are required.

%prep
%setup

%build
make clean
make

%install
make install

%files
/usr/local/bin/aviz
/usr/local/man/man1/aviz.1
/usr/local/share/aviz/aviz.html
/usr/local/share/aviz/aviz.ps
/usr/local/share/aviz/index.html
/usr/local/share/aviz/node10.html
/usr/local/share/aviz/node11.html
/usr/local/share/aviz/node12.html
/usr/local/share/aviz/node13.html
/usr/local/share/aviz/node14.html
/usr/local/share/aviz/node15.html
/usr/local/share/aviz/node16.html
/usr/local/share/aviz/node17.html
/usr/local/share/aviz/node18.html
/usr/local/share/aviz/node19.html
/usr/local/share/aviz/node1.html
/usr/local/share/aviz/node20.html
/usr/local/share/aviz/node21.html
/usr/local/share/aviz/node22.html
/usr/local/share/aviz/node23.html
/usr/local/share/aviz/node24.html
/usr/local/share/aviz/node25.html
/usr/local/share/aviz/node26.html
/usr/local/share/aviz/node27.html
/usr/local/share/aviz/node28.html
/usr/local/share/aviz/node29.html
/usr/local/share/aviz/node2.html
/usr/local/share/aviz/node30.html
/usr/local/share/aviz/node31.html
/usr/local/share/aviz/node32.html
/usr/local/share/aviz/node33.html
/usr/local/share/aviz/node34.html
/usr/local/share/aviz/node35.html
/usr/local/share/aviz/node36.html
/usr/local/share/aviz/node37.html
/usr/local/share/aviz/node38.html
/usr/local/share/aviz/node39.html
/usr/local/share/aviz/node3.html
/usr/local/share/aviz/node40.html
/usr/local/share/aviz/node41.html
/usr/local/share/aviz/node42.html
/usr/local/share/aviz/node43.html
/usr/local/share/aviz/node44.html
/usr/local/share/aviz/node45.html
/usr/local/share/aviz/node46.html
/usr/local/share/aviz/node47.html
/usr/local/share/aviz/node48.html
/usr/local/share/aviz/node49.html
/usr/local/share/aviz/node4.html
/usr/local/share/aviz/node5.html
/usr/local/share/aviz/node50.html
/usr/local/share/aviz/node51.html
/usr/local/share/aviz/node52.html
/usr/local/share/aviz/node53.html
/usr/local/share/aviz/node54.html
/usr/local/share/aviz/node55.html
/usr/local/share/aviz/node56.html
/usr/local/share/aviz/node6.html
/usr/local/share/aviz/node7.html
/usr/local/share/aviz/node8.html
/usr/local/share/aviz/node9.html
