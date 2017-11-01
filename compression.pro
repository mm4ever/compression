TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    datageneration.c \
    imagecompression.c

HEADERS += \
    datageneration.h \
    imagecompression.h
