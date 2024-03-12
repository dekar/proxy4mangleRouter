QT -= gui

CONFIG += c++11 console network
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

UI_DIR = build/moc
OBJECTS_DIR = build/obj
DESTDIR = build/bin
MOC_DIR = build/moc

SOURCES += \
        src/main.cpp \
        src/httprequest.cpp \
        src/proxyhandler.cpp \
        src/socketer.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/httprequest.h \
    src/proxyhandler.h \
    src/socketer.h
