TEMPLATE = lib

CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    Vecteur2D.cc \
    ChaineDeMontagnes.cc \
    ChampPotentiels.cc \
    Ciel.cc \
    CubedAir.cc \
    Gaussienne.cc \
    Montagne.cc \
    Potentiel.cc \
    Systeme.cc

HEADERS += \
    Dessinable.h \
    SupportADessin.h \
    ChaineDeMontagnes.h \
    ChampPotentiels.h \
    Ciel.h \
    Collection3D.h \
    constantes.h \
    CubedAir.h \
    Gaussienne.h \
    Montagne.h \
    Potentiel.h \
    Systeme.h \
    Vecteur2D.h