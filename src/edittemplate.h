#ifndef EDITTEMPLATE_H
#define EDITTEMPLATE_H

#include <QDialog>
#include <QToolBar>
#include <QKeyEvent>
#include <QShowEvent>
#include <QMainWindow>



namespace Ui {
    class EditTemplate;
}

class EditTemplate : public QDialog {
    Q_OBJECT
public:
    // Впишем в переменную нашего окна интовский признак, который укажет, какой шаблон открыть
    EditTemplate(int numTemplate, QWidget *parent = 0);
    ~EditTemplate();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditTemplate *ui;

    QAction *actNewRecord;
    QAction *actEditRecord;
    QAction *actDelRecord;
    QAction *actFindRecord;
    QAction *actDownRecord;
    QAction *actUpRecord;
    QAction *actExitAction;
    int aNumTemplate;


private slots:


void CloseWindow();
void CreateAddTemplateString();
void CreateEditTemplateString();
void DelRecord();
void FindRecord();
void UpRecord();
void DownRecord();
void HelpAction();

};

#endif // EDITTEMPLATE_H
