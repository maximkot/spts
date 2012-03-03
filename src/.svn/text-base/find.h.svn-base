#ifndef FIND_H
#define FIND_H

/*
 * РћРїРёСЃР°РЅРёРµ:
 *  РљР»Р°СЃСЃ РїРѕРёcРєР°.
 *  РџРѕС‚РґРµСЂР¶РёРІР°СЋС‚СЃСЏ РєР»Р°СЃСЃС‹ QTableView, QTreeView, QTableWidget, QTreeWidget.
 *
 * Р¤СѓРЅРєС†РёРё:
 *  Find::instance(QWidget *widget) - СЃРѕР·РґР°РЅРёРµ РѕР±СЉРµРєС‚Р° РїРѕРёСЃРєР°. Р•СЃР»Рё РІС…РѕРґСЏС‰РёР№ РїР°СЂР°РјРµС‚СЂ
 *  РїСЂРёРЅР°РґР»РµР¶РёС‚ РєР»Р°СЃСЃСѓ QMainWindow, С‚РѕРіРґР° РїРѕРёСЃРє Р±СѓРґРµС‚ РІС‹РїРѕР»РЅРµРЅ РІ РІРёРґРµ РїР°РЅРµР»Рё РёРЅСЃС‚СЂСѓРјРµРЅС‚РѕРІ(QToolBar),
 *  РёРЅР°С‡Рµ РёРЅС‚РµСЂС„РµР№СЃ РїРѕРёСЃРєР° Р±СѓРґРµС‚ СЂР°Р·РјРµС‰РµРЅ РЅР° РїРµСЂРµРґР°РЅРЅРѕРј widget-Рµ.
 *  Find::setVisible(bool visible, bool focus) - СѓРїСЂР°РІР»РµРЅРёРµ РѕС‚РѕР±СЂР°Р¶РµРЅРёРµРј РїРѕРёСЃРєР°.
 *  РџРѕ СѓРјРѕР»С‡Р°РЅРёСЋ, РїРѕСЃР»Рµ СЃРѕР·РґР°РЅРёСЏ, РѕР±СЉРµРєС‚ РїРѕРёСЃРєР° РЅРµ РѕС‚РѕР±СЂР°Р¶РµРЅ РЅР° СЌРєСЂР°РЅРµ.
 *  Find::setSourceView(QVector<QAbstractItemView*> view) - РјРѕР¶РЅРѕ Р·Р°РґР°С‚СЊ РёСЃС‚РѕС‡РЅРёРєРё РїРѕРёСЃРєР°, РіРґРµ РёСЃРєР°С‚СЊ.
 *  РџРѕ СѓРјРѕР»С‡Р°РЅРёСЋ, РµСЃР»Рё РёСЃС‚РѕС‡РЅРёРєРё РїРѕРёСЃРєР° РЅРµ Р·Р°РґР°РЅС‹, РїРѕРёСЃРє Р±СѓРґРµС‚ СЂР°Р±РѕС‚Р°С‚СЊ СЃРѕ РІСЃРµРјРё РѕР±СЉРµРєС‚Р°РјРё, РїРѕС‚РґРµСЂР¶РёРІР°РµРјС‹С…
 *  РєР»Р°СЃСЃРѕРІ, РЅР° С„РѕСЂРјРµ.
 *
 * РќР°СЃР»РµРґРѕРІР°РЅРёРµ:
 *  РћР±СЉСЏР·Р°С‚РµР»СЊРЅРѕ РЅРµРѕР±С…РѕРґРёРјРѕ РїРµСЂРµРіСЂСѓР·РёС‚СЊ С„СѓРЅРєС†РёСЋ Find::instance(QWidget *widget). РўР°РєР¶Рµ РЅСѓР¶РЅРѕ СЂРµР°Р»РёР·РѕРІР°С‚СЊ
 *  Р°Р»РіРѕСЂРёС‚Рј РїРѕРёСЃРєР°. Р—Р° РѕСЃРЅРѕРІСѓ РјРѕР¶РЅРѕ РІР·СЏС‚СЊ СѓР¶Рµ СЃСѓС‰РµСЃС‚РІСѓСЋС‰РёР№ Р°Р»РіРѕСЂРёС‚Рј Find::findIndex(...), РґРѕРїРёСЃР°РІ
 *  РЅРµРѕР±С…РѕРґРёРјС‹Рµ РїСЂРѕРІРµСЂРєРё (РїСЂРѕРґРѕР»Р¶РёС‚СЊ РїРѕРёСЃРє СЃ РЅР°С‡Р°Р»Р°, РїСЂРѕРґРѕР»Р¶РёС‚СЊ РїРѕРёСЃРє СЃ РєРѕРЅС†Р°).
 *
 * 620-109:lukinav@jabber
 */

