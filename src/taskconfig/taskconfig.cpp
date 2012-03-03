// $Id: taskconfig.cpp 575 2011-11-18 11:15:38Z robkovai $

#include "taskconfig.h"

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QObject>
#include <QProcess>
#include <QtGui/QSplitter>
#include <QtGui/QToolBar>
#include <QtGui/QMainWindow>
//#include <QHeaderView>
#include <QDebug>

#include "summon_help.h"

#ifdef TASKCONFIG_BIN
#include "splashform.h"
#endif

#ifdef Q_OS_MCBC
#include "mac.h"
#endif

//#ifdef TASKCONFIG_BIN
TaskConfig *TaskConfig::self = 0;
//#endif

TaskConfig::TaskConfig( QObject *parent )
        : QObject( parent )
{
    codec = QTextCodec::codecForName( "UTF-8" );
    QTextCodec::setCodecForTr( codec );
    QTextCodec::setCodecForCStrings( codec );
    //initTC();
#ifdef PROP_DEBUG
    qDebug() << "TaskConfig::TaskConfig()";
#endif
}

TaskConfig *TaskConfig::setExistsTC( TaskConfig *tc )
{
    if( tc )
        TaskConfig::self = tc;

    return TaskConfig::self;
}
TaskConfig *TaskConfig::instance( QObject *parent )
{
    if( !TaskConfig::self )
        TaskConfig::self = new TaskConfig( parent );
    return TaskConfig::self;
}

int TaskConfig::getMacLevel()
{
#ifdef Q_OS_MCBC
    struct mac_label ml;
    getmac (&ml);
    return ml.lev;
#endif
    return -1;
}

bool TaskConfig::init( QString cName, int minMacLevel, int maxMacLevel )
{
    configName = cName;
#ifdef Q_OS_UNIX
#ifdef Q_OS_MCBC
    struct mac_label ml;
    getmac (&ml);
//    qDebug() << ml.lev;
    if( !(ml.lev >= minMacLevel && ml.lev <= maxMacLevel) )
    {
        QMessageBox::critical( 0, QObject::tr( "Ошибка" ),
                               QObject::tr( "Уровень сеанса не соответствует условиям работы программы" ) );
        return false;
    }
#endif
#ifdef PROP_DEBUG
   staticDataPath = qApp->applicationDirPath()+"/";
   workPath = staticDataPath;
   configPath = staticDataPath;
   xalanPath = "";
   helpBrowser = "firefox";
   outFormBrowser = "firefox";
   outFormPath = staticDataPath+"outForms/";
   secdel = "";
#else
#ifdef TASKCONFIG_RESCUE
   configPath = "/etc/rast/";
#else
   configPath = "/etc/usto/";
#endif
   staticDataPath = "/usr/local/"+qAppName()+"/";
   QDir homeDir( QDir::homePath()+"/."+qAppName() );
   if( !homeDir.exists() )
       homeDir.mkdir( QDir::homePath()+"/."+qAppName() );
   workPath = QDir::homePath()+"/."+qAppName()+"/";
   xalanPath = "";
   helpBrowser = "/usr/bin/kgod";
   outFormBrowser = "/usr/bin/kgod";
   outFormPath = staticDataPath+"outForms/";
   secdel = "secdel -d -o";

/*#ifdef USE_NOMACL
   TSziFile sziFile;
   if( sziFile.exists( configPath+configName+".cfg" ) )
   {
       QMessageBox::critical( 0, QObject::tr( "Ошибка" ), QObject::tr( "Файл "+configPath+configName+".cfg"+" несуществует" ) );
       isSettings = false;
       return false;       
   }
#endif*/
#endif
#else
   char *dynamicInfoPath = getenv ("HOMEPATH");
   char *dynamicDrive = getenv ("HOMEDRIVE");
   workPath = QString (dynamicDrive).append(QString (dynamicInfoPath));
   workPath.replace( "\\", "/" );

   char *staticInfoPath = getenv("ALLUSERSPROFILE");
   staticDataPath = QString (staticInfoPath);
   staticDataPath.replace( "\\", "/" );

#ifdef TASKCONFIG_RESCUE   

   workPath.append("/Local Settings/Application Data/RAST" );
   QDir rastDir( workPath );
   if( !rastDir.exists() )
       rastDir.mkdir( workPath );
   workPath.append("/.").append(qAppName()).append("/");   
   configPath = staticDataPath.append("/Application Data/RAST/");
   staticDataPath.append(qAppName()).append("/");
#else
//   workPath.append("/Local Settings/Application Data/").append(qAppName()).append("/");
   workPath = qApp->applicationDirPath()+"/";
   staticDataPath = workPath;
   configPath = staticDataPath;
#endif
//   staticDataPath = workPath;
//   configPath = staticDataPath;
   QDir homeDir( workPath );
   if( !homeDir.exists() )
       homeDir.mkdir( workPath );
   xalanPath = qApp->applicationDirPath()+"/xalan/";
   helpBrowser = "iexplore";
   outFormBrowser = "iexplore";
   outFormPath = staticDataPath+"outForms/";
   secdel = "";
#endif
   //if (minMacLevel < maxMacLevel) {}
   translator.load( "qt_ru", staticDataPath );
   qApp->installTranslator(&translator);

   TSziIniFile settings( configPath+configName+".cfg" );
   /*if( settings.getLastError()!=0 )
   {
       QMessageBox::critical( 0, QObject::tr( "Ошибка" ), QObject::tr( "Невозможно открыть файл: " )+configPath+configName+".cfg" );
       return false;
   }*/
   settings.beginGroup( "viewers" );
   xalanPath = settings.value( "xalanPath", xalanPath ).toString();
   outFormPath = settings.value( "outFormPath", outFormPath ).toString();
   helpBrowser = settings.value( "helpBrowser", helpBrowser ).toString();
   outFormBrowser = settings.value( "outFormBrowser", outFormBrowser ).toString();
   secdel = settings.value( "secdel", secdel ).toString();
   settings.setValue( "xalanPath", xalanPath );
   settings.setValue( "outFormPath", outFormPath );
   settings.setValue( "helpBrowser", helpBrowser );
   settings.setValue( "outFormBrowser", outFormBrowser );
   settings.setValue( "secdel", secdel );
   settings.endGroup();
/*#ifdef Q_OS_WIN
	xalanPath.prepend(workPath);
	outFormPath.prepend(workPath);
#endif*/
   return true;
}

