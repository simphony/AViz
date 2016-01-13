TEMPLATE        = app
TARGET          = aviz
CONFIG          += qt opengl warn_on release thread
QMAKE_CXXFLAGS  = -O2
QMAKE_CXXFLAGS += -std=c++11  # for qt4
QT +=  opengl 
LIBS += -lX11 -lpng -lGLU

message(testing for git descibe)
system(git describe):HAS_GIT=TRUE
equals(HAS_GIT,TRUE) {
   message( git describe works. revision number will be used in AViz )
   GIT_REVISION = $$system($$quote(git describe))
   DEFINES += $$quote(GIT_REVISION=\'\"$$GIT_REVISION\"\')
} else {
   message( 'git describe' is missing. revision number will be not be used in AViz )
}

HEADERS		= SoAnyThumbWheel.h \
		  SoQtThumbWheel.h \
		  animationBoard.h \
		  annotationBoard.h \
		  atomBoard.h \
		  aviz.h \
		  bondBoard.h \
		  clipBoard.h \
		  codeBox.h \
		  colorBoard.h \
		  colorLabel.h \
		  customFileDialog.h \
		  data.h \
		  defaultParticles.h \
		  defaults.h \
		  explicitBoard.h \
		  fileFunctions.h \
		  fileListBoard.h \
		  floatSpin.h \
		  glCanvasArea.h \
		  glCanvasFrame.h \
		  imageFileFunctions.h \
		  lcBoard.h \
		  license.h \
		  lightBoard.h \
		  lineTypeBox.h \
		  liveBox.h \
		  mainForm.h \
		  memoryFunctions.h \
		  messageBox.h \
		  parameterLimits.h \
		  polymerBoard.h \
		  poreBoard.h \
		  positionBox.h \
		  propertyBox.h \
		  renderStyleBox.h \
		  sizeBox.h \
		  sliceBoard.h \
		  spinBoard.h \
		  stretchBoard.h \
		  trackBoard.h \
		  translationBoard.h \
		  typeColorNumberBox.h \
		  version.h \
		  widgets/doneapplycancelwidget.h
SOURCES         = SoAnyThumbWheel.cpp \
		  SoQtThumbWheel.cpp \
		  animationBoard.cpp \
		  annotationBoard.cpp \
		  atomBoard.cpp \
		  aviz.cpp \
		  bondBoard.cpp \
		  clipBoard.cpp \
		  codeBox.cpp \
		  colorBoard.cpp \
		  colorLabel.cpp \
		  customFileDialog.cpp \
		  defaultParticles.cpp \
		  explicitBoard.cpp \
		  fileFunctions.cpp \
		  fileListBoard.cpp \
		  floatSpin.cpp \
		  glCanvasArea.cpp \
		  glCanvasFrame.cpp \
		  imageFileFunctions.cpp \
		  lcBoard.cpp \
		  lightBoard.cpp \
		  lineTypeBox.cpp \
		  liveBox.cpp \
		  main.cpp \
		  mainForm.cpp \
		  memoryFunctions.cpp \
		  messageBox.cpp \
		  polymerBoard.cpp \
		  poreBoard.cpp \
		  positionBox.cpp \
		  propertyBox.cpp \
		  renderStyleBox.cpp \
		  sizeBox.cpp \
		  sliceBoard.cpp \
		  spinBoard.cpp \
		  stretchBoard.cpp \
		  trackBoard.cpp \
		  translationBoard.cpp \
		  typeColorNumberBox.cpp \
		  version.cpp \
		  widgets/doneapplycancelwidget.cpp
