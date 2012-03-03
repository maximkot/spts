#ifndef P_NPAPRED_H
#define P_NPAPRED_H

#include <QDialog>

namespace Ui {
    class p_npapredForm;
}

class p_npapredForm : public QDialog {
    Q_OBJECT
public:

    // Форма одинаковая как для Добавления/Редактирования ПРЕД/ПОСЛЕпогружных проверок
   // Признак bool AddEdit = true добавление, AddEdit = false редактирование
  // Признак bool BeforeAfter = true ПРЕД, BeforeAfter = false ПОСЛЕ
  p_npapredForm(bool AddEdit, bool BeforeAfter, int recnum, QWidget *parent=0);
  ~p_npapredForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::p_npapredForm *ui;

private slots:
    void CloseWindow();
    void SaveRecord();

};

#endif // P_NPAPRED_H
