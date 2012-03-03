#include "about.h"

About::About(QWidget *parent): QDialog(parent) {
    ui.setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint);
    this->setWindowTitle(tr("О программе"));
}

void About::setMessage(QString msgText, QString vers)
{
    ui.L_caption->setText(msgText+"\n\nВерсия "+vers+"\n\nЗАО НИИ ЦПС\nг. Тверь\n"+QString(__DATE__).right(4));

}

void About::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        this->reject();
    }
    QWidget::keyPressEvent(event);
}

About::~About() {

}
