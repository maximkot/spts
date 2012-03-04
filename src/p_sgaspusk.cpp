#include "p_sgaspusk.h"
#include "ui_p_sgaspusk.h"
#include "taskconfig.h"
#include "moduleconst.h"
#include "modulevar.h"
#include "moduleproc.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>
#include <QString>


//-------------------------------------------------------------------------------------------------

p_sgaspuskForm::p_sgaspuskForm(bool AddEdit, int recnum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::p_sgaspuskForm)
{
    ui->setupUi(this);

    // Соединяем сигналы со слотами:
    connect (ui->buttonBox, SIGNAL(accepted()), this, SLOT (SaveRecord()));    //кнопка "Ok"
    connect (ui->buttonBox, SIGNAL(rejected()), this, SLOT (CloseWindow()));  // кнопка "Закрыть"


    // Грузим установки формы из конфигурационного файла
    qTC->formLoad(this, FormSetFile, false);


    // Если жобавляем новую запись, то идем сюда
    if (AddEdit == true)
    {
        QString tmpString = tr("Новое погружение ");
        setWindowTitle(tmpString.append(getCurrentTS().typeTS));

        ui->labelTSName->setText(getCurrentTS().nameTS);

         // Найдем макс № погружения и услужливо подставим его
        // Если при вычислении max индекса возникла ошибка, ф-ция возвращает '-1'
        int maxInt = MaxCode("z18_spts.p_sgaspusk","nomp");

        // Если почему-то max № не выбрался - пишем в Edit '1'
        if (maxInt == -1)
        {
            ui->lineEditNumber->setText("1");
        }
        else
        {
          maxInt++;
          ui->lineEditNumber->setText(QString::number(maxInt));
        }

        // берём текущую дату
        QDate curdate = QDate::currentDate();
        ui->dateEditPogruzh->clear();
        ui->dateEditPogruzh->setDate(curdate);

        ui->lineEditBall->clear();
        ui->lineEditBall->setText("0");

        ui->lineEditLat->clear();
        ui->lineEditLat->setText("0000");

        ui->lineEditLong->clear();
        ui->lineEditLong->setText("0000");

    }

    else  // Если редактируем существующую запись, то идем сюда

    {

  /*      QSqlQuery QPogruzh;
        //                value:  0    1    2    3      4         5         6    7
        QPogruzh.prepare("SELECT code,name,nomp,datep,begintime,endtime,codesh,prizn FROM z18_spts.p_sgapred WHERE code = :precnum");
        QPogruzh.bindValue(":precnum",recnum);
        if (!QPogruzh.exec())
        {
            QMessageBox::critical(0, tr("Редактирование записи"), msgErrorOpenEditRecord.toLatin1(), QMessageBox::Ok);
            CloseWindow();
        }

        QPogruzh.first();

        QString tmpString = tr("Погружение №").append(QPogruzh.value(2).toString());
        setWindowTitle(tmpString);

        ui->labelTSName->setText(getCurrentTS().nameTS);

        ui->lineEditNumber->setText(QPogruzh.value(2).toString());

        // берем дату погружения из записи
        QDate pogrdate = QPogruzh.value(3).toDate();
        ui->dateEditPogruzh->clear();
        ui->dateEditPogruzh->setDate(pogrdate);

        // берем время начала из записи
        QTime pogrstarttime = QPogruzh.value(4).toTime();
        ui->timeEditStartPogruzh->clear();
        ui->timeEditStartPogruzh->setTime(pogrstarttime);

        // берем время конца из записи
        QTime pogrendtime = QPogruzh.value(5).toTime();
        ui->timeEditEndPogruzh->clear();
        ui->timeEditEndPogruzh->setTime(pogrendtime);

        // Готов или не готов
        bool YesNoReady = QPogruzh.value(7).toBool();
        if (YesNoReady == false)
        {
            ui->sostLabel->setText(tr("Не готов"));
            ui->sostLabel->setPalette(QPalette(getNotReadyColor()));
        }
        else
        {
            ui->sostLabel->setText(tr("Готов"));
            ui->sostLabel->setPalette(QPalette(getReadyColor()));
        }
*/

    }




}

