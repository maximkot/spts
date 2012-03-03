#include <QTextEdit>
#include <QKeyEvent>
#include <QHeaderView>
#include <QSqlRecord>
#include "settingstable.h"
#include "ui_settingstable.h"

//-----------------------------------------------
SettingsTableModel::SettingsTableModel(int rows, int columns, QObject *parent)
      : QStandardItemModel(rows, columns, parent)
{
}

Qt::ItemFlags SettingsTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled;
    if(index.column() == 0)
        flags |= Qt::ItemIsUserCheckable;
    else
        flags |= Qt::ItemIsEditable | Qt::ItemIsSelectable;

    return flags;
}

QVariant SettingsTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QStandardItemModel::data(index, Qt::DisplayRole);
    switch(role) {

    case Qt::TextAlignmentRole: {
        if(index.column() == 0)
            return (int)Qt::AlignCenter;
        if(index.column() == 1)
            return (int)Qt::AlignLeft | Qt::AlignVCenter;
        if(index.column() == 2)
			return (int)Qt::AlignCenter;
    }
    }

    return QStandardItemModel::data(index, role);
}

//-----------------------------------------------
MyDelegate::MyDelegate(QObject *parent)
        : QItemDelegate(parent)
{
}

void MyDelegate::paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    if(index.column() != 0)
        QItemDelegate::drawFocus(painter, option, option.rect.adjusted(0, 0, 0, 0));
}

QWidget *MyDelegate::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    if(index.column() == 1) {
        QTextEdit *editor = new QTextEdit(parent);

        editor->setLineWidth(1);
        editor->setMidLineWidth(1);
        editor->setFrameShape(QFrame::Box);
        editor->setFrameShadow(QFrame::Plain);
//        editor->document()->setDocumentMargin(2);
        editor->installEventFilter(const_cast<MyDelegate*>(this));
        return editor;
    } else
        return QItemDelegate::createEditor(parent, option, index);
}

void MyDelegate::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{
    if(index.column() == 1) {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QTextEdit *pte = static_cast<QTextEdit*>(editor);
        pte->setPlainText(value);
    } else
        QItemDelegate::setEditorData(editor, index);
}

void MyDelegate::setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const
{
    if(index.column() == 1) {
        QTextEdit *pte = static_cast<QTextEdit*>(editor);
        model->setData(index, pte->toPlainText());
    } else
        QItemDelegate::setModelData(editor, model, index);
}

void MyDelegate::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

void MyDelegate::drawFocus(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QRect &rect) const
{

}

//-----------------------------------------------
void SettingsTable::Run(QTableView *table)
{
    QWidget *widget = table->parentWidget();
    while(!widget->isWindow())
        widget = widget->parentWidget();

    SettingsTable *form  = new SettingsTable(table, widget);
    form->exec();
}

SettingsTable::SettingsTable(QTableView *table, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::fSettingsTable),
    table(table)
{
    m_ui->setupUi(this);

    toFillTable();

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
}

SettingsTable::~SettingsTable()
{
    delete m_ui;
}

void SettingsTable::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsTable::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        this->reject();
        break;
    }
}

void SettingsTable::on_btnSave_clicked()
{
    applySettings();
}

void SettingsTable::toFillTable()
{
    // настройка заголовков таблицы
    itemModel = new SettingsTableModel(table->horizontalHeader()->count(), 3, this);
    itemModel->setHeaderData(0, Qt::Horizontal, tr(""));
    itemModel->setHeaderData(1, Qt::Horizontal, tr("Реквизит"));
    itemModel->setHeaderData(2, Qt::Horizontal, tr("Ширина"));

    // заполнение таблицы
    for(int i = 0; i < table->horizontalHeader()->count(); i++) {
        int logicalIndex = table->horizontalHeader()->logicalIndex(i);
        itemModel->setData(itemModel->index(i, 0),
                           table->horizontalHeader()->isSectionHidden(logicalIndex) ? Qt::Unchecked : Qt::Checked,
                           Qt::CheckStateRole);
        itemModel->setData(itemModel->index(i, 1),
                           table->model()->headerData(logicalIndex, Qt::Horizontal).toString());
        itemModel->setData(itemModel->index(i, 2),
                           table->horizontalHeader()->sectionSize(logicalIndex));
    }

    // настройка таблицы
    m_ui->tvSettings->setModel(itemModel);
    m_ui->tvSettings->setColumnHidden(2, true);
    m_ui->tvSettings->setColumnWidth(0, 27);
    m_ui->tvSettings->setColumnWidth(1, 150);
    m_ui->tvSettings->setColumnWidth(2, 50);
    m_ui->tvSettings->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    m_ui->tvSettings->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    m_ui->tvSettings->verticalHeader()->hide();
    m_ui->tvSettings->setItemDelegate(new MyDelegate(m_ui->tvSettings));

    // прячем столбцы 
    for(int i = 0; i < table->horizontalHeader()->count(); i++) {
        int logicalIndex = table->horizontalHeader()->logicalIndex(i);
		QSqlQueryModel *model = static_cast<QSqlQueryModel*>(table->model());
        if(table->model()->headerData(logicalIndex, Qt::Horizontal) == model->record().fieldName(logicalIndex))
            m_ui->tvSettings->verticalHeader()->setSectionHidden(i, true);
        else
            m_ui->tvSettings->verticalHeader()->setSectionHidden(i, false);
    }
}

void SettingsTable::applySettings()
{
    for(int i = 0; i < itemModel->rowCount(); i++) {
        int logicalIndex = table->horizontalHeader()->logicalIndex(i);
        if(itemModel->data(itemModel->index(i, 0), Qt::CheckStateRole) == Qt::Unchecked)
            table->setColumnHidden(logicalIndex, true);
        else
            table->setColumnHidden(logicalIndex, false);
        table->model()->setHeaderData(logicalIndex,
                                      Qt::Horizontal,
                                      itemModel->data(itemModel->index(i, 1)).toString());
        //table->setColumnWidth(logicalIndex, itemModel->data(itemModel->index(i, 2)).toInt());
    }
}
