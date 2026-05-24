#-------------------------------------------------
#
# Project created by QtCreator 2018-11-29T15:00:25
#
#-------------------------------------------------

QT       += core gui svg printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION,5)  QT += core5compat

TARGET = TestClineChart
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

CONFIG += c++11

SOURCES += \
    MainWindow.cpp \
    CLineChart.cpp \
    Matrix.cpp \
    Main.cpp \
    CScaleDlg.cpp \
    CUnitsDlg.cpp \
    CSimOut.cpp \
    CSimOutATP.cpp

HEADERS += \
    MainWindow.h \
    CLineChart.h \
    Matrix.h \
    CScaleDlg.h \
    CUnitsDlg.h \
    CSimOut.h \
    ExcludeATPCode.h

FORMS += \
        MainWindow.ui \
    CScaleDlg.ui \
    CUnitsDlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
