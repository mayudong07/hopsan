# -------------------------------------------------
# Global project options
# -------------------------------------------------
include( ../Common.prf )

TARGET = HopsanGenerator
TEMPLATE = lib
CONFIG += shared
DESTDIR = $${PWD}/../bin
TARGET = $${TARGET}$${DEBUG_EXT}

QT -= gui
QT += core xml

# Keep these QtGui related options by them self so that external scrip may deactivate them when needed
useqtgui=True
contains( useqtgui, True ) {
    DEFINES += USEQTGUI
    QT += gui
    isEqual(QT_MAJOR_VERSION, 5){
        QT += widgets
    }
    message(Using Qt GUI components in HopsanGenerator)
}

# Enable C++11
lessThan(QT_MAJOR_VERSION, 5){
  QMAKE_CXXFLAGS += -std=c++11
} else {
  CONFIG += c++11
}

#--------------------------------------------------------
# Set the FMILibrary include and library paths
include($${PWD}/../Dependencies/fmilibrary.pri)
# On Windows, since RPATH is ignored by LoadLibrary(), copy the fmi library file to the bin directory after build,
# so that libHopsanGenerator may find it when loaded at runtime
# (this is only necessary for dev builds, on release all DLLs will be copied anyway)
# TODO: It would be better if this could be handled by the fmilibrary.pri somehow
win32 {
  src_file = $$quote($${PWD}/../Dependencies/FMILibrary/lib/libfmilib_shared.dll)
  dst_dir = $$quote($${DESTDIR})
  # Replace slashes in paths with backslashes for Windows
  src_file ~= s,/,\\,g
  dst_dir ~= s,/,\\,g
  QMAKE_POST_LINK *= $$QMAKE_COPY $${src_file} $${dst_dir}
}

#--------------------------------------------------
# Add the include path to our self, (HopsanGenerator)
INCLUDEPATH *= $${PWD}/include/
#--------------------------------------------------

#--------------------------------------------------
# Add the include path to (HopsanCore)
INCLUDEPATH *= $${PWD}/../HopsanCore/include/
LIBS *= -L$${PWD}/../bin -lHopsanCore$${DEBUG_EXT}
#--------------------------------------------------

#--------------------------------------------------
# Add the include path to (SymHop)
INCLUDEPATH *= $${PWD}/../SymHop/include/
LIBS *= -L$${PWD}/../bin -lSymHop$${DEBUG_EXT}
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
    #DEFINES += STATICCORE      #Use this if you are compiling the generator into a program directly or building a static lib
    DEFINES += DOCOREDLLEXPORT  #Use this if you are compiling the generator as a DLL or SO
    DEFINES -= UNICODE
}
unix {
    # Add runtime search path so that dynamically loaded libraries in the same directory can be found.
    # Note! QMAKE_LFLAGS_RPATH and QMAKE_RPATHDIR does not seem hande $$ORIGIN, adding manually to LFLAGS
    QMAKE_LFLAGS *= -Wl,-rpath,\'\$$ORIGIN/./\'
}

# -------------------------------------------------
# Project files
# -------------------------------------------------
SOURCES += \
    src/HopsanGeneratorLib.cc \
    src/GeneratorUtilities.cc \
    src/generators/HopsanGenerator.cc \
    src/generators/HopsanSimulinkGenerator.cc \
    src/generators/HopsanModelicaGenerator.cc \
    src/generators/HopsanFMIGenerator.cc \
    src/generators/HopsanLabViewGenerator.cc

HEADERS += \
    include/win32dll.h \
    include/GeneratorUtilities.h \
    include/generators/HopsanGenerator.h \
    include/generators/HopsanModelicaGenerator.h \
    include/generators/HopsanSimulinkGenerator.h \
    include/generators/HopsanFMIGenerator.h \
    include/generators/HopsanLabViewGenerator.h

RESOURCES += \
    templates.qrc
