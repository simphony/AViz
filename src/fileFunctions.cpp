//
// Implementation of functions used in file handling
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

#include "fileFunctions.h"

// Check the suffix in a file name
void checkSuffix( const char * filename, const char * suffix )
{
	char c = '.';
	char * buffer = (char *)malloc(BUFSIZ);

	// Append the suffix if not present
        const char * ptr = strchr( filename, c );
        if (!ptr)  
		sprintf( buffer, "%s.%s", (char *)filename, (char *)suffix );
        else 
		sprintf( buffer, "%s", (char *)filename );

	// Copy the improved string back to the filename string
	sprintf( (char *)filename, "%s", (char *)buffer);

	free(buffer);

}


// Check if a file exists
bool fileExists( const char * filename )
{
	struct stat * buf = (struct stat *)malloc( sizeof(struct stat) );
	bool exists = FALSE;

	  if (!stat( (const char *)filename, buf )) {
		  // File exists...
		  exists = TRUE;
	}

	free( buf );

	return exists;
}


// Read a coordinate file in XYZ format.  There can be up to 
// eight additional property columns per line
bool openCoordinateFunction( const char * filename, aggregateData * ad )
{
	char type[3];
	float thisRead[11];	
	char * buffer = (char*)malloc(BUFSIZ);

        // Open the file
        if (FILE * in = fopen( (char *)filename, "r" )) {
		// Register the filename
		sprintf( (*ad).filename, "%s", filename );
	
		// Read the number of particles
		fgets( buffer, BUFSIZ, in );
		(*ad).numberOfParticles = atoi(buffer);

		// Read the ID string 
		fgets( (*ad).IDstring, BUFSIZ, in );

		// Free particle data memory
		freeAggregateData( ad );

		// Allocate memory for the particle positions 
		allocateAggregateData( ad );

		// Read the atom positions and properties
		int i = 0;
		while (!feof(in)) {

			fgets( buffer, BUFSIZ, in );

			if (i < (*ad).numberOfParticles) {

				// Replace all tabs by a blank
				int k = 0;
				while (buffer[k] != 0) {
					if (buffer[k] == '\t')
						buffer[k] = ' ';
					k++;
				}	
				// Get the atom type
				k = 0;
				while (buffer[k] == ' ') k++;
				type[0] = buffer[0+k]; buffer[0+k] = ' ';
				type[1] = buffer[1+k]; buffer[1+k] = ' ';
				type[2] = 0;
	
				// Read the rest
				thisRead[0] = atof( strtok(buffer+2, " ") );
				
				for (int j=1;j<11;j++) {
					char * ptr = strtok(NULL, " ");
					if (ptr) {
						thisRead[j] = atof( ptr );
					}
					else {
						thisRead[j] = 0.0;
					}
				}
	
				sprintf( (char *)&(*ad).particles[i].type, "%s", type );
				(*ad).particles[i].x = thisRead[0];
				(*ad).particles[i].y = thisRead[1];
				(*ad).particles[i].z = thisRead[2];
				(*ad).particles[i].prop1 = thisRead[3];
				(*ad).particles[i].prop2 = thisRead[4];
				(*ad).particles[i].prop3 = thisRead[5];
				(*ad).particles[i].prop4 = thisRead[6];
				(*ad).particles[i].prop5 = thisRead[7];
				(*ad).particles[i].prop6 = thisRead[8];
				(*ad).particles[i].prop7 = thisRead[9];
				(*ad).particles[i].prop8 = thisRead[10];
				i++;
			}
		}

                fclose( in );
		free(buffer);

		if (i != (*ad).numberOfParticles) {
			printf("The coordinate file is corrupted (number of lines mismatch) -- reading failed.\n");
		       return FALSE;	
		}	
		else {
			return TRUE;
		}
        }
        else {
		free(buffer);

		return FALSE;
        }
}


// Read a file list
bool openFileListFunction( const char * filename, fileList * fl )
{
	char * buffer = (char *)malloc( BUFSIZ );
	char * fileroot = (char *)malloc( BUFSIZ );
	int numberOfFiles = 0;

	// Extract the fileroot
	strcpy( fileroot, filename );
	int i = strlen(filename);
	while (fileroot[i] != '/')
		i--;
	fileroot[i+1] = 0;

        // Open the file; first simply count the number of lines
        if (FILE * in = fopen( (char *)filename, "r" )) {
		while (!feof(in)) {
			fgets( buffer, BUFSIZ , in );
			if (!feof(in)) {
				numberOfFiles++;
			}
		}

		fclose(in);
	}

	// Allocate memory for the file list
	(*fl).filename = (char **)malloc( numberOfFiles*sizeof(char *) );	
	for (int i=0;i<numberOfFiles;i++) 
		(*fl).filename[i] = (char *)malloc( BUFSIZ );

	// Now re-read the file and copy the entries into the file list struct
	numberOfFiles = 0;
        if (FILE * in = fopen( (char *)filename, "r" )) {
		while (!feof(in)) {
			fgets( buffer, BUFSIZ , in );
			if (!feof(in)) {

				// Check if the files are given with 
				// complete paths -- add fileroot otherwise
				if (buffer[0] == '/')
					sprintf( (*fl).filename[numberOfFiles], "%s", buffer);
				else
					sprintf( (*fl).filename[numberOfFiles], "%s/%s", fileroot, buffer);
				
				// Remove trailing \n signs
				int j = 0;
				while ((*fl).filename[numberOfFiles][j] != 0) {
					if ((*fl).filename[numberOfFiles][j] == '\n') {	
						(*fl).filename[numberOfFiles][j] = 0;
					}
					j++;
				}

				numberOfFiles++;
			}
		}

		// Complete the data
		(*fl).numberOfFiles = numberOfFiles;;
		(*fl).currentFile = -1;

                fclose( in );
		free(fileroot);
		free(buffer);

		return TRUE;
	}
        else {
		free(fileroot);
		free(buffer);

		return FALSE;
        }
}


