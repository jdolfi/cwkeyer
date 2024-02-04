QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs 
DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += \
    decodetask.cpp \
    paddletask.cpp \
    tones.cpp \
    customgv.cpp \
    guiinterface.cpp \
    main.cpp \
    mainwindow.cpp \
    pulseaudio.cpp

HEADERS += \
    audiodriver.h \
    cwcodes.h \
    decodetask.h \
    paddletask.h \
    tones.h \
    customgv.h \
    guiinterface.h \
    mainwindow.h \
    pulseaudio.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


unix|win32: LIBS += -lpulse-simple

DISTFILES += \
    readme.md
