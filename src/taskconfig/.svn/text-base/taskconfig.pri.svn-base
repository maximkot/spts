# Для корректного включения TaskConfig в проект следует скопировать
# *.pri, *.cpp и *.h в каталог taskconfig который находится на
# том же уровне шо и каталог проекта. В .pro-файл проекта включить строчку
# вида 'include (../taskconfig/taskconfig.pri)'

# $Id: taskconfig.pri 575 2011-11-18 11:15:38Z robkovai $

include( ../sziinifile/sziinifile.pri )

INCLUDEPATH += ../taskconfig

SOURCES += ../taskconfig/taskconfig.cpp \
           ../taskconfig/settingstable.cpp \
           ../taskconfig/splashform.cpp \
           ../taskconfig/hheadersqlquerymodel.cpp \
           ../taskconfig/HierarchicalHeaderView.cpp \
           ../taskconfig/summon_help.cpp \
           ../taskconfig/about.cpp \
           ../taskconfig/usrlist.cpp \
           ../taskconfig/waitwidget.cpp

HEADERS += ../taskconfig/taskconfig.h \
           ../taskconfig/settingstable.h \
           ../taskconfig/splashform.h \
           ../taskconfig/hheadersqlquerymodel.h \
           ../taskconfig/HierarchicalHeaderView.h \
           ../taskconfig/summon_help.h \
           ../taskconfig/about.h \
           ../taskconfig/usrlist.h \
           ../taskconfig/waitwidget.h

FORMS += ../taskconfig/settingstable.ui \
         ../taskconfig/splashform.ui \
         ../taskconfig/about.ui \
         ../taskconfig/usrlist.ui \
         ../taskconfig/waitwidget.ui

RESOURCES += ../taskconfig/splashicons.qrc \
             ../taskconfig/waitwidget.qrc

unix {
    UNAME = $$system(uname -a)
    IS_MCBC = $$find(UNAME, vniins)
    !isEmpty(IS_MCBC) {
        message("Enable mac support for MCBC.")
#        DEFINES += Q_OS_MCBC
        LIBS += -lmac
#        HEADERS += mac.h
    }
}

#CONFIG(debug) {
# DEFINES += PROP_DEBUG
#}
