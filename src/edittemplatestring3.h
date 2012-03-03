#ifndef EDITTEMPLATESTRING3_H
#define EDITTEMPLATESTRING3_H

#include <QDialog>

namespace Ui {
    class EditTemplateString3;
}

class EditTemplateString3 : public QDialog {
    Q_OBJECT
public:
    // Впишем в переменную нашего окна булевский признак, который укажет, что писать в заголовке окна
    EditTemplateString3(bool priz, QWidget *parent = 0);
    ~EditTemplateString3();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditTemplateString3 *ui;
    bool aPriz;

private slots:

void SaveRecord();
void CloseWindow();


};

#endif // EDITTEMPLATESTRING3_H
