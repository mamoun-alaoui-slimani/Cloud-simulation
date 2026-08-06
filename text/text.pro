QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -Wall -pedantic

TARGET = ex_05_text

LIBS += -L../general -lgeneral
PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH += ../general

SOURCES += \
    main_text.cc \
    TextViewer.cc

HEADERS += \
    ../general/Dessinable.h \
    ../general/SupportADessin.h \
    TextViewer.h