bool TaskConfig::dbConnect( QString progName )
{
#ifdef SPLASHFORM_H
    TSziIniFile iniFile( configPath+configName+".cfg" );
    TSziIniFile userIniFile( workPath+"user.cfg" );
    iniFile.beginGroup( "Connection" );
    userIniFile.beginGroup( "Connection" );
    SplashForm *sf = new SplashForm;
    sf->ui.L_name->setText( progName );
    sf->ui.E_user->setText( userIniFile.value( "User", "postgres" ).toString() );
    if( sf->ui.E_user->text() != "" )
        sf->ui.E_pass->setFocus();

    db = QSqlDatabase::addDatabase( "QPSQL" );
    db.setHostName( iniFile.value( "HostName", "localhost" ).toString() );
    db.setDatabaseName( iniFile.value( "Database", "" ).toString() );
    iniFile.setValue( "HostName", db.hostName() );
    iniFile.setValue( "Database", db.databaseName() );
    iniFile.endGroup();
    if( sf->exec() )
    {        
        userIniFile.setValue( "User", sf->ui.E_user->text() );

        db.setUserName( sf->ui.E_user->text() );
        db.setPassword( sf->ui.E_pass->text() );

        delete sf;
        userIniFile.endGroup();        

        if( !db.open() )
        {
            QString noConnectMessage;
            if( db.lastError().text().indexOf( "unknown host name" ) > -1 )
                noConnectMessage = QObject::tr( "Неверное имя сервера: " )+db.hostName();
            else
                if( db.lastError().text().indexOf( "could not connect" ) > -1 ||
                    db.lastError().text().indexOf( "Connection reset by peer" ) > -1 ||
                    db.lastError().text().indexOf( "No route to host" ) > -1 ||
                    db.lastError().text().indexOf( "Network is unreachable" ) > -1 )
                    noConnectMessage = QObject::tr( "Невозможно подключиться к серверу БД: " )+db.hostName();
                else
                    if( db.lastError().text().indexOf( "no password supplied" ) > -1 )
                        noConnectMessage = QObject::tr( "Незаданно имя пользователя и/или пароль" );
                    else
                        if( db.lastError().text().indexOf( "authentication failed" ) > -1 )
                            noConnectMessage = QObject::tr( "Неверное имя пользователя и/или пароль" );
                        else
                            if( db.lastError().text().indexOf( "does not exist" ) > -1 )
                                noConnectMessage = QObject::tr( "Неверное имя базы данных: " )+db.databaseName();
                            else
                                noConnectMessage = db.lastError().text();

            QMessageBox::critical( 0, QObject::tr( "Ошибка" ), noConnectMessage );
            return false;
        }
       // UsrList::init();
        //UsrList::instance();
       // ul->usrListUp();
    }
    else
    {
        iniFile.endGroup();
        delete sf;        
        return false;
    }

    return true;
#else
    return false;
#endif
}

