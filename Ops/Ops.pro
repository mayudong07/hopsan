# -------------------------------------------------
# Global project options
# -------------------------------------------------
include( ../Common.prf )

TARGET = Ops
TEMPLATE = lib
CONFIG += shared
DESTDIR = $${PWD}/../bin

QT += core
QT -= gui

TARGET = $${TARGET}$${DEBUG_EXT}


#--------------------------------------------------
# Add the include path to our self, (Ops)
INCLUDEPATH *= $${PWD}/include/
#--------------------------------------------------

# -------------------------------------------------
# Non platform specific HopsanCompGen options
# -------------------------------------------------
CONFIG(debug, debug|release) {
  QMAKE_CXXFLAGS += -pedantic -Wno-long-long
}
CONFIG(release, debug|release) {

}

# -------------------------------------------------
# Platform specific additional project options
# -------------------------------------------------
win32 {
    DEFINES += OPSDLLEXPORT
    DEFINES -= UNICODE
}

# -------------------------------------------------
# Project files
# -------------------------------------------------

SOURCES += \
    src/OpsWorker.cc \
    src/OpsWorkerSimplex.cpp \
    src/OpsWorkerComplexRF.cpp \
    src/OpsWorkerNelderMead.cpp \
    src/OpsEvaluator.cpp \
    src/OpsWorkerParticleSwarm.cpp \
    src/OpsWorkerComplexRFP.cpp

HEADERS += \
    include/win32dll.h \
    include/OpsWorker.h \
    include/OpsWorkerSimplex.h \
    include/OpsWorkerComplexRF.h \
    include/OpsWorkerNelderMead.h \
    include/OpsEvaluator.h \
    include/OpsWorkerParticleSwarm.h \
    include/OpsWorkerComplexRFP.h








