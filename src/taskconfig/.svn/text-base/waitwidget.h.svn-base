#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include <QWidget>
#include <QMovie>
#include <QThread>
#include <QTimer>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QPaintEngine>

namespace Ui {
    class WaitWidget;
}

class WaitThread : public QThread {
    Q_OBJECT
public:
    void run();
signals:
    void repaint();
private slots:
    void timeout();
    void stop();
private:
    QTimer *timer;
};

class WaitWidget : public QWidget {
    Q_OBJECT
public:
    static void start(QWidget *parent = 0, bool needRepaint = false, QString text = QString());
    static void setText(QString text = QString());
    static void finish();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *e);

private slots:
    void sRepaint();

private:
    static WaitWidget *self;
    static bool m_repainting;

    WaitWidget(QWidget *parent, bool needRepaint);
    ~WaitWidget();
    void setLabelText(QString text);
    void correctPos();

    bool needRepaint;
    QMovie *movie;
    WaitThread *thread;
    Ui::WaitWidget *ui;
};

#endif // WAITWIDGET_H
