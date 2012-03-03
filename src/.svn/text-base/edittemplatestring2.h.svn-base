#ifndef EDITTEMPLATESTRING2_H
#define EDITTEMPLATESTRING2_H

#include <QDialog>

namespace Ui {
    class EditTemplateString2;
}

class EditTemplateString2 : public QDialog {
    Q_OBJECT
public:
    // Впишем в переменную нашего окна булевский признак, который укажет, что писать в заголовке окна
    EditTemplateString2(bool priz, QWidget *parent = 0);
    ~EditTemplateString2();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditTemplateString2 *ui;
    bool aPriz;

private slots:

void SaveRecord();
void CloseWindow();


};

#endif // EDITTEMPLATESTRING2_H
