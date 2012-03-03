#include "hheadersqlquerymodel.h"

#include "HierarchicalHeaderView.h"

HHeaderSqlQueryModel::HHeaderSqlQueryModel( QObject *parent )
        : QSqlQueryModel( parent )
{
    hModel = new QStandardItemModel( this );

    _horizontalHeaderModel = hModel;
}

QVariant HHeaderSqlQueryModel::data( const QModelIndex &index, int role ) const
{
    if( _horizontalHeaderModel && role == HierarchicalHeaderView::HorizontalHeaderDataRole )
    {
        QVariant v;
        v.setValue( (QObject*) _horizontalHeaderModel.data() );
        return v;
    }
    return QSqlQueryModel::data( index, role );
}

QStandardItemModel *HHeaderSqlQueryModel::headerModel()
{
    return hModel;
}
