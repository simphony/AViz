#ifndef AGGREGATEDATA_H
#define AGGREGATEDATA_H

#include <QList>
#include <QString>

#include <data.h> // for tag and BUFSIZE

struct PropertyInformation{
    QString name;
    /*QString description;*/
};

// Definition of an aggregate data struture -- a set of particles such as
// atoms, spins, liquid crystals, pores, ...
struct AggregateData {
    int numberOfParticles;
    char IDstring[BUFSIZ];
    char filename[BUFSIZ];
    particle * particles;
    bool haveMemoryAllocated;
    QList<PropertyInformation> propertiesInformation;
};


#endif // AGGREGATEDATA_H
