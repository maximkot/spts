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
#include <QObject>
#include <QSqlRecord>
#include <QDate>

#include "ui_mainwindow.h"

#include "taskconfig.h"
#include "tsziinifile.h"
#include "mainwindow.h"
#include "p_sgapred.h"
#include "p_gvspred.h"
#include "p_gvsspusk.h"
#include "p_npapred.h"
#include "p_sgaspusk.h"
#include "p_npaspusk.h"
#include "edittemplate.h"
#include "setsound.h"
#include "setcolor.h"
#include "moduleconst.h"
#include "modulevar.h"
#include "newts.h"
#include "find.h"


//-------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Запускаем наш поток, в котором играем тревожный звук
    thread.start();

    setWindowTitle(ZadName.toLatin1());

     // Вводим комбинации горячих клавиш для данной формы
    // Затем связываем эти шоткаты сигналами со слотами
                                                         /*
    Новая запись          Ctrl+N
    Редактировать запись  Ctrl+E
    Удалить запись        Ctrl+X
    Открыть НСИ           Ctrl+D
    Открыть настройки     Ctrl+S
    Открыть строку поиска Ctrl+F
    Вызвать помощь        Ctrl+H (а также F1, к-рый отлавливается через KeyEvent)
    Закрыть форму         Ctrl+Q   */

    QShortcut *exitShortcut = new QShortcut(QKeySequence("Ctrl+Q"),this,0);
    QShortcut *addShortcut  = new QShortcut(QKeySequence("Ctrl+N"),this,0);
    QShortcut *editShortcut = new QShortcut(QKeySequence("Ctrl+E"),this,0);
    QShortcut *delShortcut  = new QShortcut(QKeySequence("Ctrl+X"),this,0);
    QShortcut *sprShortcut  = new QShortcut(QKeySequence("Ctrl+D"),this,0);
    QShortcut *findShortcut = new QShortcut(QKeySequence("Ctrl+F"),this,0);
    QShortcut *setClrShortcut  = new QShortcut(QKeySequence("Ctrl+C"),this,0);
    QShortcut *setSndShortcut  = new QShortcut(QKeySequence("Ctrl+S"),this,0);
    QShortcut *hlpShortcut  = new QShortcut(QKeySequence("Ctrl+H"),this,0);

     // Соединяем сигналы и слоты:
    // Нажатие шоткатов
    connect (exitShortcut,  SIGNAL (activated()), this, SLOT (CloseWindowMain()));
    connect (addShortcut,   SIGNAL (activated()), this, SLOT (FormChoiceAdd()));
    connect (editShortcut,  SIGNAL (activated()), this, SLOT (FormChoiceEdit()));
    connect (delShortcut,   SIGNAL (activated()), this, SLOT (DeleteRecord()));
    connect (sprShortcut,   SIGNAL (activated()), this, SLOT (CreateEditTemplate()));
    connect (findShortcut,  SIGNAL (activated()), this, SLOT (ShortCutFindAction()));
    connect (setClrShortcut,SIGNAL (activated()), this, SLOT (CreateClrWindow()));
    connect (setSndShortcut,SIGNAL (activated()), this, SLOT (CreateSndWindow()));
    connect (hlpShortcut,   SIGNAL (activated()), this, SLOT (HelpAction()));

    // Выбор текущего типа ТС
    connect (ui->menuSelectSGA, SIGNAL (triggered()), this, SLOT (SelectSGA())); // Меню "Техническое средство\СГА" - Выбрать режим работы с СГА
    connect (ui->menuSelectGVS, SIGNAL (triggered()), this, SLOT (SelectGVS())); // Меню "Техническое средство\ЖВС" - Выбрать режим работы с ЖВС
    connect (ui->menuSelectNPA, SIGNAL (triggered()), this, SLOT (SelectNPA())); // Меню "Техническое средство\НПА" - Выбрать режим работы с НПА
    connect (ui->menuCreateSetColor, SIGNAL (triggered()), this, SLOT (CreateClrWindow()));    // Меню "Настройки\Цветовые настройки" - Открыть форму настроек цветов
    connect (ui->menuCreateSetSound, SIGNAL (triggered()), this, SLOT (CreateSndWindow()));   // Меню "Настройки\Цветовые настройки" - Открыть форму настроек цветов
    connect (ui->menuCloseWindowMain, SIGNAL (triggered()), this, SLOT (CloseWindowMain()));     // Меню 'Выход'
    connect (ui->menuSpravMain, SIGNAL (triggered()), this, SLOT (HelpAction()));           // Меню Открыть форму "О Программе"
    connect (ui->menuSpravContent, SIGNAL (triggered()), this, SLOT (HelpAction()));       // Меню Открыть форму "О Программе"
    connect (ui->menuCreateWinAbout, SIGNAL (triggered()), this, SLOT (CreateWinAbout()));// Меню Открыть форму "О Программе"

    connect (ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT (ChangeCurrentPage(int)));     // смена страницы ТабВыджета