bool TaskConfig::macConfirm( QString tableName )
{
#ifdef Q_OS_MCBC
    QSqlQuery macConfirmQuery;
    macConfirmQuery.prepare( "select relmaclabel from pg_catalog.pg_class where relname=? " );
    macConfirmQuery.bindValue( 0, tableName );
    macConfirmQuery.exec();
    if( macConfirmQuery.first() )
    {
        QString macDBLevel = macConfirmQuery.value( 0 ).toString().at( 1 );
        if( getMacLevel() >= macDBLevel.toInt() )
            return true;
        else
        {
            QMessageBox::critical( 0, QObject::tr( "Ошибка" ),
                                   QObject::tr( "Уровень сеанса не соответствует уровню БД" ) );
            return false;
        }
    }
#endif
    return true;
}

QString TaskConfig::getWorkPath()
{
    return workPath;
}

QString TaskConfig::getStaticDataPath()
{
    return staticDataPath;
}

QString TaskConfig::getConfigPath()
{
    return configPath;
}

int TaskConfig::getDefaultColumnItem( QString nameField, const DefaultColumnItem *defaultColumnItems )
{
    for( int i = 0; defaultColumnItems[i].name != NULL; i++ )
        if( defaultColumnItems[i].name == nameField )
            return i;
    return -1;
}

void TaskConfig::saveChildrenObject( TSziIniFile &childrenSettings, QObject *parent )
{    
    for( int i = 0; i < parent->children().count(); i++ )
    {
        if( QString( static_cast<QObject*> (parent->children().at( i ))->metaObject()->className() ) == "QSplitter" )
            childrenSettings.setValue( static_cast<QSplitter*> ( parent->children().at( i ) )->objectName()+".State", static_cast<QSplitter*> ( parent->children().at( i ) )->saveState() );        
        saveChildrenObject( childrenSettings, parent->children().at( i ) );
    }
}

void TaskConfig::loadChildrenObject( TSziIniFile &childrenSettings, QObject *parent )
{
    for( int i = 0; i < parent->children().count(); i++ )
    {        
        if( QString( static_cast<QObject*> (parent->children().at( i ))->metaObject()->className() ) == "QSplitter" )
            static_cast<QSplitter*> ( parent->children().at( i ) )->restoreState( childrenSettings.value( static_cast<QSplitter*> ( parent->children().at( i ) )->objectName()+".State" ).toByteArray() );        
        loadChildrenObject( childrenSettings, parent->children().at( i ) );
    }
}

void TaskConfig::formSave( QWidget *form, QString moduleName, bool mainWindow )
{
    TSziIniFile formSettings( workPath+moduleName+".cfg" );
    formSettings.beginGroup( form->objectName() );    
    //qDebug() << workPath+moduleName+".cfg" << form->objectName();
    if( mainWindow )
        formSettings.setValue( "windowState", static_cast<QMainWindow*> ( form )->saveState() );    
    formSettings.setValue( "windowGeometry", form->saveGeometry() );
    saveChildrenObject( formSettings, form );
    formSettings.endGroup();
}

void TaskConfig::formLoad( QWidget *form, QString moduleName, bool mainWindow )
{
    TSziIniFile formSettings( workPath+moduleName+".cfg" );
    formSettings.beginGroup( form->objectName() );    
    //qDebug() << workPath+moduleName+".cfg" << form->objectName();
    if( mainWindow )
        static_cast<QMainWindow*> ( form )->restoreState( formSettings.value( "windowState" ).toByteArray() );
    form->restoreGeometry( formSettings.value( "windowGeometry" ).toByteArray() );
    loadChildrenObject( formSettings, form );
    formSettings.endGroup();    
}

void TaskConfig::tableSave( QTableView *tableView, QString moduleName, QSqlQueryModel *qModel )
{
    TSziIniFile tableSettings( workPath+moduleName+".cfg" );
    tableSettings.beginGroup( tableView->objectName()+'-'+tableView->model()->objectName() );

    QSqlQueryModel *querymodel;
    if( qModel )
        querymodel = qModel;
    else
        querymodel = static_cast<QSqlQueryModel*>( tableView->model() );

    for( int i = 0; i<tableView->model()->columnCount(); i++ )
    {
            tableSettings.setValue( querymodel->record().fieldName(i)+".TitleCaption",
                                    querymodel->headerData( i, Qt::Horizontal ).toString() );

            if( tableView->horizontalHeader()->resizeMode( i ) == QHeaderView::Stretch )
                tableSettings.setValue( querymodel->record().fieldName( i )+".Stretch", true );
            else
            {
                tableSettings.setValue( querymodel->record().fieldName( i )+".Stretch", false );
                if( !tableView->isColumnHidden( i ) )
                {
                    tableSettings.setValue( querymodel->record().fieldName(i)+".Width",
                                            tableView->columnWidth(i) );
                }
            }

            tableSettings.setValue( querymodel->record().fieldName( i )+".Visible", !tableView->isColumnHidden( i ) );

            tableSettings.setValue( querymodel->record().fieldName( i )+".Index",
                                    tableView->horizontalHeader()->visualIndex( i ) );        
    }
    tableSettings.endGroup();
}

