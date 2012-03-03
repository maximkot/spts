#include "edittemplate.h"
#include "ui_edittemplate.h"
#include "edittemplatestring1.h"
#include "edittemplatestring2.h"
#include "edittemplatestring3.h"
#include "taskconfig/taskconfig.h"
#include "moduleconst.h"



//---------------------------------------------------------------------------------------------

EditTemplate::EditTemplate(int NumTemplate, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTemplate)
{
    ui->setupUi(this);

    // Грузим установки формы из конфигурационного файла
    qTC->formLoad(this, FormSetFile, false);

    // Вводим комбинации горячих клавиш для данной формы
    // Затем связываем эти шоткаты сигналами со слотами
                                                         /*
    Новая запись          Ctrl+N
    Редактировать запись  Ctrl+E
    Удалить запись        Ctrl+X
    Открыть поиск         Ctrl+F
    Вызвать помощь        Ctrl+H (а также F1, к-рый отлавливается через KeyEvent)
    Закрыть форму         Ctrl+Q   */

    QShortcut *exitShortcut  = new QShortcut(QKeySequence("Ctrl+Q"),this,0);
    QShortcut *addShortcut   = new QShortcut(QKeySequence("Ctrl+N"),this,0);
    QShortcut *editShortcut  = new QShortcut(QKeySequence("Ctrl+E"),this,0);
    QShortcut *delShortcut   = new QShortcut(QKeySequence("Ctrl+X"),this,0);
    QShortcut *findShortcut  = new QShortcut(QKeySequence("Ctrl+F"),this,0);
    QShortcut *hlpShortcut   = new QShortcut(QKeySequence("Ctrl+H"),this,0);
    QShortcut *upShortcut    = new QShortcut(QKeySequence("Ctrl+U"),this,0);
    QShortcut *downShortcut  = new QShortcut(QKeySequence("Ctrl+D"),this,0);

     // Соединяем сигналы и слоты:
    // Нажатие шоткатов
    connect (exitShortcut,  SIGNAL (activated()), this, SLOT (CloseWindow()));
    connect (addShortcut,   SIGNAL (activated()), this, SLOT (CreateAddTemplateString()));
    connect (editShortcut,  SIGNAL (activated()), this, SLOT (CreateEditTemplateString()));
    connect (delShortcut,   SIGNAL (activated()), this, SLOT (CreateEditTemplateString()));
    connect (findShortcut,  SIGNAL (activated()), this, SLOT (CreateEditTemplateString()));
    connect (hlpShortcut,   SIGNAL (activated()), this, SLOT (HelpAction()));
    connect (upShortcut,    SIGNAL (activated()), this, SLOT (UpRecord()));
    connect (downShortcut,  SIGNAL (activated()), this, SLOT (DownRecord()));


     //--- Порисуем динамически менюшки и тулбары:
    // Создаём выджет
    QWidget *w = new QWidget (this);
    // от Выджета создаём тулбар и менюбар
    QToolBar *tb = new QToolBar (w);
    QMenuBar *mb = new QMenuBar (w);
    // Выравниваем по лэйауту
    w->setLayout(new QVBoxLayout (w));
    w->layout()->setContentsMargins(3, 3, 3, 3);
    QMenu *actionMenu = mb->addMenu(tr("Действия"));
    QMenu *actionMenu2 = mb->addMenu(tr("Закрыть"));
    w->layout()->addWidget(mb);
    w->layout()->addWidget(tb);
    this->layout()->setMenuBar(w);


     // Рисуем в тулбаре кнопки сиконками
    // Добавить
    QIcon newIcon;
    newIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/new.png"), QSize(), QIcon::Normal, QIcon::Off);
    actNewRecord = new QAction (newIcon, tr("Добавить запись\n(Ctrl+N)"), this);
    // Изменить
    QIcon editIcon;
    editIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
    actEditRecord = new QAction (editIcon, tr("Редактировать запись\n(Ctrl+E)"), this);
    // Удалить
    QIcon delIcon;
    delIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
    actDelRecord = new QAction (delIcon, tr("Удалить запись\n(Ctrl+X)"), this);
    // Поиск
    QIcon findIcon;
    findIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/find.png"), QSize(), QIcon::Normal, QIcon::Off);
    actFindRecord = new QAction (findIcon, tr("Поиск (Ctrl+F)"), this);
    // Кнопка 'Вниз'
    QIcon downIcon;
    downIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/down.png"), QSize(), QIcon::Normal, QIcon::Off);
    actDownRecord = new QAction (downIcon, tr("Переместить запись вниз\n(Ctrl+D)"), this);
    // Кнопка 'Вверх'
    QIcon upIcon;
    upIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/up.png"), QSize(), QIcon::Normal, QIcon::Off);
    actUpRecord = new QAction (upIcon, tr("Переместить запись вверх\n(Ctrl+U)"), this);
    // Выход
    QIcon exitIcon;
    exitIcon.addFile(QString::fromUtf8(":/new/buttonpictures/src/icons/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
    actExitAction = new QAction (exitIcon, tr("Закрыть окно\n(Ctrl+Q)"), this);

    // привязываем к тулбару экшонсы
    tb->addAction(actNewRecord);
    tb->addAction(actEditRecord);
    tb->addAction(actDelRecord);
    tb->addAction(actFindRecord);
    // разделитель перед вверх/вниз
    tb->addSeparator();
    tb->addAction(actDownRecord);
    tb->addAction(actUpRecord);
    // вставляем разделитель перед выходом
    tb->addSeparator();
    tb->addAction(actExitAction);

    // привязываем к меню экшонсы
    actionMenu->addAction(actNewRecord);
    actionMenu->addAction(actEditRecord);
    actionMenu->addAction(actDelRecord);
    actionMenu->addAction(actFindRecord);
    actionMenu->addSeparator();
    actionMenu->addAction(actDownRecord);
    actionMenu->addAction(actUpRecord);
    actionMenu->addSeparator();
    actionMenu->addAction(actExitAction);
    actionMenu2->addAction(actExitAction);

    // Соединяем сигналы-слоты
    connect (actNewRecord,  SIGNAL (triggered ()), this, SLOT (CreateAddTemplateString()));   // Экшонс на кнопку/меню "Добавить"
    connect (actEditRecord, SIGNAL (triggered ()), this, SLOT (CreateEditTemplateString())); // Экшонс на кнопку/меню "Редактировать"
    connect (actDelRecord,  SIGNAL (triggered ()), this, SLOT (DelRecord()));               // Экшонс на кнопку/меню "Удалить"
    connect (actFindRecord, SIGNAL (triggered ()), this, SLOT (FindRecord()));             // Экшонс на кнопку/меню "Поиск"
    connect (actDownRecord, SIGNAL (triggered ()), this, SLOT (DownRecord()));            // Экшонс на кнопку/меню "Запись вниз"
    connect (actUpRecord,   SIGNAL (triggered ()), this, SLOT (UpRecord()));             // Экшонс на кнопку/меню "Запись вверх"
    connect (actExitAction, SIGNAL (triggered ()), this, SLOT (CloseWindow()));         // Экшонс на кнопку/меню "Закрыть окно"


    aNumTemplate = NumTemplate;

    // Подпишем заголовок шаблона с которым работаем
    switch (aNumTemplate)
    {
        case 5:  ui->label->setText(tr("Контрольно-проверочный лист предпогружной подготовки ").append(tsSGA.toLatin1()));
        break;
        case 6:  ui->label->setText(tr("Лист учета событий при работе ").append(tsSGA.toLatin1().append(" под водой")));
        break;
        case 7:  ui->label->setText(tr("Контрольно-поверочный лист послепогружной проверки ").append(tsSGA.toLatin1()));
        break;
        case 8:  ui->label->setText(tr("Лист предспусковой проверки ").append(tsGVS.toLatin1()));
        break;
        case 9:  ui->label->setText(tr("Лист предспусковой проверки ").append(tsBDD.toLatin1()));
        break;
        case 10: ui->label->setText(tr("Лист послеспусковой проверки ").append(tsGVS.toLatin1()));
        break;
        case 11: ui->label->setText(tr("Лист послеспусковой проверки ").append(tsBDD.toLatin1()));
        break;
        case 12: ui->label->setText(tr("Лист записи показаний ").append(tsSJO.toLatin1()));
        break;
        case 13: ui->label->setText(tr("Лист проверки ").append(tsGVS.toLatin1().append(" при спуске (подъеме)")));
        break;
        case 14: ui->label->setText(tr("Контрольно-проверочный лист предпогружных проверок ").append(tsNPA.toLatin1()));
        break;
        case 15: ui->label->setText(tr("Контрольно-проверочный лист послепогружных проверок ").append(tsNPA.toLatin1()));
        break;
        default: ui->label->setText(tr("Ошибка идентификации ТС"));
        break;
    }
}

//---------------------------------------------------------------------------------------------

EditTemplate::~EditTemplate()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,false);
    delete ui;
}