//    connect (ui->tblSostTS, SIGNAL(clicked(QModelIndex)),this, SLOT (ChangeCurrentPage(int)));


    connect (ui->menuNSISGA_5,  SIGNAL (triggered ()), this, SLOT (CreateEditTemplate5()));   // Вызов окна "Редактирование НСИ" из соответствующего пункта меню
    connect (ui->menuNSISGA_6,  SIGNAL (triggered ()), this, SLOT (CreateEditTemplate6()));   // Вызов окна "Редактирование НСИ" из соответствующего пункта меню
    connect (ui->menuNSISGA_7,  SIGNAL (triggered ()), this, SLOT (CreateEditTemplate7()));   // Вызов окна "Редактирование НСИ" из соответствующего пункта меню
    connect (ui->menuNSIGVS_8,  SIGNAL (triggered ()), this, SLOT (CreateEditTemplate8()));   // Вызов окна "Редактирование НСИ" из соответствующего пункта меню
    connect (ui->menuNSIGVS_9,  SIGNAL (triggered ()), this, SLOT (CreateEditTemplate9()));   // Вызов окна "Редактирование НСИ" из соответствующего пункта меню
    connect (ui->menuNSIGVS_10, SIGNAL (triggered ()), this, SLOT (CreateEditTemplate10()));  // Вызов окна "Редактирование НСИ" из соответствующего пункта меню
    connect (ui->menuNSIGVS_11, SIGNAL (triggered ()), this, SLOT (CreateEditTemplate11()));  // Вызов окна "Редактирование НСИ" из соответствующего пункта меню
    connect (ui->menuNSIGVS_12, SIGNAL (triggered ()), this, SLOT (CreateEditTemplate12()));  // Вызов окна "Редактирование НСИ" из соответствующего пункта меню
    connect (ui->menuNSIGVS_13, SIGNAL (triggered ()), this, SLOT (CreateEditTemplate13()));  // Вызов окна "Редактирование НСИ" из соответствующего пункта меню
    connect (ui->menuNSINPA_14, SIGNAL (triggered ()), this, SLOT (CreateEditTemplate14()));  // Вызов окна "Редактирование НСИ" из соответствующего пункта меню
    connect (ui->menuNSINPA_15, SIGNAL (triggered ()), this, SLOT (CreateEditTemplate15()));  // Вызов окна "Редактирование НСИ" из соответствующего пункта меню


     // Рисуем в тулбаре кнопки сиконками---
    // Добавить
    QIcon newIcon;
    newIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/new.png"), QSize(), QIcon::Normal, QIcon::Off);
    actNewRecord = new QAction (newIcon, tr("Новая запись\nCtrl+N"), this);
    // Изменить
    QIcon editIcon;
    editIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
    actEditRecord = new QAction (editIcon, tr("Редактировать запись\nCtrl+E"), this);
    // Удалить
    QIcon delIcon;
    delIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
    actDelRecord = new QAction (delIcon, tr("Удалить запись\nCtrl+X"), this);
    // Поиск
    QIcon find;
    find.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/find.png"), QSize(), QIcon::Normal, QIcon::Off);
    actFindAction = new QAction (find, tr("Поиск\nCtrl+F"), this);
    //Ставим кнопу зажимаемой
    actFindAction->setCheckable(true);
    // НСИ
    QIcon nsiIcon;
    nsiIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/directory.png"), QSize(), QIcon::Normal, QIcon::Off);
    actNSIAction = new QAction (nsiIcon, tr("Редактирование НСИ\nCtrl+D"), this);
    // Кнопка 'Настройки цвета'
    QIcon setColorIcon;
    setColorIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/color.png"), QSize(), QIcon::Normal, QIcon::Off);
    actSetColorAction = new QAction (setColorIcon, tr("Настройки цвета\nCtrl+C"), this);
    // Кнопка 'Настройки звука'
    QIcon setSoundIcon;
    setSoundIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/myicons/mysound.ico"), QSize(), QIcon::Normal, QIcon::Off);
    actSetSoundAction = new QAction (setSoundIcon, tr("Настройки звука\nCtrl+S"), this);
    // Кнопка 'Справка'
    QIcon helpIcon;
    helpIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/helpindx.png"), QSize(), QIcon::Normal, QIcon::Off);
    actHelpAction = new QAction (helpIcon, tr("Система помощи\nCtrl+H"), this);
    // Выход
    QIcon exitIcon;
    exitIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
    actExitAction = new QAction (exitIcon, tr("Завершить работу\nCtrl+Q"), this);

    // привязываем к тулбару экшонсы
    ui->toolBar->addAction(actNewRecord);
    ui->toolBar->addAction(actEditRecord);
    ui->toolBar->addAction(actDelRecord);
    // разделитель после кнопок /ДобавитьИзменитьУдалить/
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actFindAction);
    // Вставляем сепаратор, ещё
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actNSIAction);
    // Вставляем сепаратор, ещё
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actSetColorAction);
    ui->toolBar->addAction(actSetSoundAction);
    // Вставляем сепаратор, ещё
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actHelpAction);
    // И ещё один перед выходом
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actExitAction);

    // Соединим экшонсы сигналами со слотами
    connect (actNewRecord,     SIGNAL (triggered ()), this, SLOT (FormChoiceAdd()));          // Экшонс на кнопку/меню "Добавить"
    connect (actEditRecord,    SIGNAL (triggered ()), this, SLOT (FormChoiceEdit()));        // Экшонс на кнопку/меню "Редактировать"
    connect (actDelRecord,     SIGNAL (triggered ()), this, SLOT (DeleteRecord()));         // Экшонс на кнопку/меню "Удалить"
    connect (actNSIAction,     SIGNAL (triggered ()), this, SLOT (CreateEditTemplate()));  // Экшонс на кнопку/меню "Редактирование НСИ"
    connect (actSetSoundAction,SIGNAL (triggered ()), this, SLOT (CreateSndWindow()));    // Экшонс на кнопку/меню "Настройки звука"
    connect (actSetColorAction,SIGNAL (triggered ()), this, SLOT (CreateClrWindow()));   // Экшонс на кнопку/меню "Настройки цвета"
    connect (actHelpAction,    SIGNAL (triggered ()), this, SLOT (HelpAction()));       // Экшонс на кнопку/меню "Система помощи"
    connect (actFindAction,    SIGNAL (triggered ()), this, SLOT (FindAction()));      // Экшонс на кнопку/меню "Система помощи"
    connect (actExitAction,    SIGNAL (triggered ()), this, SLOT (CloseWindowMain()));// Экшонс на кнопку/меню "Закрыть окно"



    // Грузим установки формы из конфигурационного файла
    qTC->formLoad(this, FormSetFile, true);

    // Отредактируем необходимые наименования пунктов меню
    QString MenuCaption;
    // Подменю "Техническое средство"

    MenuCaption = tr("Ведение информации о состоянии ").append(getTSSGA());
    ui->menuSelectSGA->setText(MenuCaption);
    MenuCaption = tr("Ведение информации о состоянии ").append(getTSGVS());
    ui->menuSelectGVS->setText(MenuCaption);
    MenuCaption = tr("Ведение информации о состоянии ").append(getTSNPA());
    ui->menuSelectNPA->setText(MenuCaption);

    //Подменю "НСИ по учету состояний ТС"
    MenuCaption = tr("НСИ по учету состояний ").append(getTSSGA());
    ui->menuNSISGA->setTitle(MenuCaption);
    MenuCaption = tr("НСИ по учету состояний ").append(getTSGVS());
    ui->menuNSIGVS->setTitle(MenuCaption);
    MenuCaption = tr("НСИ по учету состояний ").append(getTSNPA());
    ui->menuNSINPA->setTitle(MenuCaption);

    //Подменю "НСИ по учету состояний tsSGA"
    ui->menuNSISGA_5->setText(blankPredSGA.toLatin1());
    ui->menuNSISGA_6->setText(blankRabSGA.toLatin1());
    ui->menuNSISGA_7->setText(blankPosleSGA.toLatin1());

    //Подменю "НСИ по учету состояний tsGVS"
    ui->menuNSIGVS_8->setText(blankPredGVS.toLatin1());
    ui->menuNSIGVS_9->setText(blankPredBDD.toLatin1());
    ui->menuNSIGVS_10->setText(blankPosleGVS.toLatin1());
    ui->menuNSIGVS_11->setText(blankPosleBDD.toLatin1());
    ui->menuNSIGVS_12->setText(blankParamSGO.toLatin1());
    ui->menuNSIGVS_13->setText(blankSpuskGVS.toLatin1());

    //Подменю "НСИ по учету состояний tsNPA"
    ui->menuNSINPA_14->setText(blankPredNPA.toLatin1());
    ui->menuNSINPA_15->setText(blankPosleNPA.toLatin1());



    // Создаём Запросы, Модели, ПрохиМодели, связываем их
    ui->tblSostTS->setSortingEnabled(true);
    ui->tblSpisPogruzh->setSortingEnabled(true);


    QuerySostTS = new QSqlQuery (getDbName());
    QuerySpisPogruzh = new QSqlQuery (getDbName());

    ModelSostTS = new QSqlQueryModel(this);
    ModelSpisPogruzh = new QSqlQueryModel(this);

    ProxyModelSpisPlaceTS = new QSortFilterProxyModel(this);
    ProxyModelSpisPogruzh = new QSortFilterProxyModel(this);

    ProxyModelSpisPlaceTS->setSourceModel(ModelSostTS);
    ProxyModelSpisPogruzh->setSourceModel(ModelSpisPogruzh);

    ui->tblSostTS->setModel(ModelSostTS);
    ui->tblSpisPogruzh->setModel(ProxyModelSpisPogruzh);

    SelectSGA();



    // в этом случае, чтобы увидеть результаты, необходимо присвоить модели запрос ПОСЛЕ выполнения запроса
    ModelSpisPogruzh->setQuery(*QuerySpisPogruzh);


    // грузим столбцы
    //if (!qTC->tableLoad(ui->tblSpisPogruzh, ModelSpisPogruzh))
    // {
    QStringList TitleSpisPogruzh;
    TitleSpisPogruzh<<tr("Код записи")<<tr("Наименование")<<tr("№ погружения")<<tr("Дата погружения")<<tr("Время начала работ")<<tr("Время окончания")<<tr("Код шаблона")<<tr("Готовность к работе");
    for (int i = 0; i < TitleSpisPogruzh.count(); i++)
      ProxyModelSpisPogruzh->setHeaderData(i, Qt::Horizontal, TitleSpisPogruzh.at(i));
    // Спрячем ненужные пока колонки, но они пригодятся нам позже
    ui->tblSpisPogruzh->setColumnHidden(0,true);
    ui->tblSpisPogruzh->setColumnHidden(1,true);
    ui->tblSpisPogruzh->setColumnHidden(5,true);
    ui->tblSpisPogruzh->setColumnHidden(6,true);
    ui->tblSpisPogruzh->setColumnHidden(7,true);

    // Рaвняем по ширине
    ui->tblSpisPogruzh->resizeColumnsToContents();
    //}


    // отдебажим запрос
