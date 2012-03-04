#include "edittemplatestring1.h"
#include "ui_edittemplatestring1.h"
#include "taskconfig.h"
#include "moduleconst.h"

//---------------------------------------------------------------------------------------------

// В конструкторе формы внесли изменения, чтобы передать в нее булевский параметр
EditTemplateString1::EditTemplateString1 (bool priz, QWidget *parent) : QDialog(parent), ui(new Ui::EditTemplateString1)
{
    ui->setupUi(this);
    connect (ui->checkBox, SIGNAL(clicked()), this, SLOT (SetItemsVisible()));    // Нажатие чекбоха
    connect (ui->buttonBox, SIGNAL(accepted()),this,SLOT(SaveRecord()));         // кнопка 'Сохранить'
    connect (ui->buttonBox,SIGNAL(rejected()),this,SLOT(CloseWindow()));        // кнопка 'Отмена'

    // Выставим заголовок окна в зависимости от того в каком режиме вызвали
    aPriz = priz;
    if (aPriz == true)
    {setWindowTitle(tr("Добавление новой записи в шаблон"));}
    else
    {setWindowTitle(tr("Редактирование записи в шаблоне")); }

    // Грузим установки формы из конфигурационного файла
    qTC->formLoad(this, FormSetFile, false);


}

//---------------------------------------------------------------------------------------------

EditTemplateString1::~EditTemplateString1()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,false);
    delete ui;
}

//---------------------------------------------------------------------------------------------

void EditTemplateString1::changeEvent(QEvent *e)
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

void EditTemplateString1::SaveRecord()
{
    this->close();
}

//---------------------------------------------------------------------------------------------

void EditTemplateString1::CloseWindow()
{
    this->close();
}

//---------------------------------------------------------------------------------------------

void EditTemplateString1::SetItemsVisible()
{
    if (ui->checkBox->isChecked() == true)
    {
        ui->label_3->setDisabled(true);
        ui->lineEdit_2->setDisabled(true);
    }
    else
    {
        ui->label_3->setDisabled(false);
        ui->lineEdit_2->setDisabled(false);
    }
}

//---------------------------------------------------------------------------------------------