//---------------------------------------------------------------------------------------------

void EditTemplate::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//---------------------------------------------------------------------------------------------

void EditTemplate::CloseWindow()
{
    this->close();
}

//---------------------------------------------------------------------------------------------

// слот вызова окна добавления новой записи в шаблон
void EditTemplate::CreateAddTemplateString()
{
    // Признак того, что вызвать окно новой записи в режиме добавления (true)
    bool priznak = true;

    if (aNumTemplate == 6)
    {
        EditTemplateString2 ats(priznak);
        ats.exec();
    }
    else if ((aNumTemplate == 12)||(aNumTemplate == 13))
    {
        EditTemplateString3 ats(priznak);
        ats.exec();
    }
    else
    {
        EditTemplateString1 ats(priznak);
        ats.exec();
    }
}

//---------------------------------------------------------------------------------------------

// слот вызова окна редактирования записи в шаблоне
void EditTemplate::CreateEditTemplateString()
{

    // Признак того, что вызвать окно новой записи в режиме редактирования (false)
    bool priznak = false;

    if (aNumTemplate == 6)
    {
        EditTemplateString2 ats(priznak);
        ats.exec();
    }
    else if ((aNumTemplate == 12)||(aNumTemplate == 13))
    {
        EditTemplateString3 ats(priznak);
        ats.exec();
    }
    else
    {
        EditTemplateString1 ats(priznak);
        ats.exec();
    }
}

//---------------------------------------------------------------------------------------------

void EditTemplate::HelpAction()
{
    qTC->showMainHelp();
}

//---------------------------------------------------------------------------------------------

void EditTemplate::UpRecord()
{
return;
}

//---------------------------------------------------------------------------------------------

void EditTemplate::DownRecord()
{
return;
}

//---------------------------------------------------------------------------------------------

void EditTemplate::FindRecord()
{
return;
}

//---------------------------------------------------------------------------------------------

void EditTemplate::DelRecord()
{
return;
}

//---------------------------------------------------------------------------------------------
