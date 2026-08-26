TEMPLATE = lib

CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    Vector2D.cc \
    MountainChain.cc \
    PotentialField.cc \
    Sky.cc \
    AirCell.cc \
    GaussianPeak.cc \
    Mountain.cc \
    Potential.cc \
    System.cc

HEADERS += \
    Drawable.h \
    Renderer.h \
    MountainChain.h \
    PotentialField.h \
    Sky.h \
    Grid3D.h \
    constants.h \
    AirCell.h \
    GaussianPeak.h \
    Mountain.h \
    Potential.h \
    System.h \
    Vector2D.h