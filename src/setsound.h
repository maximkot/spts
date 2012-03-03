#ifndef SETSOUND_H
#define SETSOUND_H

#include <QDialog>
#include <QColor>
#include <QSound>
#include <QDir>

namespace Ui {
    class SetSoundForm;
}

class SetSoundForm : public QDialog {
    Q_OBJECT
public:
    SetSoundForm(QWidget *parent = 0);
    ~SetSoundForm();

     // массив хранит изначальную настройку параметров звука
    // на тот случай, если сделаем изменения на форме и при выходе откажемся от них
    bool tmpAllSoundOn[3][3];


protected:
    void changeEvent(QEvent *e);

private:
    Ui::SetSoundForm *ui;
    // Путь к только что открытому в диалоге звуковому файлу
    QString LastSoundPath;


private slots:

    void CloseWindow();
    void SaveSettings();
    void SetSoundsOnOff();
    void PlayAvarSound();
    void SelectAvarSound();
    void ChangeSoundFrequency(int slaiderposition);
    void ChangeTypeTS(int n);
    void SaveSoundChange();


};

#endif // SETSOUND_H
