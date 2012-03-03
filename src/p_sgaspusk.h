#ifndef P_SGASPUSK_H
#define P_SGASPUSK_H

#include <QDialog>




namespace Ui {
    class p_sgaspuskForm;
}



class p_sgaspuskForm : public QDialog {
    Q_OBJECT
public:
    p_sgaspuskForm (bool AddEdit, int recnum, QWidget *parent = 0);
    ~p_sgaspuskForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::p_sgaspuskForm *ui;

private slots:
    void CloseWindow();
    void SaveRecord();

};

#endif // P_SGASPUSK_H
