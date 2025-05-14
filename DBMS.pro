QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Constraint.cpp \
    FieldControl.cpp \
    dml_processor.cpp \
    logui.cpp \
    main.cpp \
    mainwindow.cpp \
    qdatabase.cpp \
    sqlparser.cpp \
    storage.cpp \
    storageengine.cpp \
    table.cpp \
    table_meta.cpp \
    widget.cpp

HEADERS += \
    Database.h \
    QDatabase.h \
    common.h \
    dml_processor.h \
    exceptions.h \
    logui.h \
    mainwindow.h \
    sqlparser.h \
    storage.h \
    storageengine.h \
    table.h \
    table_meta.h \
    widget.h

FORMS += \
    mainwindow.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CMakeLists.txt \
    CMakeLists.txt.user \
    DBMS.pro.user \
    README.md \
    user.txt
