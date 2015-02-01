TEMPLATE        = app
TARGET          = aviz
CONFIG          += qt opengl warn_on release thread
QMAKE_CXXFLAGS  = -O2
QT +=  opengl 
LIBS += -lX11 -lpng -lGLU
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
		  widgets/doneapplycancelwidget.cpp