void TaskConfig::tableLoad( QTableView *tableView, const DefaultColumnItem *defaultColumnItems, QString moduleName, QSqlQueryModel *qModel )
{
    connect( tableView->horizontalHeader(), SIGNAL( sectionResized( int, int, int ) ),
             this, SLOT( tableView_sectionResized( int, int, int ) ) );

    TSziIniFile tableSettings( workPath+moduleName+".cfg" );
    tableSettings.beginGroup( tableView->objectName()+'-'+tableView->model()->objectName() );

    QSqlQueryModel *querymodel;
    if( qModel )
        querymodel = qModel;
    else
        querymodel = static_cast<QSqlQueryModel*>( tableView->model() );

    for( int i = 0; i<querymodel->columnCount(); i++ )
    {
        int indexDefaultColumnItem = getDefaultColumnItem( querymodel->record().fieldName( i ), defaultColumnItems );
        if( indexDefaultColumnItem > -1 )
        {
            if( tableSettings.value( querymodel->record().fieldName( i )+".Stretch", defaultColumnItems[indexDefaultColumnItem].stretch ).toBool() )
                tableView->horizontalHeader()->setResizeMode( i, QHeaderView::Stretch );
            else
                if( defaultColumnItems[indexDefaultColumnItem].width )
                    tableView->setColumnWidth( i, tableSettings.value( querymodel->record().fieldName( i )+".Width", defaultColumnItems[indexDefaultColumnItem].width ).toInt() );
                else
                    tableView->setColumnWidth( i, tableSettings.value( querymodel->record().fieldName( i )+".Width", tableView->columnWidth( i ) ).toInt() );

            querymodel->setHeaderData( i, Qt::Horizontal,
                                       wordWrapOnResized( tableSettings.value( querymodel->record().fieldName( i )+".TitleCaption",
                                                                               defaultColumnItems[indexDefaultColumnItem].title ).toString(),
                                                          tableView->columnWidth( i ) ) );

            tableView->setColumnHidden( i, !tableSettings.value( querymodel->record().fieldName( i )+".Visible", defaultColumnItems[indexDefaultColumnItem].visible ).toBool() );

            if( tableSettings.value( querymodel->record().fieldName( i )+".Index", i ).toInt() )
                tableView->horizontalHeader()->moveSection( tableView->horizontalHeader()->visualIndex( i ),
                                                            tableSettings.value( querymodel->record().fieldName( i )+".Index", i ).toInt() );
        }
        else
            tableView->hideColumn(i);        
    }

    tableSettings.endGroup();    
}

void TaskConfig::hHeaderTableSave( QTableView *tableView, QStandardItemModel *headerModel, QString moduleName )
{
    TSziIniFile tableSettings( workPath+moduleName+".cfg" );
    tableSettings.beginGroup( tableView->objectName() );

    QSqlQueryModel *querymodel = static_cast<QSqlQueryModel*>( tableView->model() );
    for( int i = 0; i<tableView->model()->columnCount(); i++ )
    {
        if( !tableView->isColumnHidden( i ) )
        {

            //QMessageBox::information( 0, "", headerModel->data( headerModel->index( 0, i ) ).toString() );
            tableSettings.setValue( querymodel->record().fieldName(i)+".TitleCaption",
                                    headerModel->data( headerModel->index( 0, i ) ).toString() );

            /*tableSettings.setValue( querymodel->record().fieldName(i)+".TitleCaption",
                                                                        querymodel->headerData( i, Qt::Horizontal ).toString() );*/

            if( tableView->horizontalHeader()->resizeMode( i ) == QHeaderView::Stretch )
                tableSettings.setValue( querymodel->record().fieldName( i )+".Stretch", true );
            else
            {
                tableSettings.setValue( querymodel->record().fieldName( i )+".Stretch", false );
                tableSettings.setValue( querymodel->record().fieldName(i)+".Width",
                                        tableView->columnWidth(i) );
            }

            tableSettings.setValue( querymodel->record().fieldName( i )+".Visible", !tableView->isColumnHidden( i ) );

            tableSettings.setValue( querymodel->record().fieldName( i )+".Index",
                                    tableView->horizontalHeader()->visualIndex( i ) );
        }
    }
    tableSettings.endGroup();
}

