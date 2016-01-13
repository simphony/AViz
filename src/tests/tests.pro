QT += testlib
TEMPLATE = app
TARGET = tests
INCLUDEPATH += .
QMAKE_CXXFLAGS += -std=c++11

HEADERS = ../fileFunctions.h \
          ../data.h \
          ../defaults.h \
          ../memoryFunctions.h \
          ../version.h \
          ../defaultParticles.h \
          verify_exception.h
SOURCES = fileFunctionsTest.cpp \
          ../fileFunctions.cpp \
          ../memoryFunctions.cpp \
          ../defaultParticles.cpp \
          ../version.cpp 
