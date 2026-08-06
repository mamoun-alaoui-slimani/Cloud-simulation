QT += core gui opengl widgets

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -Wall -pedantic

win32:LIBS += -lopengl32


TARGET = ex_05_gl

LIBS += -L../general -lgeneral
PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH += ../general

SOURCES += \
    main_qt_gl.cc \
    glwidget.cc \
    vue_opengl.cc \
    glsphere.cc

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
    ../general/Vecteur2D.h \
    glsphere.h

RESOURCES += \
    resource.qrc