void TaskConfig::hHeaderTableLoad( QTableView *tableView, QStandardItemModel *headerModel,
                                   const DefaultColumnItem *defaultColumnItems, QString moduleName )
{
    TSziIniFile tableSettings( workPath+moduleName+".cfg" );
    tableSettings.beginGroup( tableView->objectName() );

    QSqlQueryModel *querymodel = static_cast<QSqlQueryModel*>( tableView->model() );
    for( int i = 0; i<querymodel->columnCount(); i++ )
    {
        int indexDefaultColumnItem = getDefaultColumnItem( querymodel->record().fieldName( i ), defaultColumnItems );
        if( indexDefaultColumnItem > -1 )
        {
            headerModel->setItem(0, i, new QStandardItem( tableSettings.value( querymodel->record().fieldName( i )+".TitleCaption",
                                                                               defaultColumnItems[indexDefaultColumnItem].title ).toString() ) );

            /*querymodel->setHeaderData( i, Qt::Horizontal,
                                       tableSettings.value( querymodel->record().fieldName( i )+".TitleCaption",
                                                            defaultColumnItems[indexDefaultColumnItem].title ).toString() );*/
            //headerModel->set

            if( tableSettings.value( querymodel->record().fieldName( i )+".Stretch", defaultColumnItems[indexDefaultColumnItem].stretch ).toBool() )
                tableView->horizontalHeader()->setResizeMode( i, QHeaderView::Stretch );
            else
                if( defaultColumnItems[indexDefaultColumnItem].width )
                    tableView->setColumnWidth( i, tableSettings.value( querymodel->record().fieldName( i )+".Width", defaultColumnItems[indexDefaultColumnItem].width ).toInt() );
                else
                    tableView->setColumnWidth( i, tableSettings.value( querymodel->record().fieldName( i )+".Width", tableView->columnWidth( i ) ).toInt() );

            tableView->setColumnHidden( i, !tableSettings.value( querymodel->record().fieldName( i )+".Visible", defaultColumnItems[indexDefaultColumnItem].visible ).toBool() );

            if( tableSettings.value( querymodel->record().fieldName( i )+".Index", i ).toInt() )
                tableView->horizontalHeader()->moveSection( tableView->horizontalHeader()->visualIndex( i ),
                                                            tableSettings.value( querymodel->record().fieldName( i )+".Index", i ).toInt() );
        }
        else
            tableView->hideColumn(i);
    }

    tableSettings.endGroup();
}

void TaskConfig::treeSave( QTreeView *treeView, QString moduleName )
{
    TSziIniFile treeSettings( workPath+moduleName+".cfg" );    
    treeSettings.beginGroup( treeView->objectName() );
    for( int i = 0; i < treeView->model()->columnCount(); i++ )
    {
        treeSettings.setValue( "Column"+QString::number( i )+".TitleCaption",
                               treeView->model()->headerData( i, Qt::Horizontal ).toString() );

        if( treeView->header()->resizeMode( i ) == QHeaderView::Stretch )
            treeSettings.setValue( "Column"+QString::number( i )+".Stretch", true );
        else
        {
            treeSettings.setValue( "Column"+QString::number( i )+".Stretch", false );
            treeSettings.setValue( "Column"+QString::number( i )+".Width",
                                   treeView->columnWidth( i ) );
        }

        treeSettings.setValue( "Column"+QString::number( i )+".Visible", !treeView->isColumnHidden( i ) );

    }
    treeSettings.endGroup();
}

