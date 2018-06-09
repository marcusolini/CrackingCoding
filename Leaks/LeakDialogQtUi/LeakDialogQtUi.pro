#-------------------------------------------------
#
# Project created by QtCreator 2018-06-05T23:05:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LeakDialogQtUi
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        leakdialogqtui.cpp \
    progressdialog.cpp

HEADERS += \
        leakdialogqtui.h \
    progressdialog.h \
    resourceleaktest.h

FORMS += \
        leakdialogqtui.ui \
    progressdialog.ui


#LIBS += -L"../../LeakLib/dist/Debug/GNU-Linux" -lLeakLib
#INCLUDEPATH += ../LeakLib


unix:!macx: LIBS += -L$$PWD/../LeakLib/dist/Debug/GNU-Linux/ -lLeakLib

INCLUDEPATH += $$PWD/../LeakLib
DEPENDPATH += $$PWD/../LeakLib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../release/ -lLeakLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../debug/ -lLeakLib

INCLUDEPATH += $$PWD/../LeakLib
DEPENDPATH += $$PWD/../LeakLib