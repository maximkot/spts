#include "waitwidget.h"
#include "ui_waitwidget.h"
#include <QDebug>

WaitWidget* WaitWidget::self = 0;
bool WaitWidget::m_repainting = false;

void WaitThread::run() {
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()), Qt::DirectConnection);
    connect(this, SIGNAL(finished()), this, SLOT(stop()), Qt::DirectConnection);
    timer->start(50);
    exec();
}

void WaitThread::stop() {
    timer->stop();
    delete timer;
    while (this->isRunning()) {}
    delete this;
}

void WaitThread::timeout() {
    emit repaint();
}

// -----------------------------
void WaitWidget::start(QWidget *parent, bool needRepaint, QString text) {
    if (!self) self = new WaitWidget(parent, needRepaint);
    setText(text);
    self->show();
    self->sRepaint();
}

void WaitWidget::setText(QString text) {
    if (!self) start(0);
    self->setLabelText(text);
}

void WaitWidget::finish() {
    if (self) {
        delete self;
        self = 0;
    }
}

WaitWidget::WaitWidget(QWidget *parent, bool needRepaint): QWidget(parent), ui(new Ui::WaitWidget) {
    ui->setupUi(this);
    setWindowFlags(Qt::Popup);
    this->needRepaint = needRepaint;
    movie = new QMovie(":/wait/icons/wait.gif");
    ui->movie->setMovie(movie);
    thread = new WaitThread();
    connect(thread, SIGNAL(repaint()), SLOT(sRepaint()), Qt::DirectConnection);
    thread->start();
    correctPos();
}

void WaitWidget::setLabelText(QString text) {
    if (text.isNull()) return;
    ui->label->setText(text);
    correctPos();
}

void WaitWidget::correctPos() {
    sRepaint();
    QWidget *widget = this->parentWidget() ? this->parentWidget() : qApp->desktop()->screen();
    QRect rect = widget->geometry();
    this->move(rect.width() / 2 - this->width() / 2 + rect.x(),
               rect.height() / 2 - this->height() / 2 + rect.y());
}

void WaitWidget::sRepaint() {
    if (m_repainting) return;
    m_repainting = true;
    movie->jumpToNextFrame();
    if (needRepaint) {
        this->repaint();
    }
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    m_repainting = false;
}

void WaitWidget::showEvent(QShowEvent *) {
    correctPos();
}

void WaitWidget::closeEvent(QCloseEvent *e) {
    e->ignore();
}

WaitWidget::~WaitWidget() {
    thread->exit(0);
    delete movie;
    delete ui;
}
