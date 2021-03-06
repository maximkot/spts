#include "moduleproc.h"
#include "modulevar.h"
#include "moduleconst.h"
#include "mainwindow.h"
#include <QSqlQuery>
#include <QMessageBox>

//-------------------------------------------------------------------------------------------------

// Создаём конструктор нашего звукового потока
SoundThread::SoundThread (QObject *parent) : QThread(parent)
{
    avSound = new QSound(getLastSoundPath(), this);
}

//-------------------------------------------------------------------------------------------------

// Создаём деструктор нашего звукового потока
SoundThread::~SoundThread()
{
    delete avSound;
}

//-------------------------------------------------------------------------------------------------

// То, что будет крутиться в этом потоке, задаём тут
void SoundThread::run()
{
    // Если общий звук включен, то играем его
    if (getSoundOn() == true)
    {
        // Смотрим, в каком мы режиме и разрешен ли для данного вида работ аварийный сигнал
//        if (getAllSoundOn(getRabMode(), j) == true)
//        {
            avSound->setLoops(getNumSoundRepeat());
            avSound->play();
//        }
    }
}

//-------------------------------------------------------------------------------------------------

// Считывает из БД коды и наименования типов ТС и возвращает false, если вдруг ошибка
bool LoadTSidFromBase()
{

    QSqlQuery TSQuery;
    QString TStype;
    int TSid;

    TSQuery.prepare("SELECT codets,namets FROM z18_spts.kls_ts ORDER BY codets");
    if (TSQuery.exec() == false)
    {
        QMessageBox::critical (0, QObject::tr("Загрузка параметров"), "Не удалось считать из БД параметры необходимые для работы программы/nПриложение будет закрыто");
        return false;
    }
    else
    {
        if (TSQuery.size() != 5)
        {
        QMessageBox::critical (0, QObject::tr("Загрузка параметров"), msgErrorCodeTableTS.toLatin1());
        return false;
        }
    }

    // если всё хорошо, переходим к первой записи и начинаем считывать из БД
    TSQuery.first();

    // Считываем 1й тип (СГА)
    TSid = TSQuery.value(0).toInt();
    TStype = TSQuery.value(1).toString();
    setTSSGAid(TSid);
    setTSSGA(TStype);
    TSQuery.next();

    // Считаем 2й тип (ЖВС)
    TSid = TSQuery.value(0).toInt();
    TStype = TSQuery.value(1).toString();
    setTSGVSid(TSid);
    setTSGVS(TStype);
    TSQuery.next();

    // Считаем 3й тип (БДД)
    TSid = TSQuery.value(0).toInt();
    TStype = TSQuery.value(1).toString();
    setTSBDDid(TSid);
    setTSBDD(TStype);
    TSQuery.next();

    // Считаем 4й тип (НПА)
    TSid = TSQuery.value(0).toInt();
    TStype = TSQuery.value(1).toString();
    setTSNPAid(TSid);
    setTSNPA(TStype);
    TSQuery.next();

    // Считаем 5й тип (СЖО)
    TSid = TSQuery.value(0).toInt();
    TStype = TSQuery.value(1).toString();
    setTSSJOid(TSid);
    setTSSJO(TStype);
    TSQuery.next();

    return true;
}
//-------------------------------------------------------------------------------------------------

int MaxCode(QString TabName, QString FieldName)
{
    QSqlQuery MaxSQL(getDbName());

    MaxSQL.prepare(QString("SELECT MAX(%1) AS maxnump FROM %2").arg(FieldName,TabName));
    if (!MaxSQL.exec())
    {
        QMessageBox::critical (0, QObject::tr("Добавление записи"), "Ошибка вычисления максимального индекса записи в таблице");
        return -1;
    }

    MaxSQL.first();
    int MaxNumID = MaxSQL.value(0).toInt();
    return MaxNumID;

}

//-------------------------------------------------------------------------------------------------
