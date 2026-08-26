QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -Wall -pedantic

TARGET = cloudsim-tests
CONFIG -= app_bundle
CONFIG += console

LIBS += -L../general -lgeneral
PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH += ../general ../text

SOURCES += \
    main_tests.cc \
    ../text/TextRenderer.cc
