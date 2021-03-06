#ifndef P_SGAPRED_H
#define P_SGAPRED_H

#include <QDialog>



namespace Ui {
    class p_sgapredForm;
}

class p_sgapredForm : public QDialog {
    Q_OBJECT
public:

  // Форма одинаковая как для Добавления/Редактирования ПРЕД/ПОСЛЕпогружных проверок
 // Признак bool AddEdit = true добавление, AddEdit = false редактирование
// Признак bool BeforeAfter = true ПРЕД, BeforeAfter = false ПОСЛЕ
p_sgapredForm(bool AddEdit, bool BeforeAfter, int recnum, QWidget *parent=0);
~p_sgapredForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::p_sgapredForm *ui;

private slots:
    void CloseWindow();
    void SaveRecord();

};

#endif // P_SGAPRED_H
