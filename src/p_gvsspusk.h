#ifndef P_GVSSPUSK_H
#define P_GVSSPUSK_H

#include <QDialog>

namespace Ui {
    class p_gvsspuskForm;
}

class p_gvsspuskForm : public QDialog {
    Q_OBJECT
public:
    p_gvsspuskForm(bool AddEdit, int recnum, QWidget *parent = 0);
    ~p_gvsspuskForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::p_gvsspuskForm *ui;

private slots:
    void CloseWindow();
    void SaveRecord();

};

#endif // P_GVSSPUSK_H
