#ifndef P_GVSPRED_H
#define P_GVSPRED_H

#include <QDialog>

namespace Ui {
    class p_gvspredForm;
}

class p_gvspredForm : public QDialog {
    Q_OBJECT
public:
    // Форма одинаковая как для Добавления/Редактирования ПРЕД/ПОСЛЕпогружных проверок
   // Признак bool AddEdit = true добавление, AddEdit = false редактирование
  // Признак bool BeforeAfter = true ПРЕД, BeforeAfter = false ПОСЛЕ
  p_gvspredForm(bool AddEdit, bool BeforeAfter, int recnum, QWidget *parent=0);
 ~p_gvspredForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::p_gvspredForm *ui;

private slots:
    void CloseWindow();
    void SaveRecord();

};

#endif // P_GVSPRED_H
