#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QColor>
#include <QString>
#include <QtSql>
#include <QDir>
#include <QMessageBox>
#include <QTranslator>
#include <QObject>
#include <QFile>
#include <QSplitter>
#include <QWidget>
#include <QKeyEvent>
#include <QShortcut>
#include <QKeySequence>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QAbstractProxyModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QPaintDevice>


#include "taskconfig.h"
#include "splashform.h"
#include "tsziinifile.h"
#include "mainwindow.h"
#include "modulevar.h"
#include "moduleconst.h"
#include "moduleproc.h"



//------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Зададим проекту кодировку
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QTranslator translator;
    translator.load(QString("qt_ru"), ":/new/btn_rus/src/qtru/");
    a.installTranslator(&translator);

     // Реховские синглтоны
    // Инициализация приложения, мин и макс уровни секретности
    if (!qTC->init(ProgName,0,0))
      return 0;
    // Подключение к базе
    if (!qTC->dbConnect(ZadName.toLatin1()))
      return 0;


    // Устанавливаем основной файл системы помощи
    qTC->setMainHelpFile(ProgName.toLatin1().append(".html"));

    QDir Proverka = getStatDirProgSetFilePath();

/*    //если динамический каталог не существует - лезем смотреть каталог со статической инфой
    if (!getDinDirProgSetFilePath().exists())
    {
        // если и статической директории на существует, то грязно ругаемся
        if (!getStatDirProgSetFilePath().exists())
        {
        QMessageBox::critical (0, QObject::tr("Запуск программы"), msgErrorNoStaticDir.toLatin1());


        }
// Создавать самому динамич./статич. директории или нет - пока неизвестно
//        QString newDir;
//        if (!dir.mkdir(newDir))
//        QMessageBox::critical (0, QObject::tr("Запуск программы"), msgErrorCreateProgRabDir.toLatin1());
    }

*/
    // Загрузим из БД коды типов ТС
    if (!LoadTSidFromBase())
      return 1;




    // считываем настройки цветов и звуков из конфигурационного файла
    TSziIniFile localIniFile (getDinStrProgSetFilePath().toUtf8());

    if (localIniFile.getLastError() < 0)
    {
        QMessageBox::critical (0, QObject::tr("Запуск программы"), msgErrorReadSettings.toLatin1());
        return 0;
    }

    localIniFile.beginGroup("COLORSET");
    setPohodColor(localIniFile.value("pohod", defaultPohodColor).toUInt());
    setReadyColor(localIniFile.value("ready", defaultReadyColor).toUInt());
    setNotReadyColor(localIniFile.value("notready", defaultNotReadyColor).toUInt());
    setAvarColor(localIniFile.value("avar", defaultAvarColor).toUInt());
    setSelectColor(localIniFile.value("select", defaultSelectColor).toUInt());
    localIniFile.endGroup();

    localIniFile.beginGroup("SOUNDSET");
    setSoundOn(localIniFile.value("sound_on", true).toBool());
    setNumSoundRepeat(localIniFile.value("repeat", 1).toInt());
    setLastSoundPath(localIniFile.value("LastSoundPath", qTC->getWorkPath()).toString());

    localIniFile.endGroup();
    localIniFile.beginGroup("SOUNDSETMODE");
    setAllSoundOn(0,0,localIniFile.value("sound_on_sgaPred", true).toBool());
    setAllSoundOn(0,1,localIniFile.value("sound_on_sgaSpusk", true).toBool());
    setAllSoundOn(0,2,localIniFile.value("sound_on_sgaPosle", true).toBool());
    setAllSoundOn(1,0,localIniFile.value("sound_on_gvsPred", true).toBool());
    setAllSoundOn(1,1,localIniFile.value("sound_on_gvsSpusk", true).toBool());
    setAllSoundOn(1,2,localIniFile.value("sound_on_gvsPosle", true).toBool());
    setAllSoundOn(2,0,localIniFile.value("sound_on_npaPred", true).toBool());
    setAllSoundOn(2,1,localIniFile.value("sound_on_npaSpusk", true).toBool());
    setAllSoundOn(2,2,localIniFile.value("sound_on_npaPosle", true).toBool());
    localIniFile.endGroup();


    // Отображаем главное окно
    MainWindow mw;
    mw.show();

    a.exec();

    // перед завершением программы сохраним настройки цвета и звука в файл конфигурации
    localIniFile.reloadIniFile();

    localIniFile.beginGroup("COLORSET");
    localIniFile.setValue("pohod", getPohodColor().rgb());
    localIniFile.setValue("ready", getReadyColor().rgb());
    localIniFile.setValue("notready", getNotReadyColor().rgb());
    localIniFile.setValue("avar", getAvarColor().rgb());
    localIniFile.setValue("select", getSelectColor().rgb());
    localIniFile.endGroup();

    localIniFile.beginGroup("SOUNDSET");
    localIniFile.setValue("sound_on", getSoundOn());
    localIniFile.setValue("repeat", getNumSoundRepeat());
    localIniFile.setValue("LastSoundPath", getLastSoundPath());
    localIniFile.endGroup();
    localIniFile.beginGroup("SOUNDSETMODE");
    localIniFile.setValue("sound_on_sgaPred",  getAllSoundOn(0,0));
    localIniFile.setValue("sound_on_sgaSpusk", getAllSoundOn(0,1));
    localIniFile.setValue("sound_on_sgaPosle", getAllSoundOn(0,2));
    localIniFile.setValue("sound_on_gvsPred",  getAllSoundOn(1,0));
    localIniFile.setValue("sound_on_gvsSpusk", getAllSoundOn(1,1));
    localIniFile.setValue("sound_on_gvsPosle", getAllSoundOn(1,2));
    localIniFile.setValue("sound_on_npaPred",  getAllSoundOn(2,0));
    localIniFile.setValue("sound_on_npaSpusk", getAllSoundOn(2,1));
    localIniFile.setValue("sound_on_npaPosle", getAllSoundOn(2,2));
    localIniFile.endGroup();

    return 0;
}

//------------------------------------------------------------------------------------------------------
