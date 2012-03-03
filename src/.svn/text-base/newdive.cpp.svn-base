#include "newdive.h"
#include "ui_newdive.h"
#include "taskconfig/taskconfig.h"
#include "moduleconst.h"
#include "modulevar.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>
#include <QString>

//---------------------------------------------------------------------------------------------


NewDiveWindow::NewDiveWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDiveWindow)
{
    ui->setupUi(this);

    // Соединяем сигналы со слотами:
    connect (ui->buttonBox, SIGNAL(accepted()), this, SLOT (SaveRecord()));    //кнопка "Ok"
    connect (ui->buttonBox, SIGNAL(rejected()), this, SLOT (CloseWindow()));  // кнопка "Закрыть"

    // Грузим установки формы из конфигурационного файла
    qTC->formLoad(this, FormSetFile, false);

    QString tmpString = tr("Новое погружение ");
    setWindowTitle(tmpString.append(getCurrentTS().typeTS.toLatin1()));

    // Найдем макс № погружения и услужливо подставим его
    QSqlQuery queryMax(getDbName());
    queryMax.clear();
    queryMax.prepare("SELECT MAX(numoakt) AS maxnumoakt FROM ntd.otmennd");

    // Если почему-то max № не выбрался - ставим '1'
    if (!queryMax.exec())
    {  ui->lineEditNumber->setText("1");}
    else
    {
      queryMax.first();
      int mxInt = queryMax.value(0).toInt();
      mxInt++;
      ui->lineEditNumber->setText(QString::number(mxInt));
    }

    // берем текущую дату
    QDate curdate = QDate::currentDate();
    ui->dateEditPogruzh->clear();
    ui->dateEditPogruzh->setDate(curdate);



}

//---------------------------------------------------------------------------------------------

NewDiveWindow::~NewDiveWindow()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,false);
    delete ui;
}

//---------------------------------------------------------------------------------------------

void NewDiveWindow::changeEvent(QEvent *e)
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
void NewDiveWindow::SaveRecord()
{

    QSqlQuery queryInsert(getDbName());
    if (ui->lineEditNumber->text().isEmpty())
    {
        QMessageBox::warning(0, tr("Добавление данных"), tr("Не заполнено поле 'Номер погружения'"), QMessageBox::Ok);
        ui->lineEditNumber->setFocus();
        return;
    }

    // Проверим, имеется ли уже похожая запись
    queryInsert.clear();
    queryInsert.prepare("SELECT COUNT(*) AS totalzap FROM ntd.otmennd WHERE numoakt=:numoakt AND dateoakt=:dateoakt");
    queryInsert.bindValue(":numoakt", ui->lineEditNumber->text().trimmed());
    queryInsert.bindValue(":dateoakt", ui->dateEditPogruzh->date());
    if (!queryInsert.exec())
    {
        QMessageBox::critical(0, tr("Добавление данных"), tr("Произошла ошибка при добавлении данных!"), QMessageBox::Ok);
        return;
    }
    else // Если запрос сработал корректно
    {
        // если такая запись уже есть в таблице - ругаемся и выходим
        queryInsert.first();
        if (queryInsert.value(0).toInt() > 0)
        {
          QMessageBox::warning(0, tr("Добавление данных"), tr("Подобная запись уже имеется в таблице!"), QMessageBox::Ok);
          return;
        }
    }

    // Если все проверки пройдены успешно, вставляем запись
    queryInsert.clear();
    queryInsert.prepare("INSERT INTO ntd.otmennd(numoakt, dateoakt) VALUES (:numoakt, :dateoakt)");
    queryInsert.bindValue(":numoakt", ui->lineEditNumber->text().trimmed());
    queryInsert.bindValue(":dateoakt", ui->dateEditPogruzh->date());

    if (!queryInsert.exec())
    {
        QMessageBox::critical(0,tr("Добавление данных"),tr("Ошибка при добавлении данных"), QMessageBox::Ok);
        return;
    }
    else  // если всё получилось
    {
        this->close();
    }
}

//---------------------------------------------------------------------------------------------

void NewDiveWindow::CloseWindow()
{
    this->close();
}
//---------------------------------------------------------------------------------------------