// Generate track file data based on a file list
bool generateTrackDataFunction( fileList * fl, aggregateData * ad, 
							trackData * td )
{
	// Read first file and find the number of particles
	if (openCoordinateFunction( (*fl).filename[0], ad )) {

		// Prepare track data
		(*td).numberOfTracks = (*ad).numberOfParticles;
		(*td).numberOfStages = (*fl).numberOfFiles;

		// Free track memory
		freeTrackData( td );

		// Allocate memory for the track lists
		(*td).type  = (tag *)malloc( (*td).numberOfTracks*sizeof(tag) );
		(*td).x = (float **)malloc( (*td).numberOfTracks*sizeof(float *) );
		(*td).y = (float **)malloc( (*td).numberOfTracks*sizeof(float *) );
		(*td).z = (float **)malloc( (*td).numberOfTracks*sizeof(float *) );
		for (int i=0;i<(*td).numberOfTracks;i++) {
			(*td).x[i] = (float *)malloc( (*td).numberOfStages*sizeof(float) );
			(*td).y[i] = (float *)malloc( (*td).numberOfStages*sizeof(float) );
			(*td).z[i] = (float *)malloc( (*td).numberOfStages*sizeof(float) );
		}
	
		(*td).haveMemoryAllocated = TRUE;

		// Read the files and fill in the data
		for (int stage=0;stage<(*td).numberOfStages;stage++) {
			if (openCoordinateFunction( (*fl).filename[stage], ad )) {
	
				for (int i=0;i<(*td).numberOfTracks;i++) {

					// Fill in types at first pass
					if (stage == 0) 
						typeCopy( (char *)&(*ad).particles[i].type, (char *)&(*td).type[i] );
					
					// Fill in coordinates
					(*td).x[i][stage] = (*ad).particles[i].x;
					(*td).y[i][stage] = (*ad).particles[i].y;
					(*td).z[i][stage] = (*ad).particles[i].z;
				}
			}
			else {
				printf("Could not read list entry %s -- abort\n", (*fl).filename[stage] );
				return FALSE;
			}
		}
	}
	else {
		printf("Could not read first list entry -- abort\n");
		return FALSE;
	}

	return TRUE;
}


// Read a view parameter file
bool openViewParamFunction( const char * filename, viewParam * vp )
{
	int len;
	char * buffer = (char *)malloc( BUFSIZ );
	char * version = (char *)malloc( BUFSIZ );
	
	sprintf(version, "%s", VERSION_STRING);

        // Open the file
        if (FILE * in = fopen( (char *)filename, "r" )) {
		// Read the length of the version string
		fread( &len, sizeof( int ), 1, in );

		// Read the version string and make sure it matches
		fread( buffer, sizeof( char ), len, in );

		// Compare version strings
		if (strcmp( buffer, version ) == 0) {
			// Version strings agree: read this file.
                	fread( vp, sizeof( viewParam ), 1, in );

			free(buffer);
			free(version);
	
			return TRUE;
		}
		else {
			// Version strings do not agree: do not read
			// this file
			printf("Sorry, version strings do not agree -- cannot read the file.\n");
			free(buffer);
			free(version);
	
			return FALSE;
		}
        }
        else {
		free(buffer);
		free(version);

		return FALSE;
        }
}


// Save a view parameter file
bool saveViewParamFunction( const char * filename, viewParam * vp )
{
	int len;
	char * buffer = (char *)malloc( BUFSIZ );

	sprintf(buffer, "%s", VERSION_STRING);
	len = strlen(buffer);

	// Open and write the file
	if (FILE * out = fopen( filename, "w" )) {
		// Write out the length of the version string
		fwrite( &len, sizeof( int ), 1, out );

		// Write out the version string
		fwrite( buffer, sizeof( char ), len, out );
	
		// Print parameters
		fwrite( vp, sizeof( viewParam ), 1, out );

		fclose( out );

		free(buffer);
		return TRUE;
	}
	else {
		free(buffer);
		return FALSE;
	}
}


