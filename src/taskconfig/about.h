#ifndef F_ABOUT_H
#define F_ABOUT_H

#include <QtGui/QDialog>
#include "ui_about.h"
#include <QKeyEvent>

class About : public QDialog {
    Q_OBJECT

public:
    About(QWidget *parent = 0);
    void setMessage(QString msgText, QString vers);
    ~About();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
    Ui::AboutClass ui;
};

#endif // F_ABOUT_H
