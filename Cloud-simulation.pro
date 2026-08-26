TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS = \
    general \
    text \
    tests \
    Qt_GL

text.depends  = general
tests.depends = general
Qt_GL.depends = general
