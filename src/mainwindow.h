#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./taskconfig/taskconfig.h"
#include <QKeyEvent>
#include <QShowEvent>
#include <QToolBar>
#include <QMainWindow>
#include <QKeyEvent>
#include <QShowEvent>
#include <QShortcut>
#include <QModelIndex>
#include "moduleproc.h"


namespace Ui {
    class MainWindow;
}

//-------------------------------------------------------------------------------------------------

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


protected:


private:
    Ui::MainWindow *ui;

    QAction *actNewRecord;
    QAction *actEditRecord;
    QAction *actDelRecord;
    QAction *actNSIAction;
    QAction *actSetColorAction;
    QAction *actSetSoundAction;
    QAction *actHelpAction;
    QAction *actExitAction;
    QAction *actFindAction;
    SoundThread thread;

    QModelIndex currentMdIndex;

    QSqlQueryModel *ModelSpisPogruzh;
    QSqlQuery *QuerySpisPogruzh;
    QSortFilterProxyModel *ProxyModelSpisPogruzh;

    QSqlQueryModel *ModelSostTS;
    QSqlQuery *QuerySostTS;
    QSortFilterProxyModel *ProxyModelSpisPlaceTS;



private slots:

    void keyPressEvent(QKeyEvent *event);
    void CreateWinAbout();
    void CloseWindowMain();
    //void Create_p_sgapredForm();
    void CreateEditTemplate();
    void CreateEditTemplate5();
    void CreateEditTemplate6();
    void CreateEditTemplate7();
    void CreateEditTemplate8();
    void CreateEditTemplate9();
    void CreateEditTemplate10();
    void CreateEditTemplate11();
    void CreateEditTemplate12();
    void CreateEditTemplate13();
    void CreateEditTemplate14();
    void CreateEditTemplate15();
    void CreateClrWindow();
    void CreateSndWindow();
    void SelectSGA();
    void SelectGVS();
    void SelectNPA();
    void ChangeCurrentPage(int index);
    void HelpAction();
    void FindAction();
    void ShortCutFindAction();
    void SelectActiveComponent(QModelIndex index);
    void FormChoiceAdd();
    void FormChoiceEdit();
    void DeleteRecord();
    void ChangingTables();
    void SetCurrentMode();


};

#endif // MAINWINDOW_H
