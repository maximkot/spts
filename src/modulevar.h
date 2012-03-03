#ifndef MODULEVAR_H
#define MODULEVAR_H

#include <QString>
#include <QWidget>
#include <QtSql>
#include <QSound>
#include <QSqlDatabase>

//----------------------------------------------------------------------------------------------


// Структура описывающая текущее ТС
struct currentTS
 { QString typeTS;   // Наименование типа ТС
   int typeTSid;     // Код типа ТС
   QString nameTS;   // Наименование ТС
   int nomTS;        // № ТС
   int zavnomTS;     // Заводской № ТС
 };

currentTS getCurrentTS();
void setCurrentTS(QString tpTS, int tpTSid, QString nmTS, int noTS, int zvTS);

// Текущий режим работы (1-СГА, 2-ЖВС, 3-НПА)
void setRabMode(int RabMode);
int getRabMode();

// Текущий режим работы для текущего ТС (1-Предспуск, 2-Спуск, 3-Послеспуск)
void setRabModeCurrentTS(int RabModeCurrentTS);
int getRabModeCurrentTS();


void setAllSoundOn(int i,int j, bool k);
bool getAllSoundOn(int i, int j);


// Получение наименований и кодов типов ТС---------------
// Наименования типов ТС заковали в константы в moduleconst.h, до тех далеких времен,
// пока не появится возможность разобраться с проблемами кодировок при загрузке из БД
void setTSBDD(QString tstype);
QString getTSBDD();
void setTSGVS(QString tstype);
QString getTSGVS();
void setTSNPA(QString tstype);
QString getTSNPA();
void setTSSGA(QString tstype);
QString getTSSGA();
void setTSSJO(QString tstype);
QString getTSSJO();

// Сеттеры-Геттеры Кодов Типов ТС
void setTSBDDid(int idtstype);
int getTSBDDid();
void setTSGVSid(int idtstype);
int getTSGVSid();
void setTSNPAid(int idtstype);
int getTSNPAid();
void setTSSGAid(int idtstype);
int getTSSGAid();
void setTSSJOid(int idtstype);
int getTSSJOid();


// Функции получения путей-----------------------
QString getDinStrProgSetFilePath();
QDir getDinDirProgSetFilePath();
QString getDinStrFormSetFilePath();
QDir getDinDirFormSetFilePath();

QString getStatStrProgSetFilePath();
QDir getStatDirProgSetFilePath();
QString getStatStrFormSetFilePath();
QDir getStatDirFormSetFilePath();


// Функции установки/получения цветов------------
QColor getPohodColor();
void setPohodColor (const QColor color);

QColor getReadyColor();
void setReadyColor (const QColor color);

QColor getNotReadyColor();
void setNotReadyColor (const QColor color);

QColor getAvarColor();
void setAvarColor (const QColor color);

QColor getSelectColor();
void setSelectColor (const QColor color);


 // Звук и всё, что с ним связано----------------
// Функции установки/получения пути к ф-лам звуков
QString getAvarSoundPath();
void setAvarSoundPath(QString avarsoundfile);

// Звук Вкл/выкл
bool getSoundOn();
void setSoundOn(bool SoundOn);

// Количество повторений звука
int getNumSoundRepeat();
void setNumSoundRepeat(int numsoundrepeat);

// Путь к послежнему открытому звуковому файлу
void setLastSoundPath(QString SoundPath);
QString getLastSoundPath();

// База------------------------------------------
QSqlDatabase getDbName();

//----------------------------------------------------------------------------------------------

#endif // MODULEVAR_H
