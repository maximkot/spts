#ifndef P_NPASPUSK_H
#define P_NPASPUSK_H

#include <QDialog>

namespace Ui {
    class p_npaspuskForm;
}

class p_npaspuskForm : public QDialog {
    Q_OBJECT
public:
    p_npaspuskForm (bool AddEdit, int recnum, QWidget *parent = 0);
    ~p_npaspuskForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::p_npaspuskForm *ui;

private slots:
    void CloseWindow();
    void SaveRecord();

};

#endif // P_NPASPUSK_H
