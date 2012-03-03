#include <QColorDialog>
#include <QFileDialog>
#include <QColor>
#include <QSound>
#include <QDir>
#include <QMessageBox>
#include <QtGui/QApplication>

#include "setsound.h"
#include "modulevar.h"
#include "moduleconst.h"
#include "ui_setsound.h"
#include "taskconfig/taskconfig.h"


//-------------------------------------------------------------------------------------------------

SetSoundForm::SetSoundForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetSoundForm)

{
    ui->setupUi(this);

    // Свяжем сигналы-слоты
    connect (ui->buttonBox, SIGNAL(rejected()), this, SLOT (CloseWindow()));      // Кнопка 'Закрыть'
    connect (ui->buttonBox, SIGNAL(accepted()), this, SLOT (SaveSettings()));     // Кнопка 'Сохранить'

    connect (ui->chBoxSoundsOnOff, SIGNAL(clicked()), SLOT (SetSoundsOnOff()));          // Чек бокс вкл/выкл звуков
    connect (ui->toolBtnSound, SIGNAL(clicked()), SLOT (PlayAvarSound()));              // Тулботн для прослушивания звука
    connect (ui->toolBtnSoundDialog, SIGNAL(clicked()), SLOT (SelectAvarSound()));     // Тулботн вызова диалогового окна звука
    connect (ui->SliderSound, SIGNAL(sliderMoved(int)),SLOT (ChangeSoundFrequency(int)));  // Слайдер изменения частоты повторов
    connect (ui->SliderSound, SIGNAL(valueChanged(int)),SLOT (ChangeSoundFrequency(int)));  // Слайдер изменения частоты повторов
    connect (ui->comboBoxTypeTS, SIGNAL(currentIndexChanged(int)), SLOT(ChangeTypeTS(int)));
    connect (ui->checkBoxPred, SIGNAL(clicked()), SLOT(SaveSoundChange()));
    connect (ui->checkBoxSpusk, SIGNAL(clicked()), SLOT(SaveSoundChange()));
    connect (ui->checkBoxPosle, SIGNAL(clicked()), SLOT(SaveSoundChange()));


    // Грузим установки формы из конфигурационного файла
    qTC->formLoad(this, FormSetFile, false);


    // Заполним вспомогательный временный массив настроек звуков
    int i,j = 0;
    for (i=0;i<3;i++)
      for (j=0;j<3;j++)
        tmpAllSoundOn[i][j] = getAllSoundOn(i,j);



    // Зададим начальное значение звука "Аварийное состояние"
    QSound avarSound(getLastSoundPath());
    // Если настройка "Звуки включены" - выставим ее
    if (getSoundOn())
    {
        ui->chBoxSoundsOnOff->setChecked(true);
    }
    else
    {
        ui->chBoxSoundsOnOff->setChecked(false);
    }

    // Выставим значение повторов звука
    switch (getNumSoundRepeat())
    {
    case 1:
        ui->SliderSound->setValue(1);
        ui->slideLabel->setText("одноразовый сигнал");
    break;
    case 3:
        ui->SliderSound->setValue(2);
    break;
    case 10:
        ui->SliderSound->setValue(3);
    break;
    case 50:
        ui->SliderSound->setValue(4);
    break;
    case -1:
        ui->SliderSound->setValue(5);
    break;
    }

    SetSoundsOnOff();

}

//-------------------------------------------------------------------------------------------------

SetSoundForm::~SetSoundForm()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,false);
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void SetSoundForm::changeEvent(QEvent *e)
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
void SetSoundForm::CloseWindow()
{
    this->close();
}

//-------------------------------------------------------------------------------------------------

void SetSoundForm::SaveSettings()
{

    if (ui->chBoxSoundsOnOff->isChecked())
    {
         setSoundOn(true);
    }
    else
    {
         setSoundOn(false);
    }

    int m,n=0;
    for (m=0;m<3;m++)
      for (n=0;n<3;n++)
        setAllSoundOn(m,n,tmpAllSoundOn[m][n]);

    this->close();
}

//-------------------------------------------------------------------------------------------------

