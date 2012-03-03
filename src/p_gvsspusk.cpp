#include "p_gvsspusk.h"
#include "ui_p_gvsspusk.h"


//-------------------------------------------------------------------------------------------------

p_gvsspuskForm::p_gvsspuskForm(bool AddEdit, int recnum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::p_gvsspuskForm)
{
    ui->setupUi(this);

    // Соединяем сигналы со слотами:
    connect (ui->buttonBox, SIGNAL(accepted()), this, SLOT (SaveRecord()));    //кнопка "Ok"
    connect (ui->buttonBox, SIGNAL(rejected()), this, SLOT (CloseWindow()));  // кнопка "Закрыть"

}
//-------------------------------------------------------------------------------------------------

p_gvsspuskForm::~p_gvsspuskForm()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void p_gvsspuskForm::changeEvent(QEvent *e)
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

void p_gvsspuskForm::CloseWindow()
{
    this->close();
}

//---------------------------------------------------------------------------------------------

void p_gvsspuskForm::SaveRecord()
{

}

//---------------------------------------------------------------------------------------------
