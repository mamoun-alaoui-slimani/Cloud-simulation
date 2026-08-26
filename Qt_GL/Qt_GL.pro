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
    GLWidget.cc \
    OpenGLRenderer.cc

HEADERS += \
    GLWidget.h \
    ShaderAttributes.h \
    OpenGLRenderer.h \
    ../general/Renderer.h \
    ../general/Drawable.h \
    ../general/MountainChain.h \
    ../general/PotentialField.h \
    ../general/Sky.h \
    ../general/Grid3D.h \
    ../general/constants.h \
    ../general/AirCell.h \
    ../general/GaussianPeak.h \
    ../general/Mountain.h \
    ../general/Potential.h \
    ../general/System.h \
    ../general/Vector2D.h

RESOURCES += \
    resource.qrc
