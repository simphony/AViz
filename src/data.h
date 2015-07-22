//
// Declaration of data structures
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

#ifndef DATA_H
#define DATA_H

#include "defaults.h"
#include <cstdio> // BUFSIZ

// Define view modes
enum vMode{ PERSPECTIVE, ORTHO };

// Define background colors
enum bgColor{ BGBLACK, BGWHITE };

// Define render qualities
enum quality{ LOW, HIGH, FINAL }; 

// Define render modes
enum rMode{ ATOMS, SPINS, LIQUID_CRYSTALS, POLYMERS, PORES }; 

// Define atom render styles
enum atomRStyle{ ASPHERE, ACYLINDER, ACONE, ACUBE, ALINE, ADOT }; 

// Define spin render styles
enum spinRStyle{ SSPHERE, SCYLINDER, SCONE, SCUBE, SLINE, SDOT }; 

// Define liquid crystal render styles
enum lcRStyle{ LSPHERE, LCYLINDER, LCONE, LCUBE, LLINE, LDOT }; 

// Define polymer render styles
enum polymerAtomRStyle{ PASPHERE, PACYLINDER, PACONE, PACUBE, PALINE, PADOT };

// Define pore render styles
enum poreRStyle{ PSPHERE, PCYLINDER, PCONE, PCUBE, PLINE, PDOT }; 

// Define color modes 
enum colorCriterion{ TYPE, POSITION, PROPERTY, COLORCODE }; 

// Define color modes 
enum colorCriterionPos{ XPOS, YPOS, ZPOS }; 

// Define color modes 
enum colorCriterionColumn{ PROP1, PROP2, PROP3, PROP4, PROP5, PROP6, PROP7, PROP8 }; 

// Define relative render sizes
enum relativeSize{ REGULAR, TINY, SMALL, LARGE, HHUGE }; 

// Define colors
enum fixedColor{ WHITE, BLACK, RED, GREEN, BLUE, YELLOW, GOLD, SILVER, BRONZE }; 
// Define bond and cylinder thickness
enum fixedThickness{ LINE, THIN, MEDIUM, THICK, FAT, VARYING_LINEAR, VARYING_QUADRATIC };

// Define file types
enum fileType { XYZ_FILE, VP_FILE, PNG_FILE, ANIMATION, UNKNOWN }; // Define file types

// Define track render modes
enum trackRenderMode{ ALL_STAGES, UP_TO_CURRENT };


// Definition of a view parameter structure: all parameters 
// specifying the rendering that do not depend on the particular
// data set to be rendered
typedef struct{
    double fovy;
    double phi;
    double theta;
    double chi;
    double dolly;
    double phiHome;
    double thetaHome;
	double chiHome;
	double dollyHome;
	double fovyHome;
	double panX;
	double panY;
	double panZ;
	double panRight;
	double panTop;
	double panForward;
	float clipNear;
	float clipFar;
	bool autoClipNear;
	bool autoClipFar;
	float renderSize;
	quality renderQuality;
	rMode renderMode;
	atomRStyle atomRenderStyle;
	spinRStyle spinRenderStyle;
	lcRStyle lcRenderStyle;
	polymerAtomRStyle polymerAtomRenderStyle;
	poreRStyle poreRenderStyle;
	bool onlyContour;
	bool showContour;
	bool showAxes;
	bool autoRot1;
	bool autoRot2;
	bool autoTilt1;
	bool autoTilt2;
	bool autoSpin1;
	bool autoSpin2;
	bool autoZoom1;
	bool autoZoom2;
	bool autoSnap;
	bool enableUpdate;
	float minDist, maxDist;
	float eyex, eyey, eyez;
	bool slicing;
	bool slicingSet;
	float sliceXMin, sliceXMax;
	float sliceYMin, sliceYMax;
	float sliceZMin, sliceZMax;
	bool showSlicePlaneX;
	bool showSlicePlaneY;
	bool showSlicePlaneZ;
    bool showTracks;
	int trackRenderFromStage;
	int trackRenderToStage;
	trackRenderMode tRenderMode;
	bool periodicBCTracks;
	float ** x;
	float stretchX, stretchY, stretchZ;
	bgColor background;
	vMode viewMode;
	bool light[LIGHTS_MAX];
	float lightDepth[LIGHTS_MAX];
	float ambientLight;
	float shininess;
	bool showAnnotation;
	int annotationCoordX;
	int annotationCoordY;
	int annotationSize;
	char annotationText[BUFSIZ];
	double eyeSeparation;
	bool stereoVisionEn;
} viewParam;


