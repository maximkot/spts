// $Id: taskconfig.h 575 2011-11-18 11:15:38Z robkovai $

/*
Класс основных настроек приложения обеспечивает:
  - проверку уровня сеанса;
  - настройку на пути необходимые для работы приложения (рабочий каталог, каталог со статическими данными, настройку на приложения отображения справки и выходных документов);
  - соединение с БД (также отображение сплэша);
  - загрузку и сохранение параметров форм, таблиц и т.д.;
  - работу диалога настроек отображения реквизитов таблиц;
  - настройку и запуск маскирующего удаления;
  - отображение окна "О программе..."
*/

//Директивы для настройки режима класса (прописываются в файле проекта *.pro)
/*
  PROP_DEBUG   - настройки приложения для дебага
  TASKCONFIG_BIN                - класс собирается для исполняемого модуля;
  TASKCONFIG_RESCUE             - класс собирается для ОКР Растворение (если данная деректива не указана, то сборка производится для ОКР Юпитер-СТО;
  Пример подключения:
  DEFINES += TASKCONFIG_BIN TASKCONFIG_RESCUE
*/


//Использование класса
/*
	Для использования класса ипользовать макрос qTC который возвращает указатель на TaskConfig
	класс оформлен как синглтон
	пример использования qTC
	void main()
	{
		QApplication a;				
		if( !qTC->init( "settings", 0, 1 ) ) return 0;
		if( !qTC->dbConnect( "Пример использования TaskConfig" ) ) return 0;
                if( !qTC->macConfirm( "tablename" ) ) return 0;
		QMainWindow mw;
		mw.show();
		return a.exec();
	}	
*/

//Пример конфига
/*
  [Connection]
  Database=
  HostName=localhost
  [viewers]
  helpBrowser=firefox
  outFormBrowser=firefox
  xalanPath=/usr/bin/
  secdel=secdel -d -o
  outFormPath=../../outForms/
*/

#ifndef TASKCONFIG_H
#define TASKCONFIG_H

#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QTextCodec>
#include <QStandardItemModel>

#include <QtCore/qglobal.h>

#include "tsziinifile.h"
#include "settingstable.h"
#include "usrlist.h"
#include "about.h"
#include "waitwidget.h"
//#include "find.h"

/*#if defined(TASKCONFIG_BIN)
#  define TASKCONFIG_EXPORT Q_DECL_EXPORT
#else
#  define TASKCONFIG_EXPORT Q_DECL_IMPORT
#endif*/

//Описание структуры настроек для TableView и TreeView по умолчанию
struct DefaultColumnItem
{
    const char* name;
    const char* title;
    int width;
    bool visible;
    bool stretch;
};

Q_DECLARE_METATYPE ( DefaultColumnItem *)

//Пример описания таблиц по умолчанию
/*const DefaultColumnItem defaultColumnsTableView[] =
{//   fieldName,    name,                       width,  visible,    stretch
    { "fieldname",  "titlename", 0,      true,       false },
    { NULL,         NULL,                       0,      false,      false }
};*/

class /*TASKCONFIG_EXPORT*/ TaskConfig: public QObject //Класс настроек приложения
{
    Q_OBJECT
    //Q_DECLARE_PRIVATE( TaskConfig );
public:    
    //virtual ~TaskConfig();

    static TaskConfig *setExistsTC( TaskConfig *tc ); //Указать существующий класс (использовать в динамических библиотеках *.dll, *.so)
    static TaskConfig *instance( QObject *parent = qApp ); //Возвращает указатель на екземпляр

    bool init( QString cName = qAppName(), int minMacLevel = 0, int maxMacLevel = 0 ); //Инициализация
    bool dbConnect( QString progName ); //Соединение с БД
    bool macConfirm( QString tableName ); //Проверка соответствия уровня сеанса и меток базы по указанной таблице

    QString getWorkPath(); //Возвращает рабочий каталог приложения
    QString getStaticDataPath(); //Возвращает каталог со статическими данными приложения
    QString getConfigPath(); //Возвращает католог нахождения файла основных настроек приложения
    
    int getMacLevel(); //Возвращает уровень сеанса

    //далее moduleName - имя создаваемого конфига