//-------------------------------------------------------------------------------------------------

p_sgaspuskForm::~p_sgaspuskForm()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,false);

    delete ui;
}

//-------------------------------------------------------------------------------------------------

void p_sgaspuskForm::changeEvent(QEvent *e)
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

//-------------------------------------------------------------------------------------------------

void p_sgaspuskForm::SaveRecord()

{
   /* if (ui->lineEditNumber->text().trimmed().isEmpty())
    {
        QMessageBox::warning(0, tr("Добавление данных"), msgWarningEmptyNumPogr.toLatin1(), QMessageBox::Ok);
        ui->lineEditNumber->setFocus(Qt::ActiveWindowFocusReason);
        return;
    }

    if (ui->dateEditPogruzh->text().trimmed().isEmpty())
    {
        QMessageBox::warning(0, tr("Добавление данных"), msgWarningEmptyDatePogr.toLatin1(), QMessageBox::Ok);
        ui->dateEditPogruzh->setFocus(Qt::ActiveWindowFocusReason);
        return;
    }

    if (ui->timeEditStartPogruzh->text().trimmed().isEmpty())
    {
        QMessageBox::warning(0, tr("Добавление данных"), msgWarningEmptyTimeSPogr.toLatin1(), QMessageBox::Ok);
        ui->timeEditStartPogruzh->setFocus(Qt::ActiveWindowFocusReason);
        return;
    }

    // Проверим, имеется ли уже похожая запись
    QSqlQuery queryInsert(getDbName());
    queryInsert.clear();
    queryInsert.prepare("SELECT COUNT(*) AS totalzap FROM z18_spts.p_sgapred WHERE (name = :pname)AND(nomp = :pnomp)AND(datep = :pdatep)");
    queryInsert.bindValue(":pname", ui->labelTSName->text().trimmed());
    queryInsert.bindValue(":pnomp", ui->lineEditNumber->text().trimmed());
    queryInsert.bindValue(":pdatep", ui->dateEditPogruzh->date());

    if (!queryInsert.exec())
    {
        QMessageBox::critical(0, tr("Добавление данных"), msqErrorAddRecord.toLatin1(), QMessageBox::Ok);
        return;
    }
    else // Если запрос сработал корректно
    {
        // если такая запись уже есть в таблице - ругаемся и выходим
        queryInsert.first();
        if (queryInsert.value(0).toInt() > 0)
        {
          QMessageBox::warning(0, tr("Добавление данных"), msgWarningSameRecord.toLatin1(), QMessageBox::Ok);
          return;
        }
    }

    // Если все проверки пройдены успешно, вычислим мах № записи и вставляем
    int maxInt = MaxCode("z18_spts.p_sgapred","code");
    maxInt++;
    queryInsert.clear();
    queryInsert.prepare("INSERT INTO z18_spts.p_sgapred (code,name,nomp,datep,begintime,codesh,prizn) VALUES (:pcode,:pname,:pnomp,:pdatep,:pbegintime,:pcodesh,:pprizn)");
    queryInsert.bindValue(":pcode", maxInt);
    queryInsert.bindValue(":pname", ui->labelTSName->text().trimmed());
    queryInsert.bindValue(":pnomp", ui->lineEditNumber->text().trimmed().toInt());
    queryInsert.bindValue(":pdatep", ui->dateEditPogruzh->date());
    maxInt = MaxCode("z18_spts.p_sgapred","codesh");
    maxInt++;
    queryInsert.bindValue(":pcodesh", maxInt);
    queryInsert.bindValue(":pbegintime", ui->timeEditStartPogruzh->time());
    queryInsert.bindValue(":pprizn", "false");

    getDbName().transaction();

    if (!queryInsert.exec())
    {
        QMessageBox::critical(0,tr("Добавление данных"),msqErrorAddRecord.toLatin1(), QMessageBox::Ok);
        getDbName().rollback();
        return;
    }
    else  // если всё получилось
    {
        getDbName().commit();
        this->close();
    }*/
}

//-------------------------------------------------------------------------------------------------

void p_sgaspuskForm::CloseWindow()

{
    this->close();
}

//-------------------------------------------------------------------------------------------------
