#include "edittemplatestring3.h"
#include "ui_edittemplatestring3.h"
#include "taskconfig.h"
#include "moduleconst.h"


//---------------------------------------------------------------------------------------------
// В конструкторе формы внесли изменения, чтобы передать в нее булевский параметр
EditTemplateString3::EditTemplateString3 (bool priz, QWidget *parent) :  QDialog(parent), ui(new Ui::EditTemplateString3)
{
    ui->setupUi(this);

    connect (ui->buttonBox, SIGNAL(accepted()), this, SLOT (SaveRecord()));      // Кнопка 'Ok'
    connect (ui->buttonBox, SIGNAL(rejected()), this, SLOT (CloseWindow()));     // Кнопка 'Отмена'


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

EditTemplateString3::~EditTemplateString3()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,false);
    delete ui;
}

//---------------------------------------------------------------------------------------------

void EditTemplateString3::changeEvent(QEvent *e)
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

void EditTemplateString3::SaveRecord()
{
    this->close();
}

//---------------------------------------------------------------------------------------------

void EditTemplateString3::CloseWindow()
{
    this->close();
}

//---------------------------------------------------------------------------------------------