    void formSave( QWidget *form, QString moduleName, bool mainWindow = false ); //Сохранение настроек формы
    void formLoad( QWidget *form, QString moduleName, bool mainWindow = false ); //Загрузка настроек формы

    void tableSave( QTableView *tableView, QString moduleName = "grids", QSqlQueryModel *qModel = 0 ); //Сохранение настроек отображения реквизитов TableView
    void tableLoad( QTableView *tableView, const DefaultColumnItem *defaultColumnItems, QString moduleName = "grids", QSqlQueryModel *qModel = 0 ); //Загрузка настроек отображения реквизитов TableView
    
    void hHeaderTableSave( QTableView *tableView, QStandardItemModel *headerModel, QString moduleName = "grids" ); //Сохранение настроек отображения иерархических заголовков таблиц
    void hHeaderTableLoad( QTableView *tableView, QStandardItemModel *headerModel,
                           const DefaultColumnItem *defaultColumnItems, QString moduleName = "grids" ); //Загрузка настроек отображения иерархических заголовков таблиц

    void treeSave( QTreeView *treeView, QString moduleName ); //Сохранение настроек отображения реквизитов TreeView
    void treeLoad( QTreeView *treeView, const DefaultColumnItem *defaultColumnItems, QString moduleName ); //Загрузка настроек отображения реквизитов TreeView

    void hHeaderTreeSave( QTreeView *treeView, QStandardItemModel *headerModel, QString moduleName ); //Сохранение настроек отображения иерархических заголовков деревьев
    void hHeaderTreeLoad( QTreeView *treeView, QStandardItemModel *headerModel,
                          const DefaultColumnItem *defaultColumnItems, QString moduleName ); //Загрузка настроек отображения иерархических заголовков деревьев

    void executeBrowser( QString fileName, bool help = true ); //Запуск программы просмотра справок и выходных форм
    void executeXalan( QString htmlFileName, QString xmlFileName, QString xsltFileName ); //Запуск Xalan'а

    QString sortTable(QTableView *table, const DefaultColumnItem *defaultColumnItems);
    //void showFinder(QTableView *table);
    void showSettingsTable(QTableView *table); //Отображение диалога настроек реквизитного состава

    bool fileSecDel( QString filename ); //Запуск маскирующего удаления

    void setMainHelpFile( QString mHF ); //Указание основного файла справки (где содержание)
    void showMainHelp(); //Отображение основного файла справки (где содержание)

    void delRec(QString table, QString column, QTableView *tableView, bool confirm = true);
    void delRec(QString table, QString column, QString value);

    void showAbout(QString text, QString version); //Отображение окна "О программе..."
    UsrList* showUsr(QWidget *parent, bool showed=true); //возвращает виджет настроек отображения по пользователям

    bool confirm(QString question, QString title = " ");

    void waitStart(QWidget *parent = 0, bool needRepaint = false, QString text = QString());
    void waitSetText(QString text = QString());
    void waitFinish();
protected:
    static TaskConfig *self;

    TaskConfig( QObject *parent );

private:
    QTextCodec *codec;
    QTranslator translator;
    QSqlDatabase db;

    QString configPath;     //Путь к основному конфигу
    QString configName;     //Имя основного конфига
    QString workPath;       //Рабочий каталог приложения
    QString staticDataPath; //Путь к статическим данным (справка, шаблоны и т.д.)
    QString xalanPath;      //Путь к Xalan'у
    QString outFormPath;    //Путь к шаблонам
    QString outFormBrowser; //Приложение отображения выходных форм
    QString helpBrowser;    //Приложение отображения справки
    QString secdel;         //Программа маскирующего удаления

    QString mainHelpFile;

    void saveChildrenObject( TSziIniFile &childrenSettings, QObject *parent );
    void loadChildrenObject( TSziIniFile &childrenSettings, QObject *parent );
    int getDefaultColumnItem( QString nameField, const DefaultColumnItem *defaultColumnItems );

    QString wordWrapOnResized( QString headerLabel, int newRectSize );
private slots:
    void tableView_sectionResized( int logicalIndex, int, int newSize );
    void treeView_sectionResized( int logicalIndex, int, int newSize );
};

//Доступ к TaskConfig через qTC
#if defined(qTC)
#undef qTC
#endif
#define qTC TaskConfig::instance()

#endif // TASKCONFIG_H
