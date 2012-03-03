#ifndef SETCOLOR_H
#define SETCOLOR_H

#include <QDialog>
#include <QColor>
#include <QSound>
#include <QDir>

namespace Ui {
    class SetColorForm;
}

class SetColorForm : public QDialog {
    Q_OBJECT
public:
    SetColorForm(QWidget *parent = 0);
    ~SetColorForm();

    QColor newPohodColor;
    QColor newReadyColor;
    QColor newNotReadyColor;
    QColor newAvarColor;
    QColor newSelectColor;


protected:
    void changeEvent(QEvent *e);

private:
    Ui::SetColorForm *ui;
    // Путь к только что открытому в диалоге звуковому файлу
    QString LastSoundPath;


private slots:

    void CloseWindow();
    void SaveSettings();
    void SelectPohodColor();
    void SelectReadyColor();
    void SelectNotReadyColor();
    void SelectAvarColor();
    void SelectSelectColor();
    void SetStandardColors();


};

#endif // SETCOLOR_H
