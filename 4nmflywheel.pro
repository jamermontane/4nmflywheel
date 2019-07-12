#-------------------------------------------------
#
# Project created by QtCreator 2019-07-05T22:07:09
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 4nmflywheel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    csqldatabase.cpp \
    motor.cpp \
    qcustomplot.cpp \
    motordriver.cpp

HEADERS  += mainwindow.h \
    csqldatabase.h \
    motor.h \
    qcustomplot.h \
    motordriver.h

FORMS    += mainwindow.ui

QT += sql

#LIBS += -LC:/mycode/motionStudio -lTML_lib
#LIBS += -LC:/mycode/motionStudio -ltmlcomm

QT += serialport