//    qDebug() << "Model query last error " << ModelSpisPogruzh->query().lastError();
//    qDebug() << "Model query last error " << ModelSostTS->query().lastError();

    // Ставим изначально сортировку на 0-колонку
    ui->tblSpisPogruzh->sortByColumn(0, Qt::AscendingOrder);


}

//-------------------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,true);
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_F1:
            qTC->showMainHelp();
        break;
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::CloseWindowMain()
{
    this->close();
}
//-------------------------------------------------------------------------------------------------

void MainWindow::CreateWinAbout()
{
    qTC->showAbout(tr("ОКР Растворение \n\"Ведение информации о состоянии технических средств, используемых при проведении спасательных работ\""), "1.0");
}
//-------------------------------------------------------------------------------------------------

  // В этот слот мы забредем в случае, если тупо нажали кнопку "Редактировать НСИ"
 // Будет вызвана форма редактирования настроенная на тот ТС, к-рый сейчас открыт

void MainWindow::CreateEditTemplate()
{

    switch (getCurrentTS().typeTSid)
    {
     case 1: CreateEditTemplate5();
     break;
     case 2: CreateEditTemplate8();
     break;
     case 3: CreateEditTemplate14();
     break;
     default: CreateEditTemplate5();
     break;
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::CreateEditTemplate5()
{
    EditTemplate et(5);
    et.exec();
}

//-------------------------------------------------------------------------------------------------

void MainWindow::CreateEditTemplate6()
{
    EditTemplate et(6);
    et.exec();
}
//-------------------------------------------------------------------------------------------------
void MainWindow::CreateEditTemplate7()
{
    EditTemplate et(7);
    et.exec();
}
//-------------------------------------------------------------------------------------------------
void MainWindow::CreateEditTemplate8()
{
    EditTemplate et(8);
    et.exec();
}
//-------------------------------------------------------------------------------------------------
void MainWindow::CreateEditTemplate9()
{
    EditTemplate et(9);
    et.exec();
}
//-------------------------------------------------------------------------------------------------
void MainWindow::CreateEditTemplate10()
{
    EditTemplate et(10);
    et.exec();
}
//-------------------------------------------------------------------------------------------------
void MainWindow::CreateEditTemplate11()
{
    EditTemplate et(11);
    et.exec();
}
//-------------------------------------------------------------------------------------------------
void MainWindow::CreateEditTemplate12()
{
    EditTemplate et(12);
    et.exec();
}
//-------------------------------------------------------------------------------------------------
void MainWindow::CreateEditTemplate13()
{
    EditTemplate et(13);
    et.exec();
}
//-------------------------------------------------------------------------------------------------
void MainWindow::CreateEditTemplate14()
{
    EditTemplate et(14);
    et.exec();
}

//-------------------------------------------------------------------------------------------------
void MainWindow::CreateEditTemplate15()
{
    EditTemplate et(15);
    et.exec();
}
//-------------------------------------------------------------------------------------------------

void MainWindow::CreateClrWindow()
{
    SetColorForm sclw;
    sclw.exec();
}

//-------------------------------------------------------------------------------------------------

void MainWindow::CreateSndWindow()
{
    SetSoundForm sndw;
    sndw.exec();
}

//-------------------------------------------------------------------------------------------------

void MainWindow::SelectSGA()
{
    ui->menuSelectSGA->setChecked(true);
    ui->menuSelectGVS->setChecked(false);
    ui->menuSelectNPA->setChecked(false);

    // Зафиксируем режим работы (СГА)
    setRabMode(1);

    SetCurrentMode();

    QuerySpisPogruzh->clear();
    QuerySpisPogruzh->prepare("SELECT code,name,nomp,datep,begintime,endtime,codesh,prizn FROM z18_spts.p_sgapred");
    QuerySpisPogruzh->exec();


}
//-------------------------------------------------------------------------------------------------

void MainWindow::SelectGVS()
{
    ui->menuSelectSGA->setChecked(false);
    ui->menuSelectGVS->setChecked(true);
    ui->menuSelectNPA->setChecked(false);

    // Зафиксируем режим работы (ЖВС)
    setRabMode(2);

    SetCurrentMode();

    QuerySpisPogruzh->clear();
    QuerySpisPogruzh->prepare("SELECT code,name,nomp,datep,begintime,endtime,codesh,prizn FROM z18_spts.p_gvspred");
    QuerySpisPogruzh->exec();


}
//-------------------------------------------------------------------------------------------------
void MainWindow::SelectNPA()
{
    ui->menuSelectSGA->setChecked(false);
    ui->menuSelectGVS->setChecked(false);
    ui->menuSelectNPA->setChecked(true);

    // Зафиксируем режим работы (НПА)
    setRabMode(3);

    SetCurrentMode();

    QuerySpisPogruzh->clear();
    QuerySpisPogruzh->prepare("SELECT code,name,nomp,datep,begintime,endtime,codesh,prizn FROM z18_spts.p_sgapred");
    QuerySpisPogruzh->exec();


}
//-------------------------------------------------------------------------------------------------


void MainWindow::ChangeCurrentPage(int index)
{

    setRabModeCurrentTS(index);

    switch(index)
    {
    case 0:
        ui->frame->setVisible(false);
        ui->tblSostTS->setFocus(Qt::ActiveWindowFocusReason);
        ui->tblSostTS->selectRow(0);
    break;
    default:
        ui->frame->setVisible(true);
        ui->tblSpisPogruzh->setFocus(Qt::ActiveWindowFocusReason);
        ui->tblSpisPogruzh->selectRow(0);
    break;
    }


     // если выбрана 1я вкладка ТабВыджета, то скрываем список погружений
    // если любая другая, то отображаем список
    if (index == 0)
    {
        ui->frame->setVisible(false);
        ui->tblSostTS->setFocus(Qt::ActiveWindowFocusReason);
        ui->tblSostTS->selectRow(0);

    }
    // если выбрана 2я вкладка ТабВыджета
    else if (index == 1)
    {
        ui->frame->setVisible(true);
        ui->tblSpisPogruzh->setFocus(Qt::ActiveWindowFocusReason);
        ui->tblSpisPogruzh->selectRow(0);

    }
    // если выбрана 3я вкладка ТабВыджета
    else if (index == 2)
    {
        ui->frame->setVisible(true);
        ui->tblSpisPogruzh->setFocus(Qt::ActiveWindowFocusReason);
        ui->tblSpisPogruzh->selectRow(0);

    }
    // если выбрана 4я вкладка ТабВыджета
    else if (index == 3)
    {
        ui->frame->setVisible(true);
        ui->tblSpisPogruzh->setFocus(Qt::ActiveWindowFocusReason);
        ui->tblSpisPogruzh->selectRow(0);


    }

}

//-------------------------------------------------------------------------------------------------
void MainWindow::SelectActiveComponent(QModelIndex index)
{
    currentMdIndex = index;
}

//-------------------------------------------------------------------------------------------------

void MainWindow::HelpAction()
{
    qTC->showMainHelp();
}

//-------------------------------------------------------------------------------------------------

void MainWindow::FormChoiceAdd()

{
    // Если открыта 1я вкладка на табВыджете, то открываем форму добавления ТС
    switch (getRabModeCurrentTS())
    {
        case 0:
            NewTS ts;
            ts.exec();
        break;
        case 1: // Открыта вкладка предспусковых работ
            // Вызываем формы добавления в зависимости от режима работы (СГА, ЖВC, НПА)
            switch (getRabMode())
            {
                case 1: // Режим СГА-----------------
                  p_sgapredForm psgap(true,true,-1,this);
                  psgap.exec();
                break;
                case 2: // Режим ЖВС-----------------
                  p_gvspredForm pgvsp(true,true,-1,this);
                  pgvsp.exec();
                break;
                case 3: // Режим НПА-----------------
                  p_npapredForm pnpap(true,true,-1,this);
                  pnpap.exec();
                break;
                default:  // Если никуда не попали, то это сбой
                  QMessageBox::critical (0, QObject::tr("Ошибка добавления данных"), msqErrorAddRecord.toLatin1());
                break;
            }
        break;
        case 2: // Открыта вкладка спусковых работ
            // Вызываем формы добавления в зависимости от режима работы (СГА, ЖВC, НПА)
            switch (getRabMode())
            {
                case 1: // Режим СГА-----------------
                  p_sgaspuskForm psgas(true,-1,this);
                  psgas.exec();
                break;
                case 2: // Режим ЖВС-----------------
                  p_gvsspuskForm pgvss(true,-1,this);
                  pgvss.exec();
                break;
                case 3: // Режим НПА-----------------
                  p_npaspuskForm pnpas(true,-1,this);
                  pnpas.exec();
                break;
                default:  // Если никуда не попали, то это сбой
                  QMessageBox::critical (0, QObject::tr("Ошибка добавления данных"), msqErrorAddRecord.toLatin1());
                break;
            }
        break;
        case 3: // Открыта вкладка спусковых работ
            // Вызываем формы добавления в зависимости от режима работы (СГА, ЖВC, НПА)
            switch (getRabMode())
            {
                case 1: // Режим СГА-----------------
                  p_sgapredForm psgap(true,false,-1,this);
                  psgap.exec();
                break;
                case 2: // Режим ЖВС-----------------
                  p_gvspredForm pgvsp(true,false,-1,this);
                  pgvsp.exec();
                break;
                case 3: // Режим НПА-----------------
                  p_npapredForm pnpap(true,false,-1,this);
                  pnpap.exec();
                break;
                default:  // Если никуда не попали, то это сбой
                  QMessageBox::critical (0, QObject::tr("Ошибка добавления данных"), msqErrorAddRecord.toLatin1());
                break;
            }
        break;
        // сюда попадём, если в программе глюк---
        default:
            QMessageBox::critical(0,tr("Добавление данных"),msqErrorAddRecord.toLatin1(), QMessageBox::Ok);
            return;
        break;
    }

    // Надо бы обновить список погружений после этой формы
    // и поставить курсор на вновь добавленную запись
    // Фишка в том, что в обычной несортированной модели новая запись добавляется в конец модели как последняя
    ModelSpisPogruzh->setQuery(ModelSpisPogruzh->query().lastQuery(), getDbName());
    ui->tblSpisPogruzh->setFocus(Qt::ActiveWindowFocusReason);
    QModelIndex Index = ModelSpisPogruzh->index(ModelSpisPogruzh->rowCount() - 1, 0, QModelIndex());
    if (Index.isValid())
       ui->tblSpisPogruzh->setCurrentIndex(ProxyModelSpisPogruzh->mapFromSource(Index));



}

//-------------------------------------------------------------------------------------------------

void MainWindow::FormChoiceEdit()

{
  /*  // Если открыта 1я вкладка на табВыджете, то открываем форму редактирования положения состояния ТС
    if (ui->tabWidget->currentIndex() == 0)
    {
        NewTS nts;
        nts.exec();
    }
    // Если открыта 2я вкладка на табВыджете, то открываем форму редактирования погружения текущего ТС
    if (ui->tabWidget->currentIndex() == 1)
    {
         // Передаём в форму булевский признак редактирования - false
        // Признак ПРЕДпогружных работ true и код текущей записи и
        int RecNum = ModelSpisPogruzh->record(ProxyModelSpisPogruzh->mapToSource(ui->tblSpisPogruzh->currentIndex()).row()).value(0).toInt();
        p_sgapredForm psgap(false,true,RecNum);
        psgap.exec();
     }
*/
}

//-------------------------------------------------------------------------------------------------

void MainWindow::DeleteRecord()

{
    if (QMessageBox::question(0,tr("Удаление данных"), msgAskDeleteCurrentRec.toLatin1(),QMessageBox::Yes, QMessageBox::Cancel) == QMessageBox::Cancel)
        return;

    QSqlQuery DelQuery(getDbName());
    // Запомним позицию текущей записи
    QModelIndex Index = ProxyModelSpisPogruzh->mapToSource(ui->tblSpisPogruzh->currentIndex());
    if (!Index.isValid())
       return;
    // Запомним номер текущей записи
    int RecNum = ModelSpisPogruzh->record(ProxyModelSpisPogruzh->mapToSource(ui->tblSpisPogruzh->currentIndex()).row()).value(0).toInt();

    DelQuery.prepare("DELETE FROM z18_spts.p_sgapred WHERE code = :pcode");
    DelQuery.bindValue(":pcode", RecNum);

//    qDebug() << DelQuery.lastError();
//    qDebug() << ModelSpisPogruzh->record(Index.row()).value(0).toString();
//    qDebug() << ModelSpisPogruzh->record(Index.row()).value(1).toDate().toString("yyyy-MM-dd");

    getDbName().transaction();
    if (DelQuery.exec())
    {
        getDbName().commit();
        // номер текущей строки (выделенной)
        int CurrentRow = ui->tblSpisPogruzh->currentIndex().row();
        // Надо бы обновить список погружений после всего
        ModelSpisPogruzh->setQuery (ModelSpisPogruzh->query().lastQuery(), getDbName()); // обновить БД
        // Курсор ставим на предыдущую запись, если она не 1я
        if (ModelSpisPogruzh->rowCount()-1 < CurrentRow)
          CurrentRow--;

        // переставляем курсор на новую
        ui->tblSpisPogruzh->setFocus(Qt::ActiveWindowFocusReason);
        ui->tblSpisPogruzh->selectRow(CurrentRow);
    }
    else // если не удалилось - ругаемся
    {
        getDbName().rollback();
        QMessageBox::critical(0,tr("Удаление данных"), msgErrorDeleteCurrentRec.toLatin1(),QMessageBox::Ok);
    }

}

//-------------------------------------------------------------------------------------------------
void MainWindow::ShortCutFindAction()
{
    // Если кнопа не чекнута - чекаем и наоборот
    actFindAction->setChecked(!actFindAction->isChecked());
    FindAction();
}

//-------------------------------------------------------------------------------------------------

void MainWindow::FindAction()
{
    Find::instance(this)->setVisible(actFindAction->isChecked());
}

//-------------------------------------------------------------------------------------------------

void MainWindow::ChangingTables()
{

    // Связываем прохи модель с моделью
    ProxyModelSpisPlaceTS->setSourceModel(ModelSostTS);
    ProxyModelSpisPogruzh->setSourceModel(ModelSpisPogruzh);
    // Связываем ТейблВьюху с моделью
    ui->tblSostTS->setModel(ProxyModelSpisPlaceTS);
    ui->tblSpisPogruzh->setModel(ProxyModelSpisPogruzh);

    //Обновляем текст скуэль запросов
    QuerySostTS->clear();
    QuerySostTS->prepare("SELECT code,name,nomp,datep FROM z18_spts.p_sgapred");
    QuerySostTS->exec();

    QuerySpisPogruzh->clear();
    QuerySpisPogruzh->prepare("SELECT code,name,nomp,datep,begintime,endtime,codesh,prizn FROM z18_spts.p_sgapred");
    QuerySpisPogruzh->exec();

    // в этом случае, чтобы увидеть результаты, необходимо присвоить модели запрос ПОСЛЕ выполнения запроса
    ModelSostTS->setQuery(*QuerySostTS);
    ModelSpisPogruzh->setQuery(*QuerySpisPogruzh);

    // грузим столбцы
    //if (!qTC->tableLoad(ui->tblSpisPogruzh, ModelSpisPogruzh))
    // {
    QStringList TitleSpisPlaceTS;
    TitleSpisPlaceTS<<tr("Ангар/Контейнер")<<tr("Подготовка к работе")<<tr("Работа")<<tr("Дежурство");
    for (int i = 0; i < TitleSpisPlaceTS.count(); i++)
      ProxyModelSpisPlaceTS->setHeaderData(i, Qt::Horizontal, TitleSpisPlaceTS.at(i));
    ui->tblSostTS->resizeColumnsToContents();

    //}


    // грузим столбцы
    //if (!qTC->tableLoad(ui->tblSpisPogruzh, ModelSpisPogruzh))
    // {
    QStringList TitleSpisPogruzh;
    TitleSpisPogruzh<<tr("Код записи")<<tr("Наименование")<<tr("№ погружения")<<tr("Дата погружения")<<tr("Время начала работ")<<tr("Время окончания")<<tr("Код шаблона")<<tr("Готовность к работе");
    for (int i = 0; i < TitleSpisPogruzh.count(); i++)
      ProxyModelSpisPogruzh->setHeaderData(i, Qt::Horizontal, TitleSpisPogruzh.at(i));
    // Спрячем ненужные пока колонки, но они пригодятся нам позже
    ui->tblSpisPogruzh->setColumnHidden(0,true);
    ui->tblSpisPogruzh->setColumnHidden(1,true);
    ui->tblSpisPogruzh->setColumnHidden(5,true);
    ui->tblSpisPogruzh->setColumnHidden(6,true);
    ui->tblSpisPogruzh->setColumnHidden(7,true);

    // Ровняем по ширине
    ui->tblSpisPogruzh->resizeColumnsToContents();
    //}


    // отдебажим запрос
//    qDebug() << "Model query last error " << ModelSpisPogruzh->query().lastError();
//    qDebug() << "Model query last error " << ModelSostTS->query().lastError();

    // Ставим изначально сортировку на 0-колонку
    ui->tblSpisPogruzh->sortByColumn(0, Qt::AscendingOrder);

}

//-------------------------------------------------------------------------------------------------
 // Выставляем наименования закладок TabWidget из массива,
// Отображаем в таблице состояние всех ТС
void MainWindow::SetCurrentMode()
{

    int RabMode = getRabMode();
    int i = 0;
    int j = ui->tabWidget->count();

    // Переберем все вкладки TabWidget, назначим им Caption из массива
    for (i=0; i<=j-1; i++)
    {
        ui->tabWidget->setTabText(i,TabWidgetTexts[RabMode-1][i].toLatin1());
    }

    QString SqlString = QString("select s.name,");
                  SqlString.append("case when p.nompp=1 then st.name end as col1, ");
                  SqlString.append("case when p.nompp=2 then st.name end as col2, ");
                  SqlString.append("case when p.nompp=3 then st.name end as col3, ");
                  SqlString.append("case when p.nompp=4 then st.name end as col4 ");
                  SqlString.append(QString("from %1 s left outer join z18_spts.kls_polts p on (p.code=s.codepol) ").arg(ModelSostTSTables[RabMode-1]));
                  SqlString.append("left outer join ");
                  SqlString.append("z18_spts.kls_sostts st on (st.code=s.codesost) ");
                  SqlString.append(QString("where (p.idts=%1)").arg(RabMode));

                  //в запросе будет меняться idts  - это выбранный режим работы


    QuerySostTS->clear();
    QuerySostTS->prepare(SqlString);
    QuerySostTS->exec();

    // Свяжем запрос с моделью
    ModelSostTS->setQuery(*QuerySostTS);

    // Отрисуем наименования столбцов во въюхе
    QString str;
    str.setNum(RabMode);
    SqlString = QString("SELECT name FROM %1 WHERE idts = %2 ORDER BY nompp").arg("z18_spts.kls_polts", str);
    QSqlQuery QSost;
    QSost.prepare(SqlString);
    QSost.exec();

    QSost.first();

    ModelSostTS->setHeaderData(0, Qt::Horizontal, "Наименование ТС");

    for (int i = 1; i <= QSost.size(); i++)
    {
        ModelSostTS->setHeaderData(i, Qt::Horizontal, QSost.record().value(0).toString());
        QSost.next();
    }

    // Выравниваем ширину колонок по содержимому
    ui->tblSostTS->resizeColumnsToContents();


    // Перечень работ на первой закладке не виден
    ui->frame->setVisible(false);

    // Текущая вкладка ТабВыджета - 1ая,
    ui->tabWidget->setCurrentIndex(0);

    // Ставим фокус на таблицу и выделяем 1ю строку
    ui->tblSostTS->setFocus(Qt::ActiveWindowFocusReason);
    ui->tblSostTS->selectRow(0);

}
//-------------------------------------------------------------------------------------------------



