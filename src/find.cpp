#include "find.h"

/* ************************* Методы класса FindPrivate ************************* */

QVector<Find*> Find::objectList;

class FindPrivate : public QObject
{
    Q_DECLARE_PUBLIC(Find)

public:
    FindPrivate(QObject *parent = 0);
    Find *q_ptr;

    QAbstractItemView *sourceView;                  // текущий источник поиска
    QVector<QAbstractItemView*> sourceViewList;     // список источников
    QWidget *parentWgt;

    QWidget *spacerWgt;
    QLineEdit *eText;
    QLabel *lFind;

    QToolButton *btnSettings;
    QToolButton *btnNext;
    QToolButton *btnPrev;

    QMenu *menu;

    QAction *aCaseSensitive;
    QAction *aWholeWord;
    QAction *aOneColumn;

    QString findText;
    Find::FindOptions findOptions;

    bool eventFilter(QObject *watched, QEvent *event);
    void findSourceView(QWidget *parent);

    QToolBar *createToolBar(QWidget *parent);
    void createWgts(QWidget *parent);
    QHBoxLayout *createLayout(QWidget *parent);

    void find(QModelIndex currentIndex, Find::FindCourse findCourse);
    static QModelIndex parentNext(const QModelIndex &currentIndex);
    static QModelIndex childNext(const QModelIndex &currentIndex);

    void _q_textChange(const QString &text);
    void _q_optionsChange(bool checked);
};

FindPrivate::FindPrivate(QObject *parent) :
        QObject(parent)
{
    sourceView = 0;
    findText = "";
    findOptions = Find::NoOptions;
}

bool FindPrivate::eventFilter(QObject *watched, QEvent *event)
{
    Q_Q(Find);

    if (watched == parentWgt && event->type() == QEvent::Show) {
        emit q->visibleChanged(true);
        return false;
    }

    if (watched == parentWgt && event->type() == QEvent::Hide) {
        emit q->visibleChanged(false);
        return false;
    }

    if (watched == eText && event->type() == QEvent::FocusIn) {
        lFind->hide();
        return false;
    }

    if (watched == eText && event->type() == QEvent::FocusOut) {
        if (!eText->hasFocus() && eText->text().isEmpty())
            lFind->show();
        return false;
    }

    if (watched != menu && watched != eText && watched != parentWgt && event->type() == QEvent::FocusIn) {
        // QTableView
        QTableView *tableView = dynamic_cast<QTableView*>(watched);
        if (tableView) {
            if (sourceViewList.size() <= 0) sourceView = tableView;
            else {
                sourceView = 0;
                foreach (QAbstractItemView *view, sourceViewList)
                    if (view == tableView) {
                        sourceView = tableView;
                        break;
                    }
            }
            return false;
        }

        // QTableWidget
        QTableWidget *tableWidget = dynamic_cast<QTableWidget*>(watched);
        if (tableWidget) {
            if (sourceViewList.size() <= 0) sourceView = tableWidget;
            else {
                sourceView = 0;
                foreach (QAbstractItemView *view, sourceViewList)
                    if (view == tableWidget) {
                        sourceView = tableWidget;
                        break;
                    }
            }
            return false;
        }

        // QTreeView
        QTreeView *treeView = dynamic_cast<QTreeView*>(watched);
        if (treeView) {
            if (sourceViewList.size() <= 0) sourceView = treeView;
            else {
                sourceView = 0;
                foreach (QAbstractItemView *view, sourceViewList)
                    if (view == treeView) {
                        sourceView = treeView;
                        break;
                    }
            }
            return false;
        }

        // QTreeWidget
        QTreeWidget *treeWidget = dynamic_cast<QTreeWidget*>(watched);
        if (treeWidget) {
            if (sourceViewList.size() <= 0) sourceView = treeWidget;
            else {
                sourceView = 0;
                foreach (QAbstractItemView *view, sourceViewList)
                    if (view == treeWidget) {
                        sourceView = treeWidget;
                        break;
                    }
            }
            return false;
        }

        return false;
    }

    if (watched == menu && event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        if (mouseEvent->button() != Qt::LeftButton)
            return false;

        QAction *action = menu->actionAt(mouseEvent->pos());
        if (action) action->setChecked(!action->isChecked());
        return true;
    }

    return QObject::eventFilter(watched, event);
}

