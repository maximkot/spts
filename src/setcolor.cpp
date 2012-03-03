#include <QColorDialog>
#include <QFileDialog>
#include <QColor>
//#include <QSound>
#include <QDir>
#include <QMessageBox>
#include <QtGui/QApplication>

#include "setcolor.h"
#include "modulevar.h"
#include "moduleconst.h"
#include "ui_setcolor.h"
#include "taskconfig/taskconfig.h"


//-------------------------------------------------------------------------------------------------

SetColorForm::SetColorForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetColorForm)

{
    ui->setupUi(this);

    // Свяжем сигналы-слоты
    connect (ui->buttonBox, SIGNAL(rejected()), this, SLOT (CloseWindow()));      // Кнопка 'Закрыть'
    connect (ui->buttonBox, SIGNAL(accepted()), this, SLOT (SaveSettings()));     // Кнопка 'Сохранить'

    connect (ui->toolBtnPohod, SIGNAL(clicked()), this, SLOT (SelectPohodColor()));       // Кнопка выбора цвета 'По-походному'
    connect (ui->toolBtnReady, SIGNAL(clicked()), this, SLOT (SelectReadyColor()));       // Кнопка выбора цвета 'Готов'
    connect (ui->toolBtnNotReady, SIGNAL(clicked()), this, SLOT (SelectNotReadyColor())); // Кнопка выбора 'Не Готов'
    connect (ui->toolBtnAvar, SIGNAL(clicked()), this, SLOT (SelectAvarColor()));         // Кнопка выбора цвета 'Аварийное состояние'
    connect (ui->toolBtnSelect, SIGNAL(clicked()), this, SLOT (SelectSelectColor()));     // Кнопка выбора цвета 'НеАктивный компонент'
    connect (ui->chBoxStandartColors, SIGNAL(clicked()), SLOT (SetStandardColors()));     // Чек бокс выбора стандартной цветовой схемы

    // Грузим установки формы из конфигурационного файла
    qTC->formLoad(this, FormSetFile, false);

    // зарисуем поля текущими значениями цветов
    ui->label_5->setPalette(QPalette(getPohodColor()));
    ui->label_7->setPalette(QPalette(getReadyColor()));
    ui->label_8->setPalette(QPalette(getNotReadyColor()));
    ui->label_9->setPalette(QPalette(getAvarColor()));
    ui->label_13->setPalette(QPalette(getSelectColor()));

     //Зададим начальное значение нового цвета равное сохраненному, чтобы отследить изменения,
    // если юзер начнет тыкать кнопки и менять цвета
    newPohodColor = getPohodColor();
    newReadyColor = getReadyColor();
    newNotReadyColor = getNotReadyColor();
    newAvarColor = getAvarColor();
    newSelectColor = getSelectColor();

    // Теперь посмотрим: если стандартная цветовая схема, то ставим галку в чекбохе
    if ((getPohodColor() == defaultPohodColor)&&(getReadyColor() == defaultReadyColor)&&(getNotReadyColor() == defaultNotReadyColor)&&(getAvarColor() == defaultAvarColor)&&(getSelectColor() == defaultSelectColor))
    {
        ui->chBoxStandartColors->setChecked(true);
    }
    else
    {
        ui->chBoxStandartColors->setChecked(false);
    }

}

//-------------------------------------------------------------------------------------------------

