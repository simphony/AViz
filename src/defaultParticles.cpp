//
// Implementation of functions used in setting of default 
// particle types and colors
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

#include "defaultParticles.h"

#include "defaults.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// Set particle defaults
void useDefaultParticleData( particleData * pd )
{
	char * buffer = (char *)malloc( BUFSIZ );

	(*pd).numberOfParticleTypes = 0;	

	// Insert the definitions
	int i=0;
#ifdef aluminium
	sprintf( buffer, aluminium ); setColorAndDefaults( pd, buffer, i++ ); 
#endif
#ifdef carbon
	sprintf( buffer, carbon); setColorAndDefaults( pd, buffer, i++ );
#endif
#ifdef copper
	sprintf( buffer, copper); setColorAndDefaults( pd, buffer, i++ );
#endif
#ifdef germanium
	sprintf( buffer, germanium); setColorAndDefaults( pd, buffer, i++ );
#endif
#ifdef gold
	sprintf( buffer, gold); setColorAndDefaults( pd, buffer, i++ );
#endif
#ifdef helium
	sprintf( buffer, helium); setColorAndDefaults( pd, buffer, i++ );
#endif
#ifdef hydrogen
	sprintf( buffer, hydrogen); setColorAndDefaults( pd, buffer, i++ );
#endif
#ifdef lead
	sprintf( buffer, lead); setColorAndDefaults( pd, buffer, i++ );
#endif
#ifdef oxygen
	sprintf( buffer, oxygen); setColorAndDefaults( pd, buffer, i++ );
#endif
#ifdef silicium
	sprintf( buffer, silicium); setColorAndDefaults( pd, buffer, i++ );
#endif
#ifdef silver
	sprintf( buffer, silver); setColorAndDefaults( pd, buffer, i++ );
#endif
#ifdef vanadium
	sprintf( buffer, vanadium); setColorAndDefaults( pd, buffer, i++ );
#endif

	free(buffer);

	// Assign an emergency color for cases in which 
	// particle types cannot be identified
	setUnidentColor( pd, i++ );
}


// Set a default value
void setColorAndDefaults( particleData * pd, char * buffer, int index )
{
	int i;
	char type[3];
	float red, green, blue;
	char * ptr;

	// This is an ugly hack... extract the data out of 
	// the string
	type[0] = buffer[0];
	type[1] = buffer[1]; 
	type[2] = 0;

	ptr = strstr( buffer, "." );
	i=0;
	while (buffer+i<ptr-1) {
		buffer[i]=' ';	
		i++;
	}
	red = (float)atof(buffer);

	*ptr = ' ';
	ptr = strstr( buffer, "." );
	i=0;
	while (buffer+i<ptr-1) {
		buffer[i]=' ';	
		i++;
	}
	green = (float)atof(buffer);

	*ptr = ' ';
	ptr = strstr( buffer, "." );
	i=0;
	while (buffer+i<ptr-1) {
		buffer[i]=' ';	
		i++;
	}
	blue = (float)atof(buffer);

	// Now add the entry
	if (index < ATOMS_MAX-1) {

		// Update the number of particle entries
		(*pd).numberOfParticleTypes = index+1;

		// Set type string
		typeCopy( type, (char *)&(*pd).type[index] );

		// Set the default particle color 
		(*pd).particleColors[index].red = red;
		(*pd).particleColors[index].green = green;
		(*pd).particleColors[index].blue = blue;

		// Use defaults for the color ramp: light blue for the minimum,
		// orange for the maximum
		(*pd).particleColors[index].redMin = 0.0;
		(*pd).particleColors[index].greenMin = 0.5;
		(*pd).particleColors[index].blueMin = 1.0;
		(*pd).particleColors[index].redMax = 1.0;
		(*pd).particleColors[index].greenMax = 0.3;
		(*pd).particleColors[index].blueMax = 0.0;

		// Use defaults for the second and third type color
		(*pd).particleColors[index].redTwo = 1.0;
		(*pd).particleColors[index].greenTwo = 1.0;
		(*pd).particleColors[index].blueTwo = 0.0;
		(*pd).particleColors[index].redThree = 0.0;
		(*pd).particleColors[index].greenThree = 1.0;
		(*pd).particleColors[index].blueThree = 0.0;
	
		// Use defaults for the remaining settings
        (*pd).showParticle[index] = true;
        (*pd).showTrack[index] = true;
		(*pd).colorCrit[index] = TYPE;
		(*pd).colorCritPos[index] = XPOS;
		(*pd).colorCritProp[index] = PROP1;
		(*pd).colorCritCodeRed[index] = PROP1;
		(*pd).colorCritCodeGreen[index] = PROP2;
		(*pd).colorCritCodeBlue[index] = PROP3;
		(*pd).relSize[index] = REGULAR;
		(*pd).line[index].fThickness = THIN;
		(*pd).line[index].fEmission = 0;
		(*pd).line[index].typeColorNumber = 1;
        (*pd).line[index].fAntiAlias = true;

		// Assume zero bonding as default
		(*pd).particleBonds[index].numberOfNeighborBonds = 0;
		for (i=0;i<BONDS_MAX;i++) {
            (*pd).particleBonds[index].bShowBond[i] = false;
			(*pd).particleBonds[index].bColor[i] = RED;
			(*pd).particleBonds[index].bThickness[i] = LINE;
			(*pd).particleBonds[index].bEmission[i] = 0.0;
			(*pd).particleBonds[index].bMinLength[i] = (float)BOND_MINLENGTH;
			(*pd).particleBonds[index].bMaxLength[i] = (float)BOND_MAXLENGTH;
            (*pd).particleBonds[index].bAntiAlias[i] = true;
            (*pd).particleBonds[index].bSeqBond[i] = false;
		}		
	}
	else {
		printf("The maximum number of particle type entries (%d) is exceeded -- recompile\n", ATOMS_MAX);
	}	
	return;
}


// Set a default for unidentifiable particles; this is 
// the last entry in the list of particle colors
void setUnidentColor( particleData * pd, int index )
{
	char * buffer = (char *)malloc( BUFSIZ );

// This is the last color in the list, it is used if the
// identification of particle types fails
#ifdef unidentified
	// Set the entry now
	sprintf( buffer, unidentified); 
	setColorAndDefaults( pd, buffer, index );
#endif
	free(buffer);
}

// Copy a type string
void typeCopy( const char * typeSrc, char * typeDest )
{
	if (typeSrc && typeDest) {
        	typeDest[0] = typeSrc[0];
	        typeDest[1] = typeSrc[1];
		typeDest[2] = 0;
	}
}


// Compare two type strings denoting particle types -- 
// these can contain either one or two chars
bool typeCmp(const char * type, const char * referenceType )
{
	if (!type || !referenceType) 
        return false;

	if (type[0] != referenceType[0]) 
        return false;

	if (type[1] != referenceType[1]) {
		if (type[1] == SPACE_CHAR && referenceType[1] == 0)
            return true;
		if (type[1] == 0 && referenceType[1] == SPACE_CHAR)
            return true;
        return false;
	}
		
    return true;
}