void TaskConfig::treeLoad( QTreeView *treeView, const DefaultColumnItem *defaultColumnItems, QString moduleName )
{
    connect( treeView->header(), SIGNAL( sectionResized( int, int, int ) ),
             this, SLOT( treeView_sectionResized( int, int, int ) ) );

    TSziIniFile treeSettings( workPath+moduleName+".cfg" );
    treeSettings.beginGroup( treeView->objectName() );

    for( int i = 0; i < treeView->model()->columnCount(); i++ )
    {
        int indexDefaultColumnItem = getDefaultColumnItem( treeView->model()->headerData( i, Qt::Horizontal ).toString(), defaultColumnItems );
        if( indexDefaultColumnItem > -1 )
        {            
            treeView->model()->setHeaderData( i, Qt::Horizontal,
                                              treeSettings.value( "Column"+QString::number( i )+".TitleCaption",
                                                                  defaultColumnItems[indexDefaultColumnItem].title ) );

            if( treeSettings.value( "Column"+QString::number( i )+".Stretch", defaultColumnItems[indexDefaultColumnItem].stretch ).toBool() )
                treeView->header()->setResizeMode( i, QHeaderView::Stretch );
            else
                if( defaultColumnItems[indexDefaultColumnItem].width )
                    treeView->setColumnWidth( i, treeSettings.value( "Column"+QString::number( i )+".Width", defaultColumnItems[indexDefaultColumnItem].width ).toInt() );
                else
                    treeView->setColumnWidth( i, treeSettings.value( "Column"+QString::number( i )+".Width", treeView->columnWidth( i ) ).toInt() );

            treeView->setColumnHidden( i, !treeSettings.value( "Column"+QString::number( i )+".Visible", defaultColumnItems[indexDefaultColumnItem].visible ).toBool() );

        }
        else
            treeView->hideColumn( i );
    }    

    treeSettings.endGroup();

    //treeView->header()->setMovable( false );
}

void TaskConfig::hHeaderTreeSave( QTreeView *treeView, QStandardItemModel *headerModel, QString moduleName )
{
    TSziIniFile treeSettings( workPath+moduleName+".cfg" );
    treeSettings.beginGroup( treeView->objectName() );
    for( int i = 0; i < treeView->model()->columnCount(); i++ )
    {
        treeSettings.setValue( "Column"+QString::number( i )+".TitleCaption",
                               headerModel->data( headerModel->index( 0, i ) ).toString() );

        if( treeView->header()->resizeMode( i ) == QHeaderView::Stretch )
            treeSettings.setValue( "Column"+QString::number( i )+".Stretch", true );
        else
        {
            treeSettings.setValue( "Column"+QString::number( i )+".Stretch", false );
            treeSettings.setValue( "Column"+QString::number( i )+".Width",
                                   treeView->columnWidth( i ) );
        }

        treeSettings.setValue( "Column"+QString::number( i )+".Visible", !treeView->isColumnHidden( i ) );

    }
    treeSettings.endGroup();
}

void TaskConfig::hHeaderTreeLoad( QTreeView *treeView, QStandardItemModel *headerModel,
                                  const DefaultColumnItem *defaultColumnItems, QString moduleName )
{
    TSziIniFile treeSettings( workPath+moduleName+".cfg" );
    treeSettings.beginGroup( treeView->objectName() );

    for( int i = 0; i < treeView->model()->columnCount(); i++ )
    {
        int indexDefaultColumnItem = getDefaultColumnItem( treeView->model()->headerData( i, Qt::Horizontal ).toString(), defaultColumnItems );
        if( indexDefaultColumnItem > -1 )
        {
            headerModel->setItem( 0, i, new QStandardItem( treeSettings.value( "Column"+QString::number( i )+".TitleCaption",
                                                                               defaultColumnItems[indexDefaultColumnItem].title ).toString() ) );
            /*treeView->model()->setHeaderData( i, Qt::Horizontal,
                                              treeSettings.value( "Column"+QString::number( i )+".TitleCaption",
                                                                  defaultColumnItems[indexDefaultColumnItem].title ) );*/

            if( treeSettings.value( "Column"+QString::number( i )+".Stretch", defaultColumnItems[indexDefaultColumnItem].stretch ).toBool() )
                treeView->header()->setResizeMode( i, QHeaderView::Stretch );
            else
                if( defaultColumnItems[indexDefaultColumnItem].width )
                    treeView->setColumnWidth( i, treeSettings.value( "Column"+QString::number( i )+".Width", defaultColumnItems[indexDefaultColumnItem].width ).toInt() );
                else
                    treeView->setColumnWidth( i, treeSettings.value( "Column"+QString::number( i )+".Width", treeView->columnWidth( i ) ).toInt() );

            treeView->setColumnHidden( i, !treeSettings.value( "Column"+QString::number( i )+".Visible", defaultColumnItems[indexDefaultColumnItem].visible ).toBool() );

        }
        else
            treeView->hideColumn( i );
    }

    treeSettings.endGroup();
}

void TaskConfig::executeBrowser( QString fileName, bool help )
{
    QString browser;    
    if( help )
    {
        browser = helpBrowser;
        fileName = "file://"+staticDataPath+"help/"+fileName;

    }
    else
    {
        browser = outFormBrowser;
//		fileName = "file://"+workPath+"report/"+fileName;
    }

    /*if( QFile::exists( browser ) )
    {*/
        /*if( QFile::exists( fileName ) )
        {*/
            if( !summon_help( browser, fileName ) )
                QMessageBox::critical( 0, "Ошибка", "Невозможно открыть программу просмотра" );
        /*}
        else
            QMessageBox::critical( 0, "Ошибка", "Файл: "+fileName+" отсутствует" );*/
    /*}
    else
        QMessageBox::critical( 0, "Ошибка", "Отсутствует программа просмотра: "+browser );*/
}

