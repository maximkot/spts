#########################################################
#SZIInifiles                                            #
#########################################################

INCLUDEPATH*=$$PWD
DEPENDPATH*=$$PWD

SOURCES*=$$PWD/tsziinifile.cpp
HEADERS*=$$PWD/tsziinifile.h

unix{
SOURCES*=$$PWD/tszifile.cpp
HEADERS*=$$PWD/tszifile.h
LIBS*=-lnomacl
}