// Definition of a view object structure: the length, height, width 
// of the set of particles to be rendered, and similar items
typedef struct{
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;
	float p1min, p1max;
	float p2min, p2max;
	float p3min, p3max;
	float p4min, p4max;
	float p5min, p5max;
	float p6min, p6max;
	float p7min, p7max;
	float p8min, p8max;
	float deltax, deltay, deltaz;
	float deltap1, deltap2, deltap3, deltap4;
	float deltap5, deltap6, deltap7, deltap8;
	float xyzmin;
	float xyzmax;
	float maxXYExtension;
	float maxXZExtension;
	float maxYZExtension;
	float maxExtension;
	float centerx, centery, centerz;
} viewObject;


// Definition of a particle (atom, spin, ...) type tag
typedef struct{
	char type[3];
} tag;


// Definition of a particle (atom, spin, ...) structure: 
// the type of the particle, its position, and additional properties
typedef struct{
	tag type;
	float x, y, z;	
	float prop1, prop2, prop3, prop4, prop5, prop6, prop7, prop8;
} particle;


// Definition of an aggregate data struture -- a set of particles such as 
// atoms, spins, liquid crystals, pores, ...
typedef struct{
	int numberOfParticles;
	char IDstring[BUFSIZ];
	char filename[BUFSIZ];
	particle * particles;
	bool haveMemoryAllocated;
} aggregateData;


// Definition of a file list, used to cycle through a set of aggregate
// data files
typedef struct {
	int numberOfFiles;
	int currentFile;
	char ** filename;
} fileList;


// Definition of track data, used to display particle tracks
typedef struct {
	int numberOfTracks;
	int numberOfStages;
	tag * type;
	float ** x;
	float ** y;
	float ** z;
	bool haveMemoryAllocated;
} trackData;


// Definition of a color structure, associated with a given 
// particle type
typedef struct{
    float red;	// Used if coloring according to particle type
    float green;
    float blue;
	float redMin; 	// Used if coloring according to particle position/property
	float greenMin;
	float blueMin;
	float redMax;
	float greenMax;
	float blueMax;
	float redTwo;	// Used to use one or two additonal type colors for liquid crystals
	float greenTwo;
	float blueTwo;
	float redThree;
	float greenThree;
	float blueThree;
} particleColor;


// Definition of a bond structure, associated with a given 
// particle type
typedef struct {
	int numberOfNeighborBonds; // Each atom type forms bonds to a neighbor 
	bool bShowBond[BONDS_MAX];	// Flag to enable display of this bond
	tag neighborParticleType[BONDS_MAX];	
	fixedColor bColor[BONDS_MAX];
	fixedThickness bThickness[BONDS_MAX];
	float bEmission[BONDS_MAX];
	float bMinLength[BONDS_MAX];
	float bMaxLength[BONDS_MAX];
	bool bAntiAlias[BONDS_MAX];
	bool bSeqBond[BONDS_MAX];
} particleBond;


// Definition of a line type data structure, associated with a 
// given particle type (used only spins, pores, and liquid crystals).
typedef struct {
        fixedThickness fThickness;
        float fEmission;
	int typeColorNumber;
        bool fAntiAlias;
} lineType;


// Definition of a particle data structure -- a set of colors 
// and associated switches, plus specialized structures for 
// atoms, spins, etc
typedef struct{
	int numberOfParticleTypes;
	tag * type;
	bool * showParticle;
	bool * showTrack;
	colorCriterion * colorCrit;
	colorCriterionPos * colorCritPos;
	colorCriterionColumn * colorCritProp;
	colorCriterionColumn * colorCritCodeRed;
	colorCriterionColumn * colorCritCodeGreen;
	colorCriterionColumn * colorCritCodeBlue;
	particleColor * particleColors;
	particleBond * particleBonds;
	lineType * line;
	relativeSize * relSize;
} particleData;


#endif // DATA_H