void FindPrivate::findSourceView(QWidget *parent)
{
    QList<QTableView*> listTableView = parent->findChildren<QTableView*>();
    foreach (QTableView *table, listTableView)
        table->installEventFilter(this);

    QList<QTableWidget*> listTableWidget = parent->findChildren<QTableWidget*>();
    foreach (QTableWidget *table, listTableWidget)
        table->installEventFilter(this);

    QList<QTreeView*> listTreeView = parent->findChildren<QTreeView*>();
    foreach (QTreeView *tree, listTreeView)
        tree->installEventFilter(this);

    QList<QTreeWidget*> listTreeWidget = parent->findChildren<QTreeWidget*>();
    foreach (QTreeWidget *tree, listTreeWidget)
        tree->installEventFilter(this);
}

QToolBar *FindPrivate::createToolBar(QWidget *parent)
{
    QToolBar *toolBar = new QToolBar(tr("Поиск"), parent);
    toolBar->installEventFilter(this);
    toolBar->setObjectName(parent->objectName() + "_tbFind");
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolBar->setMovable(true);
    toolBar->setMinimumWidth(265);
    toolBar->setIconSize(QSize(16, 16));

    createWgts(toolBar);
    toolBar->addWidget(spacerWgt);
    toolBar->addWidget(btnSettings);
    toolBar->addWidget(eText);
    toolBar->addWidget(btnPrev);
    toolBar->addWidget(btnNext);

    toolBar->hide();
    parentWgt = toolBar;

    return toolBar;
}

void FindPrivate::createWgts(QWidget *parent)
{
    Q_Q(Find);

    spacerWgt = new QWidget(parent);
    spacerWgt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    eText = new QLineEdit(parent);
    connect(eText, SIGNAL(returnPressed()), q, SLOT(findFirst()));
    connect(eText, SIGNAL(textChanged(QString)), q, SLOT(_q_textChange(QString)));
    eText->installEventFilter(this);
    eText->setMinimumWidth(180);
    eText->setMaximumWidth(180);

    lFind = new QLabel(eText);
    lFind->setText(tr("Введите текст поиска"));
    lFind->setStyleSheet("QLabel { color: gray; }");
    QHBoxLayout *hlayout = new QHBoxLayout(eText);
    hlayout->setContentsMargins(5, 1, 1, 1);
    hlayout->setSpacing(0);
    hlayout->addWidget(lFind);
    eText->setLayout(hlayout);

    btnNext = new QToolButton(parent);
    connect(btnNext, SIGNAL(clicked()), q, SLOT(findForward()));
    btnNext->setIcon(QIcon(":/new/buttonpictures/src/icons/next.png"));
    btnNext->setObjectName("btnNext");
    btnNext->setEnabled(false);
    btnNext->setToolTip(tr("Вперед (F3)"));
    btnNext->setShortcut(QKeySequence(Qt::Key_F3));
    btnNext->setAutoRaise(true);
    btnNext->setMaximumSize(23, 22);
    btnNext->setMinimumSize(23, 22);

    btnPrev = new QToolButton(parent);
    connect(btnPrev, SIGNAL(clicked()), q, SLOT(findBachward()));
    btnPrev->setIcon(QIcon(":/new/buttonpictures/src/icons/prev.png"));
    btnPrev->setObjectName("btnPrev");
    btnPrev->setEnabled(false);
    btnPrev->setToolTip(tr("Назад (Shift + F3)"));
    btnPrev->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F3));
    btnPrev->setAutoRaise(true);
    btnPrev->setMaximumSize(23, 22);
    btnPrev->setMinimumSize(23, 22);

    btnSettings = new QToolButton(parent);
    btnSettings->setIcon(QIcon(":/new/buttonpictures/src/icons/proper.png"));
    btnSettings->setPopupMode(QToolButton::InstantPopup);
    btnSettings->setToolTip(tr("Параметры поиска"));
    btnSettings->setAutoRaise(true);
    btnSettings->setMaximumSize(23, 22);
    btnSettings->setMinimumSize(23, 22);

    menu = new QMenu(btnSettings);
    menu->installEventFilter(this);
    btnSettings->setMenu(menu);

    aCaseSensitive = menu->addAction(tr("Учитывать регистр"));
    connect(aCaseSensitive, SIGNAL(toggled(bool)), q, SLOT(_q_optionsChange(bool)));
    aCaseSensitive->setObjectName("caseSensitive");
    aCaseSensitive->setCheckable(true);

    aWholeWord = menu->addAction(tr("Только слово целиком"));
    connect(aWholeWord, SIGNAL(toggled(bool)), q, SLOT(_q_optionsChange(bool)));
    aWholeWord->setObjectName("wholeWord");
    aWholeWord->setCheckable(true);

    aOneColumn = menu->addAction(tr("Только по данному столбцу"));
    connect(aOneColumn, SIGNAL(toggled(bool)), q, SLOT(_q_optionsChange(bool)));
    aOneColumn->setObjectName("oneColumn");
    aOneColumn->setCheckable(true);
    aOneColumn->setChecked(true);
}

QHBoxLayout *FindPrivate::createLayout(QWidget *parent)
{
    createWgts(parent);
    QHBoxLayout *layout = new QHBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(spacerWgt);
    layout->addWidget(btnSettings);
    layout->addWidget(eText);
    layout->addWidget(btnPrev);
    layout->addWidget(btnNext);
//        layout->insertStretch(0, 1);

    return layout;
}

void FindPrivate::find(QModelIndex currentIndex, Find::FindCourse findCourse)
{
    Q_Q(Find);

    if (!sourceView) return;
    if (!currentIndex.isValid()) {
        if (findCourse == Find::FindFirst || findCourse == Find::FindLast)
            currentIndex = currentIndex.model()->index(0, currentIndex.column());
        if (findCourse == Find::FindForward || findCourse == Find::FindBachward)
            return;
    }

    currentIndex = q->findIndex(sourceView->model(), currentIndex, findText, findOptions, findCourse);
    if (QTableView *sourceTable = dynamic_cast<QTableView*>(sourceView)) {
        while (sourceTable->isColumnHidden(currentIndex.column())) {
            if (!(findOptions & Find::OneColumn) && findCourse == Find::FindFirst) findCourse = Find::FindForward;
            if (!(findOptions & Find::OneColumn) && findCourse == Find::FindLast) findCourse = Find::FindBachward;
            currentIndex = q->findIndex(sourceView->model(), currentIndex, findText, findOptions, findCourse);
        }
    }

    if (currentIndex == QModelIndex()) {
        QString msgTitle = tr("Найти");
        if (findCourse == Find::FindFirst || findCourse == Find::FindLast)
        {
            if   (sourceView->objectName()=="treeView")
            {
                 QMessageBox::information(sourceView, msgTitle,
                                          tr("Закончен просмотр дерева. Искомый элемент не найден."));
            }
            else
            {
                QMessageBox::information(sourceView, msgTitle,
                                         tr("Закончен просмотр таблицы. Искомый элемент не найден."));
            }
        }
        if (findCourse == Find::FindForward)
        {
            QString msgText;
            if  (sourceView->objectName()=="treeView")
            {
                 msgText = tr("Достигнут конец дерева. Продолжить поиск с начала?");
            }
            else
            {
                msgText = tr("Достигнут конец таблицы. Продолжить поиск с начала?");
            }
            QMessageBox msg(QMessageBox::Question, msgTitle, msgText,
                            QMessageBox::NoButton, sourceView);
            msg.addButton(tr("Да"), QMessageBox::YesRole);
            msg.addButton(tr("Нет"), QMessageBox::NoRole);
            if (msg.exec() == QMessageBox::AcceptRole)
            {
                find(sourceView->currentIndex(), Find::FindFirst);
            }
        }

        if (findCourse == Find::FindBachward)
        {
            QString msgText;
            if  (sourceView->objectName()=="treeView")
            {
                 msgText = tr("Достигнуто начало дерева. Продолжить поиск с конца?");
            }
            else
            {
                msgText = tr("Достигнуто начало таблицы. Продолжить поиск с конца?");
            }

            QMessageBox msg(QMessageBox::Question, msgTitle, msgText,
                            QMessageBox::NoButton, sourceView);
            msg.addButton(tr("Да"), QMessageBox::YesRole);
            msg.addButton(tr("Нет"), QMessageBox::NoRole);
            if (msg.exec() == QMessageBox::AcceptRole)
            {
                find(sourceView->currentIndex(), Find::FindLast);
            }
        }

        parentWgt->activateWindow();
        eText->setFocus();
    }

    if (currentIndex.isValid())
    {
        if (sourceView->objectName()=="treeView")
        {   QTreeView *source_ = dynamic_cast<QTreeView*>(sourceView);
            QModelIndex index=sourceView->currentIndex();
            while (index.isValid())
            {  source_->setExpanded(index, true);
               index=index.parent();
            }
            sourceView->setCurrentIndex(currentIndex);
            source_->setCurrentIndex(currentIndex);
        }
        else
        {
            sourceView->setCurrentIndex(currentIndex);
        }
    }
}

QModelIndex FindPrivate::parentNext(const QModelIndex &currentIndex)
{
    const QAbstractItemModel *model = currentIndex.model();
    QModelIndex parent = currentIndex.parent();
    QModelIndex parentForward = model->index(parent.row() + 1, currentIndex.column(), parent.parent());
    if (!parent.isValid()) return QModelIndex();
    else {
        if (parentForward.isValid()) return parentForward;
        else return parentNext(model->index(parent.row(), currentIndex.column(), parent.parent()));
    }
}

QModelIndex FindPrivate::childNext(const QModelIndex &currentIndex)
{
    const QAbstractItemModel *model = currentIndex.model();
    QModelIndex oneIndex = model->index(currentIndex.row(), 0, currentIndex.parent());
    if (model->hasChildren(oneIndex)) {
        return childNext(oneIndex.child(model->rowCount(oneIndex) - 1, currentIndex.column()));
    } else
        return model->index(model->rowCount(currentIndex.parent()) - 1, currentIndex.column(), currentIndex.parent());
}

void FindPrivate::_q_textChange(const QString &text)
{
    Q_Q(Find);

    lFind->hide();
    findText = text;
    btnNext->setEnabled(!text.isEmpty());
    btnPrev->setEnabled(!text.isEmpty());
    emit q->textChanged(text);
}

void FindPrivate::_q_optionsChange(bool checked)
{
    Q_Q(Find);

    QString settingName = q->sender()->objectName();
    if (settingName == "caseSensitive") {
        if (checked) findOptions |= Find::CaseSensitive;
        else findOptions ^= Find::CaseSensitive;
    }

    if (settingName == "wholeWord") {
        if (checked) findOptions |= Find::WholeWord;
        else findOptions ^= Find::WholeWord;
    }

    if (settingName == "oneColumn") {
        if (checked) findOptions |= Find::OneColumn;
        else findOptions ^= Find::OneColumn;
    }

    emit q->optionsChanged(findOptions);
}

/* ************************* Методы класса Find ************************* */

Find *Find::instance(QWidget *widget)
{
    // проверка на принадлежность формы к классу QMainWindow
    QWidget *window = widget;
    if (!dynamic_cast<QMainWindow*>(window)) {
        while(!window->isWindow())
            window = window->parentWidget();
    }

    // поиск уже существующего объекта в списке объектов objectList
    Find *find = 0;
    foreach (Find *object, objectList) {
        if (object->objectName() == window->objectName() + "_find") {
            find = object;
            break;
        }
    }

    // если объект не найден, то создаем его
    if (!find) {
        find = new Find(widget);
        objectList << find;
    }

    // если объект найден, то возвращаем его
    return find;
}

Find::~Find()
{
    Q_D(Find);
    if (dynamic_cast<QToolBar*>(d->parentWgt)) {
        delete d->parentWgt;
    } else {
        if (d->spacerWgt) delete d->spacerWgt;
        if (d->eText) delete d->eText;
        if (d->btnNext) delete d->btnNext;
        if (d->btnPrev) delete d->btnPrev;
        if (d->btnSettings) delete d->btnSettings;
    }

    int index = 0;
    foreach (Find *object, objectList) {
        if (object->objectName() == objectName()) {
            objectList.remove(index);
            break;
        }
        index += 1;
    }
}

QString Find::findText() const
{
    Q_D(const Find);
    return d->findText;
}

void Find::setFindText(const QString &text)
{
    Q_D(Find);
    d->findText = text;
    d->eText->setText(text);
}

Find::FindOptions Find::findOptions() const
{
    Q_D(const Find);
    return d->findOptions;
}

void Find::setFindOptions(Find::FindOptions findOptions)
{
    Q_D(Find);

    if (findOptions & Find::CaseSensitive) {
        if (!d->aCaseSensitive->isChecked())
            d->aCaseSensitive->setChecked(true);
    } else {
        if (d->aCaseSensitive->isChecked())
            d->aCaseSensitive->setChecked(false);
    }

    if (findOptions & Find::WholeWord) {
        if (!d->aWholeWord->isChecked())
            d->aWholeWord->setChecked(true);
    } else {
        if (d->aWholeWord->isChecked())
            d->aWholeWord->setChecked(false);
    }

    if (findOptions & Find::OneColumn) {
        if (!d->aOneColumn->isChecked())
            d->aOneColumn->setChecked(true);
    } else {
        if (d->aOneColumn->isChecked())
            d->aOneColumn->setChecked(false);
    }
}

void Find::setVisible(bool visible, bool focus)
{
    Q_D(Find);

    if (visible) {
        d->parentWgt->show();
        if (focus) {
            d->parentWgt->activateWindow();
            d->eText->setFocus();
            d->eText->selectAll();
        }
    } else {
        d->parentWgt->hide();
        if (d->sourceView) d->sourceView->setFocus();
    }
}

void Find::setSourceView(QVector<QAbstractItemView*> view)
{
    Q_D(Find);
    d->sourceViewList = view;
}

