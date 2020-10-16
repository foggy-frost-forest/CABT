QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ackmp.cpp \
    avl.cpp \
    bst.cpp \
    heap.cpp \
    infowidget.cpp \
    main.cpp \
    nodebutton.cpp \
    pst.cpp \
    sgt.cpp \
    widget.cpp

HEADERS += \
    ackmp.h \
    avl.h \
    bst.h \
    heap.h \
    infowidget.h \
    nodebutton.h \
    orderlist.hpp \
    pst.h \
    sgt.h \
    widget.h

FORMS += \
    ackmp.ui \
    avl.ui \
    bst.ui \
    heap.ui \
    infowidget.ui \
    pst.ui \
    sgt.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