#include <QtGui>

class FindPrivate;
class Find : public QObject
{
    Q_OBJECT
    Q_FLAGS(FindOptions);
    Q_ENUMS(FindCourse);
    Q_PROPERTY(QString findText READ findText WRITE setFindText NOTIFY textChanged);
    Q_PROPERTY(FindOptions findOptions READ findOptions WRITE setFindOptions NOTIFY optionsChanged);

public:
    // РІРѕР·РІСЂР°С‰Р°РµС‚ СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РѕР±СЉРµРєС‚; РµСЃР»Рё РѕР±СЉРµРєС‚ РЅРµ СЃРѕР·РґР°РЅ, СЃРѕР·РґР°РµС‚ РµРіРѕ
    static Find *instance(QWidget *widget);
    virtual ~Find();

    // РЅР°РїСЂР°РІР»РµРЅРёРµ РїРѕРёСЃРєР°
    enum FindCourse { FindFirst, FindForward, FindBachward, FindLast };
    enum FindOption {
        NoOptions       = 0x00,
        CaseSensitive   = 0x01, // СѓС‡РёС‚С‹РІР°С‚СЊ СЂРµРіРёСЃС‚СЂ
        WholeWord       = 0x02, // СЃР»РѕРІРѕ С†РµР»РёРєРѕРј
        OneColumn       = 0x04  // С‚РѕР»СЊРєРѕ РїРѕ РґР°РЅРЅРѕРјСѓ СЃС‚РѕР»Р±С†Сѓ
    };
    Q_DECLARE_FLAGS(FindOptions, FindOption);

    // С‚РµРєСЃС‚ РїРѕРёСЃРєР°
    QString findText() const;
    void setFindText(const QString &text);

    // РїР°СЂР°РјРµС‚СЂС‹ РїРѕРёСЃРєР°
    Find::FindOptions findOptions() const;
    void setFindOptions(Find::FindOptions findOptions = Find::OneColumn);

    void setVisible(bool visible, bool focus = true);       // Р·Р°РґР°С‚СЊ РІРёРґРёРјРѕСЃС‚СЊ РїРѕРёСЃРєР°
    void setSourceView(QVector<QAbstractItemView*> view);   // Р·Р°РґР°С‚СЊ РёСЃС‚РѕС‡РЅРёРєРё РїРѕРёСЃРєР°

    // Р°Р»РіРѕСЂРёС‚Рј РїРѕРёСЃРєР° (СЂР°Р±РѕС‚Р°РµС‚ СЃ С‚Р°Р±Р»РёС†РµР№ Рё РґРµСЂРµРІРѕРј)
    static QModelIndex findIndex(QAbstractItemModel *sourceModel, const QModelIndex &currentIndex, QString findText,
                                 Find::FindOptions findOptions = Find::OneColumn,
                                 Find::FindCourse findCourse = Find::FindFirst);

signals:
    void textChanged(const QString &text);                  // РёР·РјРµРЅСЏРµС‚СЃСЏ С‚РµРєСЃС‚ РїРѕРёСЃРєР°
    void optionsChanged(Find::FindOptions findOptions);     // РёР·РјРµРЅСЏСЋС‚СЃСЏ РѕРїС†РёРё РїРѕРёСЃРєР°
    void visibleChanged(bool visible);                      // РёР·РјРµРЅСЏРµС‚СЃСЏ СЃРѕСЃС‚РѕСЏРЅРёРµ РІРёРґРёРјРѕСЃС‚Рё РїРѕРёСЃРєР°

public slots:
    void findFirst();     // РёСЃРєР°С‚СЊ СЃРЅР°С‡Р°Р»Р°   (Shortcut: Enter)
    void findForward();   // РёСЃРєР°С‚СЊ РІРїРµСЂРµРґ    (Shortcut: F3)
    void findBachward();  // РёСЃРєР°С‚СЊ РЅР°Р·Р°Рґ     (Shortcut: Shift + F3)

protected:
    explicit Find(QWidget *parent = 0);

    FindPrivate *const d_ptr;
    Find(FindPrivate &dd, QObject *parent = 0) : QObject(parent), d_ptr(&dd) {}

    QAbstractItemView *sourceView() const;  // С‚РµРєСѓС‰РёР№ РёСЃС‚РѕС‡РЅРёРє РїРѕРёСЃРєР°
    static QVector<Find*> objectList;

private:
    Q_DECLARE_PRIVATE(Find);
    Q_PRIVATE_SLOT(d_func(), void _q_textChange(const QString &));
    Q_PRIVATE_SLOT(d_func(), void _q_optionsChange(bool));
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Find::FindOptions);

#endif // FIND_H
