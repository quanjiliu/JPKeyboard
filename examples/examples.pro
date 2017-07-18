#-------------------------------------------------
#
# Project created by QtCreator 2017-07-17T11:33:27
#
#-------------------------------------------------

QT       += core gui core-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = examples
TEMPLATE = app
DESTDIR += ../../JPKeyboard/bin
CONFIG(debug, debug|release){
    TARGET = examplesd
    LIBS += -L../../JPKeyboard/bin/ -ljpkeyboardd

} else {
    TARGET = examples
    LIBS += -L../../JPKeyboard/bin/ -ljpkeyboard
}

INCLUDEPATH +=  ../../JPKeyboard/   \
                ../../JPKeyboard/include

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui
