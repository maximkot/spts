#ifndef SETTINGSTABLE_H
#define SETTINGSTABLE_H

#include <QtGui/QDialog>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QTableView>
#include <QItemDelegate>

namespace Ui {
    class fSettingsTable;
}

//-----------------------------------------------
class SettingsTableModel : public QStandardItemModel {
    Q_OBJECT

public:
    SettingsTableModel(int rows, int columns, QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

//-----------------------------------------------
class MyDelegate : public QItemDelegate {
    Q_OBJECT

public:
    MyDelegate(QObject *parent = 0);
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
protected:
    void drawFocus(QPainter *painter,
                   const QStyleOptionViewItem &option,
                   const QRect &rect) const;
};

//-----------------------------------------------
class SettingsTable : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SettingsTable)
public:
    explicit SettingsTable(QTableView *table, QWidget *parent = 0);
    virtual ~SettingsTable();

    static void Run(QTableView *table);

protected:
    virtual void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::fSettingsTable *m_ui;
    SettingsTableModel *itemModel;
    QTableView *table;

    void toFillTable();
    void applySettings();

private slots:
    void on_btnSave_clicked();
};

#endif // SETTINGSTABLE_H
