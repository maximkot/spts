#ifndef USRLIST_H
#define USRLIST_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QtCore>
#include <QItemDelegate>
#include <QTableView>
#include <QScrollBar>
#include "tsziinifile.h"


namespace Ui {
    class UsrList;
}

class UsrModel : public QSqlQueryModel {
    Q_OBJECT

public:
    UsrModel(QWidget *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant data(const QModelIndex &item, int role) const;
private:
    QString _checkableField;
    QTableView* _parent;
};
//Класс модели с CheckBox'сами
class UsrList : public QWidget
{
    Q_OBJECT

public:
    explicit UsrList(QWidget *parent = 0);
    ~UsrList();

    static UsrList* instance();//Возвращает указатель на виджет
    static void usrDelete();//Удаление виджета

private:
    UsrModel *model;
    void fillList();
    Ui::UsrList *ui;
    void usrListUp();
private slots:
    void on_rbAll_clicked();
    void on_rbSel_clicked();
};

#endif // USRLIST_H
