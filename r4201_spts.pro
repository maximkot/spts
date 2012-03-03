#spts main project file
#any change in project
include (../taskconfig/taskconfig.pri)
include (./src/sziinifile/sziinifile.pri)
include (./setdirs.pri)

# INCLUDEPATH += ../taskconfig/include/
QT += sql
TARGET = r4201_spts
TEMPLATE = app
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/edittemplate.cpp \
    src/edittemplatestring3.cpp \
    src/edittemplatestring2.cpp \
    src/edittemplatestring1.cpp \
    src/modulevar.cpp \
    src/moduleproc.cpp \
    src/newts.cpp \
    src/find.cpp \
    src/p_sgapred.cpp \
    src/p_sgaspusk.cpp \
    src/p_npaspusk.cpp \
    src/p_npapred.cpp \
    src/p_gvspred.cpp \
    src/p_gvsspusk.cpp \
    src/setcolor.cpp \
    src/setsound.cpp
HEADERS += src/mainwindow.h \
    src/edittemplate.h \
    src/edittemplatestring3.h \
    src/edittemplatestring2.h \
    src/edittemplatestring1.h \
    src/modulevar.h \
    src/moduleproc.h \
    src/moduleconst.h \
    src/newts.h \
    src/find.h \
    src/p_sgapred.h \
    src/p_sgaspusk.h \
    src/p_npaspusk.h \
    src/p_npapred.h \
    src/p_gvspred.h \
    src/p_gvsspusk.h \
    src/setcolor.h \
    src/setsound.h
FORMS += src/mainwindow.ui \
    src/edittemplate.ui \
    src/edittemplatestring2.ui \
    src/edittemplatestring3.ui \
    src/edittemplatestring1.ui \
    src/newts.ui \
    src/p_sgapred.ui \
    src/p_sgaspusk.ui \
    src/p_npaspusk.ui \
    src/p_npapred.ui \
    src/p_gvspred.ui \
    src/p_gvsspusk.ui \
    src/setcolor.ui \
    src/setsound.ui
RESOURCES += progicons.qrc
CONFIG(debug)
 { 
    DEFINES += TASKCONFIG_BIN \
        TASKCONFIG_RESCUE
    DEFINES -= PROP_DEBUG
}
