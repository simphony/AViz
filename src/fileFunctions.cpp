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
#include <sys/stat.h>
#include <iostream>
#include <cstring>

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFile>

#include "fileFunctions.h"
#include "memoryFunctions.h"
#include "version.h"
#include "defaultParticles.h"
#include "version.h"
#include "exceptions.h"


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
    bool exists = false;

    if (!stat( (const char *)filename, buf )) {
        // File exists...
        exists = true;
    }

    free( buf );

    return exists;
}

particle parseParticleLine(const QString& line) {
    particle p;
    QStringList list = line.split(' ', QString::SkipEmptyParts);
    if (list.count() < 4 || list.count() > 12) {
        throw aviz::parse_error("Incorrect number of values. Expecting type, x, y, z and 8 or less properties");
    }

    // Get the atom type
    QString type = list[0];
    if(type.length()>2) {
        throw aviz::parse_error ("Incorrect format of type");
    }

    tag pTag;
    for(int i=0; i<2; i++) {
        pTag.type[i] = i<type.length() ? type.at(i).toLatin1() : ' ';
    }
    pTag.type[3] = 0;

    float values[11] = {};
    bool ok = true;
    for(int index = 1; index < list.count(); index++) {
        values[index-1] = QString(list[index]).toDouble(&ok);
        if(!ok) {
            QString msg = QString("Problem reading coordinate or property (%1:'%2')").arg(index).arg(list[index]);
            throw aviz::parse_error (qPrintable(msg));
        }
    }

    return particle {pTag,
                values[0], // x
                values[1], // y
                values[2], // z
                values[3], // prop1
                values[4], // prop2 ..
                values[5],
                values[6],
                values[7],
                values[8],
                values[9],
                values[10]};
    return p;
}

struct PropertyInformation{
    QString name;
    /*QString description;*/
};

QList<PropertyInformation> parsePropertyInformation(QStringList propertyInformation) {
    auto lineNumber = 1;
    QList<PropertyInformation> information;
    if(propertyInformation.empty() || propertyInformation.takeFirst() != "properties:") {
        throw aviz::parse_error ("Property information section did not begin with 'properties:' as expected.");
    }

    while(!propertyInformation.empty()) {
        lineNumber++;

        QString line = propertyInformation.takeFirst();
        line = line.trimmed();
        if(line.startsWith("-name:")) {
            line.remove(0, line.indexOf(":")+1);

            PropertyInformation info;
            info.name = line.trimmed();
            information.push_back(info);

            auto propertyNumber = lineNumber - 1;
            QString msg = QString("Property (#%1): %2").arg(propertyNumber).arg(info.name);
            std::cout << qPrintable(msg) <<std::endl;
        } else {
           QString msg = QString("Error parsing %1 line in property information section").arg(lineNumber);
           throw aviz::parse_error (qPrintable(msg));
        }
    }


    return information;
}

bool openCoordinateFunction(const char * filename, aggregateData * ad ){

    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text)) {

        // Register the filename
        sprintf( (*ad).filename, "%s", filename );

        QTextStream in(&file);

        // Read the number of particles
        bool ok = true;
        (*ad).numberOfParticles = QString(in.readLine()).toInt(&ok);

        if(!ok && in.atEnd()) {
            return false;
        }

        // Read the ID string
        std::strncpy((*ad).IDstring, in.readLine().toLatin1().constData(), BUFSIZ);

        // Free particle data memory
        freeAggregateData( ad );

        // Allocate memory for the particle positions
        allocateAggregateData( ad );

        // Read the atom positions and properties
        int i = 0;
        while (!in.atEnd() && i < (*ad).numberOfParticles) {
            QString line = in.readLine();
            try {
                (*ad).particles[i] = parseParticleLine(line);
            }
            catch (const aviz::parse_error &e) {
                std::cout << "Problem reading line #" << i << ": " << e.what() << std::endl;
                return false;
            }
            i++;
        }

        if (i != (*ad).numberOfParticles) {
            printf("The coordinate file is corrupted (number of lines mismatch) -- reading failed.\n");
            return false;
        }

        // Read the possible property information
        QStringList propertyInformationLines;
        while (!in.atEnd()) {
            propertyInformationLines.append(in.readLine());
        }
        file.close();


        if(!propertyInformationLines.empty()) {
            try {
                QList<PropertyInformation> todo = parsePropertyInformation(propertyInformationLines);
            }
            catch (const aviz::parse_error &e) {
                std::cout << "Problem reading property information (which first begins on line #)" << i+1 << ": " << e.what() << std::endl;
                return false;
            }
        } else {
            // there are no property information
        }

        return true;
    } else {
        return false;
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

        return true;
    }
    else {
        free(fileroot);
        free(buffer);

        return false;
    }
}


