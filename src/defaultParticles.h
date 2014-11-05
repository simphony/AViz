//
// Definition of default particle types, switch settings and colors
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

#ifndef ATOMDEF_H
#define ATOMDEF_H

#include "data.h"
#include "defaults.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void useDefaultParticleData( particleData * );
void setColorAndDefaults( particleData *, char *, int );
void setUnidentColor( particleData *, int );
void typeCopy( char *, char * );
void typeCopy( const char *, char * );
bool typeCmp( char *, char * );

#endif // ATOMDEF_H 
