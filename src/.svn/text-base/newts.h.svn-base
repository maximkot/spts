#ifndef NEWTS_H
#define NEWTS_H

#include <QDialog>

//---------------------------------------------------------------------------------------------

namespace Ui {
    class NewTS;
}



class NewTS : public QDialog {
    Q_OBJECT
public:
    NewTS(QWidget *parent = 0);
    ~NewTS();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::NewTS *ui;

private slots:
    void SaveRecord();
    void CloseWindow();
    void OnShipChange(QString currentComboText);
    void OnPlaceChange(QString currentComboText);
    void OnSostChange(QString currentComboText);

};

#endif // NEWTS_H
