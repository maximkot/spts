
// $Id: summon_help.cpp 233 2009-10-20 06:14:48Z polyakovda $

#include <QtCore>
#include <QProcess>

#ifdef Q_OS_UNIX
bool isKgodStarted(QString progname){
QProcess kgod;
QStringList args;
args << "-remote";
args << "ping\(\)";

kgod.start(progname, args);
kgod.waitForFinished();
int res = kgod.exitCode();

//kgod return '1' - if running and '2' if not
if (res<2) return true;
else return false;
}



bool summon_help(QString progname, QString help_file)
{

    QProcess process;
    QFileInfo filename(progname);
    QStringList progArgs;

    if (filename.baseName()=="kgod") {
        QStringList kgodArgs;
            if (isKgodStarted(progname)) kgodArgs << "-remote" << "openUrl\("+help_file+",new-tab\)";
                else kgodArgs << help_file;
    progArgs = kgodArgs;
    }
    else progArgs << help_file;

    return process.startDetached(progname,progArgs);
}

#else
#include <windows.h>

bool summon_help(QString progname, QString help_file)
{
    return (int) (ShellExecuteA(0,"open",progname.toLocal8Bit(),help_file.toLocal8Bit(),"",SW_SHOW)) > 32;
}

#endif
