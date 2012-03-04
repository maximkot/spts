#include "edittemplatestring2.h"
#include "ui_edittemplatestring2.h"
#include "edittemplate.h"
#include "taskconfig.h"
#include "moduleconst.h"

//---------------------------------------------------------------------------------------------

// В конструкторе формы внесли изменения, чтобы передать в нее булевский параметр
EditTemplateString2::EditTemplateString2 (bool priz, QWidget *parent):QDialog(parent), ui(new Ui::EditTemplateString2())
{
    ui->setupUi(this);
    connect (ui->buttonBox,SIGNAL(accepted()),this,SLOT(SaveRecord()));   // Кнопка 'Ok'
    connect (ui->buttonBox,SIGNAL(rejected()),this,SLOT(CloseWindow()));  // Кнопка 'Отмена'

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

EditTemplateString2::~EditTemplateString2()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,false);
    delete ui;
}

//---------------------------------------------------------------------------------------------

void EditTemplateString2::changeEvent(QEvent *e)
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

void EditTemplateString2::SaveRecord()
{
    this->close();
}

//---------------------------------------------------------------------------------------------

void EditTemplateString2::CloseWindow()
{
    this->close();
}

//---------------------------------------------------------------------------------------------
