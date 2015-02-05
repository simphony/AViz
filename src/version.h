//
// Definition of version string
//

/**********************************************************************
Copyright (C) 2001 - 2003 Geri Wagner

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA

Contact address: Computational Physics Group, Dept. of Physics,
                 Technion. 32000 Haifa Israel
                 gery@tx.technion.ac.il
***********************************************************************/

#ifndef VERSION_H
#define VERSION_H

#define THIS_VERSION "AViz AtomicVizualization version 6.1\n"

#define START_STRING \
THIS_VERSION\
"From revision " GIT_REVISION "\n"\
"(C) 2001 - 2003\nComputational Physics Group Technion, Haifa Israel\n"\
"\n"\
"AViz comes with ABSOLUTELY NO WARRANTY; for details click on Help/License\n"\
"or start with the command line argument -license.   This is free software,\n"\
"and you are welcome to redistribute it under certain conditions; click on\n"\
"Help/Distribution or start with `aviz -distribution' for details."

#define VERSION_STRING \
THIS_VERSION\
"(C) 2001 - 2003 Computational Physics Group Israel Institute of Technology\n"\
"Technion, 32000 Haifa Israel\n"\
"Geri Wagner, Adham Hashibon"

#define VERSION_INFO_STRING \
THIS_VERSION\
"From revision " GIT_REVISION "\n"\
"(C) 2001 - 2003 Computational Physics Group Israel Institute of Technology\n"\
"Technion, 32000 Haifa Israel\n"\
"Geri Wagner, Adham Hashibon"

#endif // VERSION_H 

