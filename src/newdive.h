#ifndef NEWDIVE_H
#define NEWDIVE_H

#include <QDialog>

namespace Ui {
    class NewDiveWindow;
}

class NewDiveWindow : public QDialog {
    Q_OBJECT
public:
    NewDiveWindow(QWidget *parent = 0);
    ~NewDiveWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::NewDiveWindow *ui;

private slots:
    void CloseWindow();
    void SaveRecord();

};

#endif // NEWDIVE_H