void TaskConfig::executeXalan(  QString htmlFileName, QString xmlFileName, QString xsltFileName )
{
    QProcess process;
    QStringList args = ( QStringList() << "-o" << htmlFileName << xmlFileName << outFormPath+xsltFileName );
    //QMessageBox::information( 0, "", htmlFileName +" "+ xmlFileName +" "+ outFormPath+xsltFileName );
    //qDebug() << xalanPath+"Xalan" << args;
    process.execute( xalanPath+"Xalan", args);
    executeBrowser( htmlFileName, false );
}

QString TaskConfig::sortTable(QTableView *table, const DefaultColumnItem *defaultColumnItems) {
	QModelIndex index = table->currentIndex();
	if (!index.isValid()) return QObject::tr("Порядок по: N");
	QSqlQueryModel *model = static_cast<QSqlQueryModel*>(table->model());
	QString field = model->record(index.row()).fieldName(index.column());
	QString query = model->query().lastQuery().toLower();
	QString queryRight = query.right(query.count() - query.lastIndexOf("order by"));
	QString queryLeft = query.left(query.lastIndexOf("order by"));
	QStringList list = queryRight.split(" ", QString::SkipEmptyParts);
	if (list.at(2) == field) {
		if (list.count() > 3) {
			if (list.at(3) != "desc") list.insert(3, "desc");
			else list.removeAt(3);
		} else list.append("desc");
	} else {
		list.replace(2, field);
		if (list.count() > 3) {
			if (list.at(3) == "desc") list.removeAt(3);
		}
	}
	QString desc = list.join(" ").indexOf(" desc") != -1 ? QObject::tr(" обр.") : QObject::tr("");
        tableSave(table, "grids");
	model->setQuery(queryLeft+list.join(" "));
        tableLoad(table, defaultColumnItems, "grids");
	table->setCurrentIndex(index);
	return QObject::tr("Порядок по: ")+model->headerData(index.column(), Qt::Horizontal).toString().toUtf8().replace("-\n", "").replace("\n", " ")+desc;
}

/*void TaskConfig::showFinder(QTableView *table) {
	Find *form = new Find(table);
	form->show();
}*/

void TaskConfig::showSettingsTable(QTableView *table) {
	SettingsTable::Run(table);
}

bool TaskConfig::fileSecDel( QString filename )
{
    if( !filename.isEmpty() )
    {
        if( getMacLevel() > 0 )
        {
            QString secDelBin = secdel.section( " ", 1 );
            if( QFile::exists( filename ) )
                if( QFile::exists( secDelBin ) )
                    QProcess::execute( secdel+" \""+filename+"\"" );
                else
                {
                    QMessageBox::critical( 0, "Ошибка", "Отсутствует средство маскирующего удаления\n"
                                           "Для успешного выполнения укажите другое имя файла" );
                    return false;
                }
        }
    }
    return true;
}

void TaskConfig::setMainHelpFile( QString mHF )
{
    mainHelpFile = mHF;
}

void TaskConfig::showMainHelp()
{
    if( !mainHelpFile.isEmpty() )
        executeBrowser( mainHelpFile );
}

void TaskConfig::delRec(QString table, QString column, QTableView *tableView, bool confirm) {
	QModelIndex index = tableView->currentIndex();
	if (!index.isValid()) return;
	QSqlQueryModel *model = static_cast<QSqlQueryModel*>(tableView->model());
	bool del = true;
	if (confirm)
		if (!this->confirm(QObject::tr("Вы действительно хотите удалить выделенную запись?"), QObject::tr("Удаление"))) del = false;
	if (del) {
		QSqlQuery sqlTemp;
		QSqlRecord record = model->record(index.row());
		sqlTemp.exec("delete from "+table+" where "+column+"="+record.value(column).toString());
		model->setQuery(model->query().lastQuery());
	}
	if (index.row() == model->rowCount()) {
		tableView->setCurrentIndex(model->index(index.row() - 1, index.column()));
	} else {
		tableView->setCurrentIndex(index);
	}
}

void TaskConfig::delRec(QString table, QString column, QString value)
{
	QSqlQuery query("delete from "+table+" where "+column+"="+value);
}

void TaskConfig::showAbout(QString text, QString version)
{
    About about;
    about.setMessage(text,version);
    about.exec();
}

