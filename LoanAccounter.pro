QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    accounter.cpp \
    addnewchangedeleteitemwindow.cpp \
    addnewitemwindow.cpp \
    changedeleteitemwindow.cpp \
    creditcontract.cpp \
    dbmanager.cpp \
    genericparametrs.cpp \
    main.cpp \
    processingexception.cpp \
    tablesortfilterwindow.cpp \
    tests/loanaccountertest.cpp

HEADERS += \
    accounter.h \
    addnewchangedeleteitemwindow.h \
    addnewitemwindow.h \
    changedeleteitemwindow.h \
    creditcontract.h \
    dbmanager.h \
    genericparametrs.h \
    processingexception.h \
    tablesortfilterwindow.h \
    tests/loanaccountertest.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
