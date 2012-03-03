#ifndef HHEADERSQLQUERYMODEL_H
#define HHEADERSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QPointer>
#include <QStandardItemModel>

class HHeaderSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT

    QPointer<QAbstractItemModel> _horizontalHeaderModel;
public:
    HHeaderSqlQueryModel( QObject *parent = 0 );

//child data()
/*
   QVariant hHeaderValue = HHeaderSqlQueryModel::data( index, role );
    if( !hHeaderValue.isNull() )
        return hHeaderValue;
*/

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

    QStandardItemModel *headerModel();
private:
    QStandardItemModel *hModel;
};

#endif // HHEADERSQLQUERYMODEL_H
