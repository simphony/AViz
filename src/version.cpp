#include "version.h"

#include <QString>

namespace aviz {

Version::Version()
    :m_major(6), m_minor(1), m_patch(0) {
}

int Version::getMajor() const {
    return m_major;
}

int Version::getMinor() const {
    return m_minor;
}

int Version::getPatch() const {
    return m_patch;
}

/*static*/ QString Version::getVersionString(){
    Version version;
    QString vstr = QString("%1.%2.%3").arg(version.getMajor()).arg(version.getMinor()).arg(version.getPatch());
    return vstr;
}

/*static*/ QString Version::getVersionInfo() {
    Version version;
    QString info = QString("AViz AtomicVizualization version %1 \n").arg(getVersionString());

#ifdef GIT_REVISION
    info += QString("(revision " + QString(GIT_REVISION) + ")\n");
#endif

    info += "(C) 2001 - 2003 Computational Physics Group Israel Institute of Technology\n"\
         "Technion, 32000 Haifa Israel\n"\
         "Geri Wagner, Adham Hashibon";
    return info;
}

/*static*/ QString Version::getStartVersionInfo() {
    QString startInfo = getVersionInfo();
    startInfo +=         "\n"\
                         "AViz comes with ABSOLUTELY NO WARRANTY; for details click on Help/License\n"\
                         "or start with the command line argument -license.   This is free software,\n"\
                         "and you are welcome to redistribute it under certain conditions; click on\n"\
                         "Help/Distribution or start with `aviz -distribution' for details.";
    return startInfo;
}

}