UsrList* TaskConfig::showUsr(QWidget *parent, bool showed)
{
    UsrList *usr1=UsrList::instance();
    usr1->setParent(parent);
    if(showed)
        usr1->show();
    return usr1;
}

bool TaskConfig::confirm(QString question, QString title) {
	bool confirm;
	QMessageBox msg(QMessageBox::Question, title, question, QMessageBox::NoButton, 0);
	QAbstractButton *yes_button = msg.addButton(QObject::tr("Да"), QMessageBox::AcceptRole);
	QAbstractButton *cancel_button = msg.addButton(QObject::tr("Нет"), QMessageBox::RejectRole);
	if (msg.exec()==QMessageBox::RejectRole) confirm = false;
	else confirm = true;
	delete yes_button;
	delete cancel_button;
	return confirm;
}

void TaskConfig::waitStart(QWidget *parent, bool needRepaint, QString text) {
    WaitWidget::start(parent, needRepaint, text);
}

void TaskConfig::waitSetText(QString text) {
    WaitWidget::setText(text);
}

void TaskConfig::waitFinish() {
    WaitWidget::finish();
}

QString TaskConfig::wordWrapOnResized( QString headerLabel, int newRectSize )
{
    QFont font;
    QFontMetrics fm( font );
    headerLabel.replace( "\n", " " );
    QRect hlRect=fm.boundingRect( headerLabel );
    if( hlRect.width() > newRectSize-fm.boundingRect( "шш" ).width() )
    {
        int lastSpace = 0;
        while( lastSpace > -1 )
        {
            int findLastSpace = headerLabel.lastIndexOf( " ", ( int ) ceil( (newRectSize-fm.boundingRect( "шш" ).width())/ceil( hlRect.width()/headerLabel.length() ) )+lastSpace );
            headerLabel.replace( findLastSpace, 1, "\n" );

            if( findLastSpace < 0 && (( int ) ceil( (newRectSize-fm.boundingRect( "шш" ).width())/ceil( hlRect.width()/headerLabel.length() ) )+lastSpace) < headerLabel.length() )
            {
                findLastSpace = headerLabel.indexOf( " ", lastSpace );
                headerLabel.replace( findLastSpace, 1, "\n" );
            }

            lastSpace = findLastSpace;
        }
    }

    return headerLabel;
}

void TaskConfig::tableView_sectionResized( int logicalIndex, int, int newSize )
{    
    QString headerLabel = static_cast<QSqlQueryModel*>( static_cast<QTableView*>( QObject::sender()->parent() )->model() )->headerData( logicalIndex, Qt::Horizontal ).toString();    
    headerLabel = wordWrapOnResized( headerLabel, newSize );

    static_cast<QSqlQueryModel*>( static_cast<QTableView*>( QObject::sender()->parent() )->model() )->setHeaderData( logicalIndex, Qt::Horizontal, headerLabel );
    //static_cast<QTableView*>( QObject::sender()->parent() )->resizeColumnsToContents();
    /*static_cast<QTableView*>( QObject::sender()->parent() )->resize( static_cast<QTableView*>( QObject::sender()->parent() )->size().width()+1,
                                                                     static_cast<QTableView*>( QObject::sender()->parent() )->size().height()+1 );
    static_cast<QTableView*>( QObject::sender()->parent() )->resize( static_cast<QTableView*>( QObject::sender()->parent() )->size().width()-1,
                                                                     static_cast<QTableView*>( QObject::sender()->parent() )->size().height()-1 );*/
}

void TaskConfig::treeView_sectionResized( int logicalIndex, int, int newSize )
{
    QString headerLabel = static_cast<QTreeView*>( QObject::sender() )->model()->headerData( logicalIndex, Qt::Horizontal ).toString();
    headerLabel = wordWrapOnResized( headerLabel, newSize );

    static_cast<QTreeView*>( QObject::sender() )->model()->setHeaderData( logicalIndex, Qt::Horizontal, headerLabel );
    //static_cast<QTableView*>( QObject::sender()->parent() )->resizeColumnsToContents();
    /*static_cast<QTableView*>( QObject::sender()->parent() )->resize( static_cast<QTableView*>( QObject::sender()->parent() )->size().width()+1,
                                                                     static_cast<QTableView*>( QObject::sender()->parent() )->size().height()+1 );
    static_cast<QTableView*>( QObject::sender()->parent() )->resize( static_cast<QTableView*>( QObject::sender()->parent() )->size().width()-1,
                                                                     static_cast<QTableView*>( QObject::sender()->parent() )->size().height()-1 );*/
}
