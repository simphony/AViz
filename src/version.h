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

class QString;

namespace aviz {

/*! @class Version
    @brief Provides version information

    Version provides version information (major, minor, patch) and
    strings containing both version, author, and copyright information.
*/
class Version {
public:
    Version();

    int getMajor() const;
    int getMinor() const;
    int getPatch() const;

    //// get version as string (i.e "major.minor.patch")
    static QString getVersionString();

    //// get version information showed at program start
    static QString getStartVersionInfo();

    /// get version (and copyright) information
    static QString getVersionInfo();

private:
    int m_major;
    int m_minor;
    int m_patch;
};

}

#endif // VERSION_H