void SetSoundForm::SetSoundsOnOff()
{

   if (ui->chBoxSoundsOnOff->isChecked())
    {
        ui->toolBtnSound->setEnabled(true);
        ui->toolBtnSoundDialog->setEnabled(true);
        ui->label_11->setEnabled(true);
        ui->slideLabel->setEnabled(true);
        ui->SliderSound->setEnabled(true);
        ui->label_14->setEnabled(true);
        ui->frame_SoundTS->setEnabled(true);

        // Заполним Комбобох типами ТС
        ui->comboBoxTypeTS->clear();
        ui->comboBoxTypeTS->addItem(tsSGA.toLatin1());
        ui->comboBoxTypeTS->addItem(tsGVS.toLatin1());
        ui->comboBoxTypeTS->addItem(tsNPA.toLatin1());
        ui->comboBoxTypeTS->setCurrentIndex(0);


    }
    else
    {
        ui->toolBtnSound->setEnabled(false);
        ui->toolBtnSoundDialog->setEnabled(false);
        ui->label_11->setEnabled(false);
        ui->toolBtnSound->setEnabled(false);
        ui->toolBtnSoundDialog->setEnabled(false);
        ui->label_11->setEnabled(false);
        ui->slideLabel->setEnabled(false);
        ui->SliderSound->setEnabled(false);
        ui->label_14->setEnabled(false);
        ui->frame_SoundTS->setEnabled(false);
        ui->checkBoxPred->setChecked(false);
        ui->checkBoxSpusk->setChecked(false);
        ui->checkBoxPosle->setChecked(false);
        ui->comboBoxTypeTS->clear();

    }
}

//-------------------------------------------------------------------------------------------------

void SetSoundForm::PlayAvarSound()
{

//    QSound avarSound(getAvarSoundPath());
    QSound avarSound(getLastSoundPath());
    if (!avarSound.isAvailable())
    {
        QMessageBox::critical (0, QObject::tr("Ошибка воспроизведения"), msgErrorPlayingSound.toLatin1(),QMessageBox::Ok);
        return;
    }
    avarSound.play();
  }

//-------------------------------------------------------------------------------------------------

void SetSoundForm::SelectAvarSound()
{
    QString newAvarSoundFile = QFileDialog::getOpenFileName(this,tr("Выбрать звуковой файл"), getLastSoundPath(), tr("Звуковые файлы (*.wav)"));
    setLastSoundPath(newAvarSoundFile);

}

//-------------------------------------------------------------------------------------------------

void SetSoundForm::ChangeSoundFrequency(int slaiderposition)
{

    switch (slaiderposition)
    {
    case 1:
        ui->slideLabel->setText("одноразовый сигнал");
        setNumSoundRepeat(1);
    break;
    case 2:
        ui->slideLabel->setText("повторить 3 раза");
        setNumSoundRepeat(3);
    break;
    case 3:
        ui->slideLabel->setText("повторить 10 раз");
        setNumSoundRepeat(10);
    break;
    case 4:
        ui->slideLabel->setText("повторить 50 раз");
        setNumSoundRepeat(50);
    break;
    case 5:
        ui->slideLabel->setText("повторять бесконечно");
        setNumSoundRepeat(-1);
    break;
    }

}

//-------------------------------------------------------------------------------------------------

void SetSoundForm::ChangeTypeTS(int n)
{

/*    ui->checkBoxPred->setChecked(getAllSoundOn(n,0));
    ui->checkBoxSpusk->setChecked(getAllSoundOn(n,1));
    ui->checkBoxPosle->setChecked(getAllSoundOn(n,2));
*/

        ui->checkBoxPred->setChecked(tmpAllSoundOn[n][0]);
        ui->checkBoxSpusk->setChecked(tmpAllSoundOn[n][1]);
        ui->checkBoxPosle->setChecked(tmpAllSoundOn[n][2]);


}

//-------------------------------------------------------------------------------------------------

void SetSoundForm::SaveSoundChange()
{
    int n = ui->comboBoxTypeTS->currentIndex();

/*    setAllSoundOn(n,0,ui->checkBoxPred->isChecked());
    setAllSoundOn(n,1,ui->checkBoxSpusk->isChecked());
    setAllSoundOn(n,2,ui->checkBoxPosle->isChecked());
*/
    tmpAllSoundOn[n][0] = ui->checkBoxPred->isChecked();
    tmpAllSoundOn[n][1] = ui->checkBoxSpusk->isChecked();
    tmpAllSoundOn[n][2] = ui->checkBoxPosle->isChecked();
}

//-------------------------------------------------------------------------------------------------