QModelIndex Find::findIndex(QAbstractItemModel *sourceModel, const QModelIndex &currentIndex, QString findText,
                            Find::FindOptions findOptions, Find::FindCourse findCourse)
{
    QModelIndex findIndex = QModelIndex();
    QModelIndex startIndex = currentIndex;
    int k = 0;

    Begin:

    if (findCourse == Find::FindFirst)
        startIndex = sourceModel->index(0, startIndex.column());
    if (findCourse == Find::FindLast)
        startIndex = FindPrivate::childNext(sourceModel->index(sourceModel->rowCount() - 1, startIndex.column()));

    QModelIndex parentIndex = startIndex.parent();

    // область поиска
    int columnBegin = 0;
    int columnEnd   = 0;

    // взависимости от опций определяем область поиска
    int inc = 1;
    if (findOptions & Find::OneColumn) {
        if (findCourse == Find::FindForward) {
            columnBegin = startIndex.column();
            columnEnd = startIndex.column() + 1;
        } else if (findCourse == Find::FindBachward) {
            columnBegin = startIndex.column();
            columnEnd = startIndex.column() - 1;
            inc = -1;
        } else if (findCourse == Find::FindFirst) {
            columnBegin = startIndex.column();
            columnEnd = startIndex.column() + 1;
        } else if (findCourse == Find::FindLast) {
            columnBegin = startIndex.column();
            columnEnd = startIndex.column() - 1;
            inc = -1;
        }
    } else {
        if (findCourse == Find::FindForward) {
            columnBegin = startIndex.column();
            columnEnd = sourceModel->columnCount(parentIndex);
        } else if (findCourse == Find::FindBachward) {
            columnBegin = startIndex.column();
            columnEnd = -1;
            inc = -1;
        } else if (findCourse == Find::FindFirst) {
            columnBegin = 0;
            columnEnd = sourceModel->columnCount(parentIndex);
        } else if (findCourse == Find::FindLast) {
            columnBegin = sourceModel->columnCount(parentIndex) - 1;
            columnEnd = -1;
            inc = -1;
        }
    }

    if (k != 0 && !(findOptions & Find::OneColumn)) {
        if (findCourse == Find::FindForward) columnBegin = 0;
        if (findCourse == Find::FindBachward) columnBegin = sourceModel->columnCount(parentIndex) - 1;
    }

    for (int j = columnBegin; j != columnEnd; j += inc, ++k) {

        if (k == 0 && (findCourse != Find::FindFirst) && (findCourse != Find::FindLast)) continue;

        QString text = sourceModel->index(startIndex.row(), j, parentIndex).data().toString();
        if (!(findOptions & Find::CaseSensitive)) {
            text = text.toLower();
            findText = findText.toLower();
        }

        QRegExp regExp;
        if (findOptions & Find::WholeWord) {
            regExp = QRegExp("\\b" + findText.replace(QRegExp("\\s"), "\\b\\s\\b") + "\\b");
        } else {
            regExp = QRegExp(".*" + findText.replace(QRegExp("\\s+"), ".*") + ".*");
        }

        if (text.indexOf(regExp) != -1)
            return findIndex = sourceModel->index(startIndex.row(), j, parentIndex);
    }

    // искать назад
    if (findCourse == Find::FindBachward || findCourse == Find::FindLast) {

        QModelIndex backwardIndex = sourceModel->index(startIndex.row() - 1, startIndex.column(), startIndex.parent());
        if (backwardIndex.isValid()) {
            if (sourceModel->hasChildren(sourceModel->index(startIndex.row() - 1, 0, startIndex.parent()))) {
                QModelIndex childLastIndex = FindPrivate::childNext(backwardIndex);

                // замена рекурсии
                startIndex = childLastIndex;
                findCourse = Find::FindBachward;
                ++k;
                goto Begin;
            } else {

                // замена рекурсии
                startIndex = backwardIndex;
                findCourse = Find::FindBachward;
                ++k;
                goto Begin;
            }
        } else {
            QModelIndex parentIndex = startIndex.parent();
            if (parentIndex.isValid()) {

                // замена рекурсии
                startIndex = sourceModel->index(parentIndex.row(), startIndex.column(), parentIndex.parent());
                findCourse = Find::FindBachward;
                ++k;
                goto Begin;
            }
        }
    }

    // искать вперед
    if (findCourse == Find::FindForward || findCourse == Find::FindFirst) {
        if (sourceModel->hasChildren(sourceModel->index(startIndex.row(), 0, startIndex.parent()))) {

            // замена рекурсии
            startIndex = sourceModel->index(startIndex.row(), 0, startIndex.parent()).child(0, startIndex.column());
            findCourse = Find::FindForward;
            ++k;
            goto Begin;

            if (findIndex != QModelIndex()) return findIndex;
        }

        QModelIndex forwardIndex = sourceModel->index(startIndex.row() + 1, startIndex.column(), startIndex.parent());
        if (forwardIndex.isValid()) {

            // замена рекурсии
            startIndex = forwardIndex;
            findCourse = Find::FindForward;
            ++k;
            goto Begin;

        } else {
            QModelIndex parentForwardIndex = FindPrivate::parentNext(startIndex);
            if (parentForwardIndex.isValid()) {

                // замена рекурсии
                startIndex = parentForwardIndex;
                findCourse = Find::FindForward;
                ++k;
                goto Begin;

            }
        }
    }

    return findIndex;

}

void Find::findFirst()
{
    Q_D(Find);
    if (sourceView()) d->find(sourceView()->currentIndex(), Find::FindFirst);
}

void Find::findForward()
{
    Q_D(Find);
    if (sourceView()) d->find(sourceView()->currentIndex(), Find::FindForward);
}

void Find::findBachward()
{
    Q_D(Find);
    if (sourceView()) d->find(sourceView()->currentIndex(), Find::FindBachward);
}

Find::Find(QWidget *parent) :
    QObject(parent), d_ptr(new FindPrivate())
{
    Q_D(Find);
    d->q_ptr = this;

    // определяем родительское окно
    QWidget *window = parent;
    while(!window->isWindow())
        window = window->parentWidget();
    setObjectName(window->objectName() + "_find");

    // ищем источник для поиска
    d->findSourceView(window);

    // создаем виджет
    if (dynamic_cast<QMainWindow*>(parent)) {
        QToolBar *toolBar = d->createToolBar(parent);
        dynamic_cast<QMainWindow*>(parent)->addToolBar(Qt::TopToolBarArea, toolBar);
    } else {
        if (parent->layout()) delete parent->layout();
        parent->setLayout(d->createLayout(parent));
        d->parentWgt = parent;
        d->parentWgt->setVisible(false);
    }
}

QAbstractItemView *Find::sourceView() const
{
    Q_D(const Find);
    return d->sourceView;
}

#include "moc_find.cpp"
