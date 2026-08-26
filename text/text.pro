QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -Wall -pedantic

TARGET = cloudsim-text
CONFIG -= app_bundle
CONFIG += console

LIBS += -L../general -lgeneral
PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH += ../general

SOURCES += \
    main_text.cc \
    TextRenderer.cc

HEADERS += \
    ../general/Drawable.h \
    ../general/Renderer.h \
    TextRenderer.h