// Generate track file data based on a file list
bool generateTrackDataFunction(const fileList &fl, aggregateData * ad,
                               trackData * td )
{
    // Read first file and find the number of particles
    if (fl.numberOfFiles > 0 && openCoordinateFunction( fl.filename[0], ad )) {

        // Prepare track data
        (*td).numberOfTracks = (*ad).numberOfParticles;
        (*td).numberOfStages = fl.numberOfFiles;

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

        (*td).haveMemoryAllocated = true;

        // Read the files and fill in the data
        for (int stage=0;stage<(*td).numberOfStages;stage++) {
            if (openCoordinateFunction(fl.filename[stage], ad )) {

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
                printf("Could not read list entry %s -- abort\n", fl.filename[stage] );
                return false;
            }
        }
    }
    else {
        printf("Could not read first list entry -- abort\n");
        return false;
    }

    return true;
}

/// check version and return true if major and minor version
/// matches current version (patch version is not considered)
/// @param versionString version string (i.e. "6.1.0")
bool isMatchingVersion(QString versionString) {
    aviz::Version currentVersion;
    QStringList version = versionString.split( "." );
    return ((version[0].toInt() == currentVersion.getMajor())
            && (version[0].toInt() == currentVersion.getMajor()));
}

// Read a view parameter file
bool openViewParamFunction( const char * filename, viewParam * vp ) {
    // Open the file
    if (FILE * in = fopen( (char *)filename, "r" )) {
        // Read the length of the version string
        int versionLength;
        fread( &versionLength, sizeof( int ), 1, in );

        // Read the version string and make sure it matches
        char * readVersion = new char[versionLength+1];
        fgets(readVersion, versionLength+1, in);

        // Compare version strings
        const bool isMatching = isMatchingVersion(readVersion);

        delete[] readVersion;

        if (isMatching) {
            // Version strings agree: read this file.
            fread( vp, sizeof( viewParam ), 1, in );
            return true;
        }
        else {
            // Version strings do not agree: do not read
            // this file
            printf("Sorry, version strings do not agree -- cannot read the file.\n");
            return false;
        }
    } else {
        return false;
    }
}


// Save a view parameter file
bool saveViewParamFunction( const char * filename, viewParam * vp )
{
    const QString version = aviz::Version::getVersionString();
    const int version_length = version.length();

    // Open and write the file
    if (FILE * out = fopen( filename, "w" )) {
        // Write out the length of the version string
        fwrite( &version_length, sizeof( int ), 1, out );

        // Write out the version string
        fwrite(qPrintable(version), sizeof( char ), version_length, out );

        // Print parameters
        fwrite( vp, sizeof( viewParam ), 1, out );

        fclose( out );
        return true;
    }
    else {
        return false;
    }
}


// Write a particle data file -- if and how to color what kind of particle
bool saveParticleDataFunction( const char * filename, particleData * pd )
{
    const int nc = (*pd).numberOfParticleTypes;
    const QString version = aviz::Version::getVersionString();
    const int version_length = version.length();

    // Open the file
    if (FILE * out = fopen( (char *)filename, "w" )) {
        // Write out the length of the version string
        fwrite( &version_length, sizeof( int ), 1, out );

        // Write out the version string
        fwrite(qPrintable(version), sizeof( char ), version_length, out );

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
        return true;
    }
    else {
        return false;
    }
}


// Read a particle data file -- if and how to color what kind of particle
bool openParticleDataFunction( const char * filename, particleData * pd )
{
    // Open the file
    if (FILE * in = fopen( (char *)filename, "r" )) {
        bool result = false;

        // Read the length of the version string
        int versionLength;
        fread( &versionLength, sizeof( int ), 1, in );

        // Read the version string and make sure it matches
        char * readVersion = new char[versionLength+1];
        fgets(readVersion, versionLength+1, in);
        bool isMatching = isMatchingVersion(readVersion);
        delete[] readVersion;

        // Compare version strings
        if (isMatching) {
            // Version strings agree: read this file.
            // Read the number of entries
            int nt;
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

            result = true;
        } else {
            // Version strings do not agree: do not read
            // this file
            printf("Sorry, version strings do not agree -- cannot read the file.\n");
        }
        fclose(in);
        return result;
    } else {
        return false;
    }
}
