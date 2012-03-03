#include "newts.h"
#include "ui_newts.h"
#include <QSqlQueryModel>
#include <QColor>
#include "taskconfig/taskconfig.h"
#include <QMessageBox>
#include "moduleconst.h"
#include "modulevar.h"

//---------------------------------------------------------------------------------------------

NewTS::NewTS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTS)
{
    ui->setupUi(this);

    // Грузим установки формы из конфигурационного файла
    qTC->formLoad(this, FormSetFile, false);

    connect (ui->buttonBox, SIGNAL(accepted()), this, SLOT(CloseWindow()));  // кнопка 'Отмена'
    connect (ui->buttonBox, SIGNAL(rejected()), this, SLOT(SaveRecord()));  // кнопка "Ок"
    connect (ui->cmbBoxShip, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnShipChange(QString)));     // на смену комбобоха 'Судно-носитель'
    connect (ui->cmbBoxPlaceTS, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnPlaceChange(QString)));// на смену комбобоха 'Местоположение ТС'
    connect (ui->cmbBoxSostTS, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnSostChange(QString))); // на смену комбобоха 'Состояние ТС'


    // Очистим все комбобохсы
    ui->cmbBoxShip->clear();
    ui->cmbBoxPripisTS->clear();
    ui->cmbBoxSostTS->clear();
    ui->cmbBoxPlaceTS->clear();

    // Наполним КомбоБох суднами-носителями
    /*QSqlQueryModel *model_kor = new QSqlQueryModel;

    model_kor->setQuery("SELECT namesl from spr.sprav_mvcp", Main_DB());
    ui->MVCP_comboBox->setModel(model_kor);
    ui->MVCP_comboBox->setModelColumn(0);
    */

    // пока что вручную
    ui->chkBoxPohod->setChecked(false);
    ui->chkBoxPohod->setEnabled(false);

    ui->cmbBoxShip->addItem(tr("Красный Марат"));
    ui->cmbBoxShip->addItem(tr("Будён Семённый"));
    ui->cmbBoxShip->addItem(tr("Семен Фарада"));

    ui->cmbBoxShip->setCurrentIndex(0);

     // Заполняем комбобох положений ТС
    // (пока что вручную)
    ui->cmbBoxPlaceTS->addItem(tr("Ангар"));
    ui->cmbBoxPlaceTS->addItem(tr("Подготовка к работе"));
    ui->cmbBoxPlaceTS->addItem(tr("Работа"));
    ui->cmbBoxPlaceTS->addItem(tr("Дежурство"));

    // Заполняем комбобох состояний ТС
    ui->cmbBoxSostTS->addItem(tr("Исправлен, к работе готов"));
    ui->cmbBoxSostTS->addItem(tr("К работе не готов"));
    ui->cmbBoxSostTS->addItem(tr("Не исправлен"));
    ui->cmbBoxSostTS->addItem(tr("Аварийное состояние"));



    // UNDER CONSTRUCTION
    // Создаём выджет
   /* QWidget *mycombo = new QWidget (this);
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

    */

}

//---------------------------------------------------------------------------------------------

NewTS::~NewTS()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,false);
    delete ui;
}

//---------------------------------------------------------------------------------------------

void NewTS::changeEvent(QEvent *e)
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

void NewTS::CloseWindow()
{
    this->close();
}


//---------------------------------------------------------------------------------------------

void NewTS::SaveRecord()
{

    QSqlQuery queryInsert(getDbName());
    if (ui->cmbBoxPripisTS->currentText().isEmpty())
    {
        QMessageBox::warning(0,tr("Добавление данных"),tr("Не заполнено поле 'Приписанные технические средства'"),QMessageBox::Ok);
        ui->cmbBoxPripisTS->setFocus();
        return;
    }

/*    queryInsert.prepare("INSERT INTO 0313_mlo.ntd.otmennd(col1, col2, col3) VALUES (:col1, :col2, :col3)");
    queryInsert.bindValue(":col1", ui->cmbBoxPripisTS->currentText().trimmed());
    queryInsert.bindValue(":col1", ui->cmbBoxPlaceTS->currentText().trimmed());
    queryInsert.bindValue(":col1", ui->cmbBoxSostTS->currentText().trimmed());

    if (!queryInsert.exec())
    {
        QMessageBox::critical(0,tr("Добавление данных"),tr("Ошибка при добавлении данных"),QMessageBox::Ok);
        return;
    }
    else
    {
        this->close();
    }
*/

}

//---------------------------------------------------------------------------------------------

void NewTS::OnShipChange(QString currentComboText)
{

    // в зависимости от выбранного судна-носителя отобразим принадлежащие ему ТС
   // пока что вручную
   ui->cmbBoxPripisTS->clear();
   ui->cmbBoxPripisTS->addItem(tr("ТС1"));
   ui->cmbBoxPripisTS->addItem(tr("ТС2"));
   ui->cmbBoxPripisTS->addItem(tr("ТС2234"));
   ui->cmbBoxPripisTS->setCurrentIndex(0);

}

//---------------------------------------------------------------------------------------------

void NewTS::OnPlaceChange(QString currentComboText)
{
    ui->chkBoxPohod->setChecked(false);

    if (currentComboText == tr("Ангар"))
    {
        ui->chkBoxPohod->setEnabled(true);
    }
    else
    {
        ui->chkBoxPohod->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------

void NewTS::OnSostChange(QString currentComboText)
{


    if (currentComboText == tr("Исправлен, к работе готов"))
    {
        ui->cmbBoxSostTS->setPalette(QPalette(getReadyColor()));
    }
    if (currentComboText == tr("К работе не готов"))
    {
        ui->cmbBoxSostTS->setPalette(QPalette(getNotReadyColor()));
    }
    if (currentComboText == tr("Не исправлен"))
    {
        ui->cmbBoxSostTS->setPalette(QPalette(getNotReadyColor()));
    }
    if (currentComboText == tr("Аварийное состояние"))
    {
        ui->cmbBoxSostTS->setPalette(QPalette(getAvarColor()));
    }
}

//---------------------------------------------------------------------------------------------
