#include "p_npapred.h"
#include "ui_p_npapred.h"
#include "taskconfig.h"
#include "moduleconst.h"
#include "modulevar.h"
#include "moduleproc.h"
#include <QMessageBox>
#include <QString>


//-------------------------------------------------------------------------------------------------

p_npapredForm::p_npapredForm(bool AddEdit, bool BeforeAfter, int recnum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::p_npapredForm)
{
    ui->setupUi(this);

    // Соединяем сигналы со слотами:
    connect (ui->buttonBox, SIGNAL(accepted()), this, SLOT (SaveRecord()));    //кнопка "Ok"
    connect (ui->buttonBox, SIGNAL(rejected()), this, SLOT (CloseWindow()));  // кнопка "Закрыть"


    // Грузим установки формы из конфигурационного файла
    qTC->formLoad(this, FormSetFile, false);

    // Если ПРЕДпогружная проверка
    if (BeforeAfter == true)
    {

        // Если жобавляем новую запись, то идем сюда
        if (AddEdit == true)
        {
            QString tmpString = tr("Новая предпогружная проверка ");
            setWindowTitle(tmpString.append(getCurrentTS().typeTS));

            ui->labelTSName->setText(getCurrentTS().nameTS);

             // Найдем макс № погружения и услужливо подставим его
            // Если при вычислении max индекса возникла ошибка, ф-ция возвращает '-1'
            int maxInt = MaxCode("z18_spts.p_npapred","nomp");

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

            // берем текущую дату
            QDate curdate = QDate::currentDate();
            ui->dateEditCheck->clear();
            ui->dateEditCheck->setDate(curdate);
            // берем текущее время
            QTime curtime = QTime::currentTime();
            ui->timeEditStartCheck->clear();
            ui->timeEditStartCheck->setTime(curtime);

            ui->lineEditFam->clear();

        }

        else  // Если редактируем существующую запись, то идем сюда

        {

     /*       QSqlQuery QPogruzh;
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

    // Если ПОСЛЕпогружная проверка
    if (BeforeAfter == false)
    {
        // Если жобавляем новую запись, то идем сюда
        if (AddEdit == true)
        {
            QString tmpString = tr("Новая послепогружная проверка ");
            setWindowTitle(tmpString.append(getCurrentTS().typeTS));

            ui->labelTSName->setText(getCurrentTS().nameTS);

             // Найдем макс № погружения и услужливо подставим его
            // Если при вычислении max индекса возникла ошибка, ф-ция возвращает '-1'
            int maxInt = MaxCode("z18_spts.p_sgaposle","nomp");

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

            // берем текущую дату
            QDate curdate = QDate::currentDate();
            ui->dateEditCheck->clear();
            ui->dateEditCheck->setDate(curdate);
            // берем текущее время
            QTime curtime = QTime::currentTime();
            ui->timeEditStartCheck->clear();
            ui->timeEditStartCheck->setTime(curtime);

            ui->lineEditFam->clear();
        }

        else  // Если редактируем существующую запись, то идем сюда

        {

/*            QSqlQuery QPogruzh;
            //                value:  0    1    2    3      4         5         6    7
            QPogruzh.prepare("SELECT code,name,nomp,datep,begintime,endtime,codesh,timeall FROM z18_spts.p_sgaposle WHERE code = :precnum");
            QPogruzh.bindValue(":precnum",recnum);
            if (!QPogruzh.exec())
            {
                QMessageBox::critical(0, tr("Редактирование записи"), msgErrorOpenEditRecord.toLatin1(), QMessageBox::Ok);
                CloseWindow();
            }

            QPogruzh.first();

            QString tmpString = tr("Послепогружная проверка  №").append(QPogruzh.value(2).toString());
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




}

//-------------------------------------------------------------------------------------------------

p_npapredForm::~p_npapredForm()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,false);
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void p_npapredForm::changeEvent(QEvent *e)
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

void p_npapredForm::CloseWindow()
{
    this->close();
}

//---------------------------------------------------------------------------------------------

void p_npapredForm::SaveRecord()
{

}

//---------------------------------------------------------------------------------------------

