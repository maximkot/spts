#ifndef TSZIINIFILE_H
#define TSZIINIFILE_H

/*
 * Модуль предназначен для работы с ini-файлами.
 * Поддерживается кроссплатформенность, что позволяет пользоваться модулем
 * под ОС Windows и ОС МСВС без изменения кода.
 * Работа с файлами под ОС МСВС производится через службу доступа к файлам nomacl.
 * Можно обойти использование этой службы выставив переменную useNomacl в false.
 * Все функции данного класса совпадают с функциями класса QSettings.
 *
 * При отсутсвии соединения со службой nomacl, каких-либо ошибок ввода/вывода
 * исключительных ситуаций возникать не будет. Класс вернет значения по умолчанию.
 * Признаком ошибки при выполнении операций будет значение QSettings::AccessError,
 * возвращаемое функцией status(). В таком случае конкретный код ошибки можно
 * получить функцией getLastError();
 *
 *
 * $Id: tsziinifile.h 231 2009-10-05 07:05:02Z griffin $
 */

#include <QtCore>
#ifdef Q_OS_UNIX
#include <tszifile.h>
#endif

class TSziIniFile
{
public:
    TSziIniFile(QString iniFileName);
    TSziIniFile(QString iniFileName, const char *codecName);
    TSziIniFile(QString iniFileName, QTextCodec * codec);
    ~TSziIniFile();
    void setValue(QString key, QVariant value);
    QVariant value(QString key, QVariant defaultValue=QVariant());
    void beginGroup(QString prefix);
    void endGroup();
    bool sync();
    //    void setFileName(QString fileName);
    QString fileName();
    QString group();
    void setIniCodec(QTextCodec* codec);
    void setIniCodec(const char* codecName); //Устанавливает кодировку для работы с ini-файлом
    QSettings::Status status();
    void clearAll(); //Очишает все значения
    bool reloadIniFile(); //Перегружает ini-файл с диска
    int getLastError();

private:
    bool pendingChanges;
    QTextCodec *iniCodec;
    QString currentGroup;
    QString iniFileName;
    QTextStream *iniFile;
    QHash <QString, QHash<QString,QVariant> > groups;
    int lastError;
    void initialize(QString iniFileName, QTextCodec * codec);
    void checkGroupExists(QString prefix);
    bool loadIniFile();
    bool openFileRead();
    bool openFileWrite();
    void closeFile();

    bool iniUnescapedStringList(const QByteArray &str, int from, int to,
                                              QString &stringResult, QStringList &stringListResult,
                                              QTextCodec *codec);
    void iniEscapedStringList(const QStringList &strs, QByteArray &result, QTextCodec *codec);
    void iniEscapedString(const QString &str, QByteArray &result, QTextCodec *codec);
    bool iniUnescapedKey(const QByteArray &key, QString &result);
    void iniEscapedKey(const QString &key, QByteArray &result);
    QVariant stringToVariant(const QString &s);
    QString variantToString(const QVariant &v);
    QStringList splitArgs(const QString &s, int idx);
    QStringList variantListToStringList(const QVariantList &l);
    QVariant stringListToVariantList(const QStringList &l);

#ifdef Q_OS_UNIX
    TSziFile *sziFile;
#endif
    QFile *ifile;
};

extern bool useNomacl;

#endif // TSZIINIFILE_H
