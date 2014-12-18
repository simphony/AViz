//
// Implementation of functions used in memory management
//

/**********************************************************************
Copyright (C) 2001  Geri Wagner

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

#include "memoryFunctions.h"
#include <cstdlib>

// Allocate memory 
void allocateParticleData( particleData * pd )
{
	(*pd).type = (tag *)malloc( ATOMS_MAX*sizeof(tag) );
	(*pd).showParticle = (bool *)malloc( ATOMS_MAX*sizeof(bool) );
	(*pd).showTrack = (bool *)malloc( ATOMS_MAX*sizeof(bool) );
	(*pd).colorCrit = (colorCriterion *)malloc( ATOMS_MAX*sizeof(colorCriterion) );
	(*pd).colorCritPos = (colorCriterionPos *)malloc( ATOMS_MAX*sizeof(colorCriterionPos) );
	(*pd).colorCritProp = (colorCriterionColumn *)malloc( ATOMS_MAX*sizeof(colorCriterionColumn) );
	(*pd).colorCritCodeRed = (colorCriterionColumn *)malloc( ATOMS_MAX*sizeof(colorCriterionColumn) );
	(*pd).colorCritCodeGreen = (colorCriterionColumn *)malloc( ATOMS_MAX*sizeof(colorCriterionColumn) );
	(*pd).colorCritCodeBlue = (colorCriterionColumn *)malloc( ATOMS_MAX*sizeof(colorCriterionColumn) );
	(*pd).particleColors = (particleColor *)malloc( ATOMS_MAX*sizeof(particleColor) );
	(*pd).particleBonds = (particleBond *)malloc( ATOMS_MAX*sizeof(particleBond) );
	(*pd).line = (lineType *)malloc( ATOMS_MAX*sizeof(lineType) );
	(*pd).relSize = (relativeSize *)malloc( ATOMS_MAX*sizeof(relativeSize) );
}


// Allocate memory
void allocateAggregateData( aggregateData * ad ) 
{
	// Allocate memory for the particle positions 
	(*ad).particles = (particle *)malloc( (*ad).numberOfParticles*sizeof(particle) );
    (*ad).haveMemoryAllocated = true;
}

// Free memory
void freeAggregateData( aggregateData * ad ) 
{
	if ((*ad).haveMemoryAllocated) {
		free((*ad).particles);
	}
}


// Free memory
void freeTrackData( trackData * td ) 
{
	if ((*td).haveMemoryAllocated) {
		for (int i=0;i<(*td).numberOfTracks;i++) {
			free((*td).x[i]);
			free((*td).y[i]);
			free((*td).z[i]);
		}
	
		free((*td).type);
		free((*td).x);
		free((*td).y);
		free((*td).z);
	}
}
