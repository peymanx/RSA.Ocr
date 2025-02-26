#-------------------------------------------------
#
# Project created by QtCreator 2021-03-01 10:11:40
#
#-------------------------------------------------

QT       += core gui
INCLUDEPATH += /home/QTCreatorPro/
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BasicFunction
TEMPLATE = app


QMAKE_LFLAGS += -Wl,-rpath=.

SOURCES += main.cpp\
        BasicFunction.cpp \
    ImageAcquisition.cpp \
    MyGraphicsitem.cpp \
    customcombobox.cpp

HEADERS  += BasicFunction.h \
    ImageAcquisition.h \
    DVPCamera.h \
    MyGraphicsitem.h \
    customcombobox.h

FORMS    += BasicFunction.ui


win32{
    contains(QT_ARCH, x86_64) {
        message("Windows x86_64 build")
        ## Windows x64 (64bit) specific build here
        LIBS += -L$$PWD/lib_x86_64/ -lDVPCamera64
        INCLUDEPATH += $$PWD/lib_x86_64
        DEPENDPATH += $$PWD/lib_x86_64
    }else{
        message("Windows x86_32 build")
        ## Windows x86 (32bit) specific build here
        LIBS += -L$$PWD/lib_x86_32/ -lDVPCamera
        INCLUDEPATH += $$PWD/lib_x86_32
        DEPENDPATH += $$PWD/lib_x86_32
    }
}else: unix {
    message("Unix build")
    ## Unix specific build here
    unix:!macx: LIBS += -L$$PWD/lib_linux_x86_64/ -ldvp
    INCLUDEPATH += $$PWD/lib_linux_x86_64
    DEPENDPATH += $$PWD/lib_linux_x86_64
}