SetColorForm::~SetColorForm()
{
    // Сохраняем установки формы
    qTC->formSave(this,FormSetFile,false);
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void SetColorForm::changeEvent(QEvent *e)
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
void SetColorForm::CloseWindow()
{
    this->close();
}

//-------------------------------------------------------------------------------------------------

void SetColorForm::SaveSettings()
{

    setPohodColor(newPohodColor);
    setReadyColor(newReadyColor);
    setNotReadyColor(newNotReadyColor);
    setAvarColor(newAvarColor);
    setSelectColor(newSelectColor);

    this->close();
}

//-------------------------------------------------------------------------------------------------

void SetColorForm::SelectPohodColor()
{    
     // сохраним цвет на случай нажатия Cancel в диалоге цвета
    // т.к. в этом случае выдается кривое значение цвета
    QColor oldColor = newPohodColor;

    newPohodColor = QColorDialog::getColor(newPohodColor, this, tr("Выбор цвета 'По-походному'"));
    if (newPohodColor.isValid())
    {
        {ui->label_5->setPalette(QPalette(newPohodColor));}
    }
    else
    {   // если нажат Cancel - возвращаем старый цвет
        newPohodColor = oldColor;
    }

     // Проверим, стоит ли теперь стандартная цветовая схема
    // Если да, то ставим галку в чекбохе, если нет - снимаем
    if ((newPohodColor == defaultPohodColor)&&(newReadyColor == defaultReadyColor)&&(newNotReadyColor == defaultNotReadyColor)&&(newAvarColor == defaultAvarColor)&&(newSelectColor == defaultSelectColor))
    {
        ui->chBoxStandartColors->setChecked(true);
    }
    else
    {
        ui->chBoxStandartColors->setChecked(false);
    }

}

//-------------------------------------------------------------------------------------------------
void SetColorForm::SelectReadyColor()
{
     // сохраним цвет на случай нажатия Cancel в диалоге цвета
    // т.к. в этом случае выдается кривое значение цвета
    QColor oldColor = newReadyColor;

    newReadyColor = QColorDialog::getColor(newReadyColor, this, tr("Выбор цвета 'Готов к работе'"));

    if (newReadyColor.isValid())
    {
        ui->label_7->setPalette(QPalette(newReadyColor));
    }
    else
    {   // если нажат Cancel - возвращаем старый цвет
        newReadyColor = oldColor;
    }

     // Проверим, стоит ли теперь стандартная цветовая схема
    // Если да, то ставим галку в чекбохе, если нет - снимаем
    if ((newPohodColor == defaultPohodColor)&&(newReadyColor == defaultReadyColor)&&(newNotReadyColor == defaultNotReadyColor)&&(newAvarColor == defaultAvarColor)&&(newSelectColor == defaultSelectColor))
    {
        ui->chBoxStandartColors->setChecked(true);
    }
    else
    {
        ui->chBoxStandartColors->setChecked(false);
    }

}

//-------------------------------------------------------------------------------------------------
void SetColorForm::SelectNotReadyColor()
{

     // сохраним цвет на случай нажатия Cancel в диалоге цвета,
    // т.к. в этом случае выдается кривое значение цвета
    QColor oldColor = newNotReadyColor;

    newNotReadyColor = QColorDialog::getColor(newNotReadyColor, this, tr("Выбор цвета 'Не готов к работе'"));
    if (newNotReadyColor.isValid())
    {
        ui->label_8->setPalette(QPalette(newNotReadyColor));
    }
    else
    {   // если нажат Cancel - возвращаем старый цвет
        newNotReadyColor = oldColor;
    }

     // Проверим, стоит ли теперь стандартная цветовая схема
    // Если да, то ставим галку в чекбохе, если нет - снимаем
    if ((newPohodColor == defaultPohodColor)&&(newReadyColor == defaultReadyColor)&&(newNotReadyColor == defaultNotReadyColor)&&(newAvarColor == defaultAvarColor)&&(newSelectColor == defaultSelectColor))
    {
        ui->chBoxStandartColors->setChecked(true);
    }
    else
    {
        ui->chBoxStandartColors->setChecked(false);
    }

}

//-------------------------------------------------------------------------------------------------
void SetColorForm::SelectAvarColor()
{
     // сохраним цвет на случай нажатия Cancel в диалоге цвета,
    // т.к. в этом случае выдается кривое значение цвета
    QColor oldColor = newAvarColor;

    newAvarColor = QColorDialog::getColor(newAvarColor, this, tr("Выбор цвета 'Аварийное состояние'"));
    if (newAvarColor.isValid())
    {
        ui->label_9->setPalette(QPalette(newAvarColor));
    }
    else
    {   // если нажат Cancel - возвращаем старый цвет
        newAvarColor = oldColor;
    }

     // Проверим, стоит ли теперь стандартная цветовая схема
    // Если да, то ставим галку в чекбохе, если нет - снимаем
    if ((newPohodColor == defaultPohodColor)&&(newReadyColor == defaultReadyColor)&&(newNotReadyColor == defaultNotReadyColor)&&(newAvarColor == defaultAvarColor)&&(newSelectColor == defaultSelectColor))
    {
        ui->chBoxStandartColors->setChecked(true);
    }
    else
    {
        ui->chBoxStandartColors->setChecked(false);
    }


}

//-------------------------------------------------------------------------------------------------

void SetColorForm::SelectSelectColor()
{
     // сохраним цвет на случай нажатия Cancel в диалоге цвета,
    // т.к. в этом случае выдается кривое значение цвета
    QColor oldColor = newSelectColor;

    newSelectColor = QColorDialog::getColor(newSelectColor, this, tr("Выбор цвета 'Выделение неактивного компонента'"));
    if (newSelectColor.isValid())
    {
        ui->label_13->setPalette(QPalette(newSelectColor));
    }
    else
    {   // если нажат Cancel - возвращаем старый цвет
        newSelectColor = oldColor;
    }

     // Проверим, стоит ли теперь стандартная цветовая схема
    // Если да, то ставим галку в чекбохе, если нет - снимаем
    if ((newPohodColor == defaultPohodColor)&&(newReadyColor == defaultReadyColor)&&(newNotReadyColor == defaultNotReadyColor)&&(newAvarColor == defaultAvarColor)&&(newSelectColor == defaultSelectColor))
    {
        ui->chBoxStandartColors->setChecked(true);
    }
    else
    {
        ui->chBoxStandartColors->setChecked(false);
    }


}

//-------------------------------------------------------------------------------------------------

void SetColorForm::SetStandardColors()
{
     // Если чекбох нажат, то рисуем стандартные цвета,
    // ежели отжат, то те, которые были при открытии формы
    if (ui->chBoxStandartColors->isChecked())
    {
        // присвоим цвета по умолчанию соответственно и лейблам и переменным новых цветов
        ui->label_5->setPalette(QPalette(defaultPohodColor));
        newPohodColor = defaultPohodColor;
        ui->label_7->setPalette(QPalette(defaultReadyColor));
        newReadyColor = defaultReadyColor;
        ui->label_8->setPalette(QPalette(defaultNotReadyColor));
        newNotReadyColor = defaultNotReadyColor;
        ui->label_9->setPalette(QPalette(defaultAvarColor));
        newAvarColor = defaultAvarColor;
        ui->label_13->setPalette(QPalette(defaultSelectColor));
        newSelectColor = defaultSelectColor;

    }
    else
    {
        // присвоим цвета загруженные из cfg соответственно и лейблам и переменным новых цветов
        ui->label_5->setPalette(QPalette(getPohodColor()));
        newPohodColor = getPohodColor();
        ui->label_7->setPalette(QPalette(getReadyColor()));
        newReadyColor = getReadyColor();
        ui->label_8->setPalette(QPalette(getNotReadyColor()));
        newNotReadyColor = getNotReadyColor();
        ui->label_9->setPalette(QPalette(getAvarColor()));
        newAvarColor = getAvarColor();
        ui->label_13->setPalette(QPalette(getSelectColor()));
        newSelectColor = getSelectColor();
    }
}

//-------------------------------------------------------------------------------------------------
