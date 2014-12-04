//
// Declaration of functions used in file handling
//

/**********************************************************************
Copyright (C) 2001 - 2003  Geri Wagner

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

#ifndef FILE_H
#define FILE_H

#include "data.h"
#include "defaultParticles.h"
#include "defaults.h"
#include "version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void checkSuffix( const char *, const char * );
bool fileExists( const char * );
bool openCoordinateFunction( const char *, aggregateData * );
bool openFileListFunction( const char *, fileList * );
bool generateTrackDataFunction( fileList *, aggregateData *, trackData * );
bool saveViewParamFunction( const char *, viewParam * );
bool openViewParamFunction( const char *, viewParam * );
bool openParticleDataFunction( const char *, particleData * );
bool saveParticleDataFunction( const char *, particleData * );

#endif // FILE_H
