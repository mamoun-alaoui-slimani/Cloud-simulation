QT += core gui opengl widgets openglwidgets

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -Wall -pedantic

win32:LIBS += -lopengl32

# Apple a déprécié OpenGL en 10.14 mais le maintient : on assume le choix
# du pipeline hérité plutôt que de laisser défiler les avertissements.
macx: DEFINES += GL_SILENCE_DEPRECATION

TARGET = cloudsim-gl
QMAKE_TARGET_BUNDLE_PREFIX = ch.epfl.cloudsim

LIBS += -L../general -lgeneral
PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH += ../general

SOURCES += \
    main_qt_gl.cc \
    glwidget.cc \
    vue_opengl.cc

HEADERS += \
    glwidget.h \
    vertex_shader.h \
    vue_opengl.h \
    ../general/SupportADessin.h \
    ../general/Dessinable.h \
    ../general/ChaineDeMontagnes.h \
    ../general/ChampPotentiels.h \
    ../general/Ciel.h \
    ../general/Collection3D.h \
    ../general/constantes.h \
    ../general/CubedAir.h \
    ../general/Gaussienne.h \
    ../general/Montagne.h \
    ../general/Potentiel.h \
    ../general/Systeme.h \
    ../general/Vecteur2D.h

RESOURCES += \
    resource.qrc
