#-------------------------------------------------
#
# Project created by QtCreator 2017-07-17T08:53:18
#
#-------------------------------------------------

QT       += gui core svg core-private

TARGET = JPKeyboard
TEMPLATE = lib
DEFINES += JPKEYBOARD_LIBRARY HAVE_PINYIN
DESTDIR += ../../JPKeyboard\bin
CONFIG(debug, debug|release){
    TARGET = jpkeyboardd
    LIBS += -L../../JPKeyboard/lib/ -lpinyind

} else {
    TARGET = jpkeyboard
    LIBS += -L../../JPKeyboard/lib/ -lpinyin
}

SOURCES += \
            KeyboardGlobal.cpp \
    src/abstractinputmethod.cpp \
    src/abstractinputpanel.cpp \
    src/appinputpanel.cpp \
    src/defaultinputmethod.cpp \
    src/desktopinputpanel2.cpp \
    src/inputcontext.cpp \
    src/inputengine.cpp \
    src/inputmethod.cpp \
    src/inputview.cpp \
    src/pinyindecoderservice.cpp \
    src/pinyininputmethod.cpp \
    src/plaininputmethod.cpp \
    src/platforminputcontext.cpp \
    src/plugin.cpp \
    src/selectionlistmodel.cpp \
    src/settings.cpp \
    src/shifthandler.cpp \
    src/trace.cpp

HEADERS +=\
    include/jpkeyboard_global.h \
    include/KeyboardGlobal.h \
    include/abstractinputmethod.h \
    include/abstractinputpanel.h \
    include/appinputpanel.h \
    include/defaultinputmethod.h \
    include/desktopinputpanel2.h \
    include/inputcontext.h \
    include/inputengine.h \
    include/inputmethod.h \
    include/inputview.h \
    include/pinyindecoderservice.h \
    include/pinyininputmethod.h \
    include/plaininputmethod.h \
    include/platforminputcontext.h \
    include/plugin.h \
    include/selectionlistmodel.h \
    include/settings.h \
    include/shifthandler.h \
    include/trace.h \
    include/virtualkeyboarddebug.h

INCLUDEPATH +=  ./include/   \
                ./include/pinyin/include/


unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    content/styles/default/default_style.qrc
