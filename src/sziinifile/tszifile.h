#ifndef TSZIFILE_H
#define TSZIFILE_H

#include <QtCore>
#include <QtGui>

/* Описание:
 * Модуль для доступа к файлам через службу доступа nomacl.
 * Класс TSziFile наследован от QIODevice и поэтому может использоваться
 * со стандартными механизмами доступа к данным библиотеки Qt позволяющими
 * работать через QIODevice.
 *
 * Использование:
 * Конструкторы TSziFile() и TSziFile(QObject) существуют только для совместимости,
 * и их использование в явном виде бессмысленно.
 * Функция open() вызывается с параметрами типа QIODevice::OpenModeFlag и QFile::OpenModeFlag
 * Следует использовать только ::ReadOnly, ::WriteOnly и ::ReadWrite. Все остальные значения
 * могут привести к возникновению ошибочных ситуаций.
 *
 * Примеры использования класса приведены в соответствующем разделе Wiki на server-mcbc.
 *
 *
 * $Id: tszifile.h 240 2009-10-30 09:07:12Z griffin $
 */

class TSziFile : public QIODevice
{

public:
    TSziFile();
    explicit TSziFile( QObject * parent );
    TSziFile( QString fileName);
    ~TSziFile();

    bool open ( OpenMode mode ); //Открытие файла
    bool open_creat(OpenMode mode, mode_t cr_mode); //Открытие или создание файла
    bool creat (mode_t cr_mode); //Создание файла
    void close ();

    bool seek(qint64 pos); //Смещение указателя в файле
    bool isSequential();

    int resize (qint64 sz); //Обрезает файл до указанного размера
    QFile::FileError error(); //Код ошибки в/в полученный из nomacl
    bool isNomaclConnected(); //Подключен ли nomacl

    bool exists (QString name); //Существует ли файл
    int euidaccess(QString name, int type);
    int mkdir(QString name); //Создание каталога
    int copy(QString oldname, QString newname); //Копирование файла
    int last_nomacl_error();

protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    bool isConnected; //is connected to nomacle
    QString fileName;
    int fileDesc; //file descriptor
    int nomacl_error;

    void connect_nomacl();
};

extern int nomacl_conn_num;

#endif // TSZIFILE_H
