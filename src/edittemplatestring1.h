#ifndef EDITTEMPLATESTRING1_H
#define EDITTEMPLATESTRING1_H

#include <QDialog>

namespace Ui {
    class EditTemplateString1;
}

class EditTemplateString1 : public QDialog {
    Q_OBJECT
public:
    // Впишем в переменную нашего окна булевский признак, который укажет, что писать в заголовке окна
    EditTemplateString1(bool priz, QWidget *parent = 0);
    ~EditTemplateString1();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditTemplateString1 *ui;
    bool aPriz;

private slots:

void SaveRecord();
void CloseWindow();
void SetItemsVisible();

};

#endif // EDITTEMPLATESTRING1_H