// Write a particle data file -- if and how to color what kind of particle
bool saveParticleDataFunction( const char * filename, particleData * pd )
{
	int nc = (*pd).numberOfParticleTypes;
	int len;
	char * buffer = (char *)malloc( BUFSIZ );

	sprintf(buffer, "%s", VERSION_STRING);
	len = strlen(buffer);

        // Open the file
        if (FILE * out = fopen( (char *)filename, "w" )) {
		// Write out the length of the version string
		fwrite( &len, sizeof( int ), 1, out );

		// Write out the version string
		fwrite( buffer, sizeof( char ), len, out );
	
		// Write out the number of entries
		fwrite( &nc, sizeof( int ), 1, out );

		// Write out the entries
		fwrite( (*pd).showParticle, sizeof( bool ), ATOMS_MAX, out );
		fwrite( (*pd).showTrack, sizeof( bool ), ATOMS_MAX, out );
		fwrite( (*pd).type, sizeof( tag ), ATOMS_MAX, out );
		fwrite( (*pd).relSize, sizeof( relativeSize ), ATOMS_MAX, out );
		fwrite( (*pd).colorCrit, sizeof( colorCriterion ), ATOMS_MAX, out );
		fwrite( (*pd).colorCritPos, sizeof( colorCriterionPos ), ATOMS_MAX, out );
		fwrite( (*pd).colorCritProp, sizeof( colorCriterionColumn ), ATOMS_MAX, out );
		fwrite( (*pd).colorCritCodeRed, sizeof( colorCriterionColumn ), ATOMS_MAX, out );
		fwrite( (*pd).colorCritCodeGreen, sizeof( colorCriterionColumn ), ATOMS_MAX, out );
		fwrite( (*pd).colorCritCodeBlue, sizeof( colorCriterionColumn ), ATOMS_MAX, out );
		fwrite( (*pd).particleColors, sizeof( particleColor ), ATOMS_MAX, out );
		fwrite( (*pd).particleBonds, sizeof( particleBond ), ATOMS_MAX, out );
		fwrite( (*pd).line, sizeof( lineType ), ATOMS_MAX, out );
		fwrite( (*pd).relSize , sizeof( relativeSize ), ATOMS_MAX, out );

		fclose(out);

		free(buffer);

		return TRUE;
	}
	else {
		free(buffer);

		return FALSE;
	}
}


// Read a particle data file -- if and how to color what kind of particle
bool openParticleDataFunction( const char * filename, particleData * pd )
{
	int nt, len;
	char * buffer = (char *)malloc( BUFSIZ );
	char * version = (char *)malloc( BUFSIZ );

	sprintf(version, "%s", VERSION_STRING);

        // Open the file
        if (FILE * in = fopen( (char *)filename, "r" )) {
		// Read the length of the version string
		fread( &len, sizeof( int ), 1, in );

		// Read the version string and make sure it matches
		fread( buffer, sizeof( char ), len, in );

		// Compare version strings
		if (strcmp( buffer, version ) == 0) {
			// Version strings agree: read this file.
			// Read the number of entries
			fread( &nt, sizeof( int ), 1, in );
			(*pd).numberOfParticleTypes = nt;

			// Read the entries 
			fread( (*pd).showParticle, sizeof( bool ), ATOMS_MAX, in );
			fread( (*pd).showTrack, sizeof( bool ), ATOMS_MAX, in );
			fread( (*pd).type, sizeof( tag ), ATOMS_MAX, in );
			fread( (*pd).relSize, sizeof( relativeSize ), ATOMS_MAX, in );
			fread( (*pd).colorCrit, sizeof( colorCriterion ), ATOMS_MAX, in );
			fread( (*pd).colorCritPos, sizeof( colorCriterionPos ), ATOMS_MAX, in );
			fread( (*pd).colorCritProp, sizeof( colorCriterionColumn ), ATOMS_MAX, in );
			fread( (*pd).colorCritCodeRed, sizeof( colorCriterionColumn ), ATOMS_MAX, in );
			fread( (*pd).colorCritCodeGreen, sizeof( colorCriterionColumn ), ATOMS_MAX, in );
			fread( (*pd).colorCritCodeBlue, sizeof( colorCriterionColumn ), ATOMS_MAX, in );
			fread( (*pd).particleColors, sizeof( particleColor ), ATOMS_MAX, in );
			fread( (*pd).particleBonds, sizeof( particleBond ), ATOMS_MAX, in );
			fread( (*pd).line, sizeof( lineType ), ATOMS_MAX, in );
			fread( (*pd).relSize, sizeof( relativeSize ), ATOMS_MAX, in );
			fclose(in);	

			free(buffer);
			free(version);

			return TRUE;
		}
		else {
			// Version strings do not agree: do not read
			// this file
			printf("Sorry, version strings do not agree -- cannot read the file.\n");
			free(buffer);
			free(version);
	
			return FALSE;
		}
        }
        else {
		free(buffer);
		free(version);

		return FALSE;
        }
}
