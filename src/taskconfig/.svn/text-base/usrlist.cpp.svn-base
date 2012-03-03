#include "usrlist.h"
#include "taskconfig.h"
#include "ui_usrlist.h"

static UsrList *usrList;

//usrList = 0;

UsrList* UsrList::instance(){ return usrList ? usrList : usrList = new UsrList; }
void UsrList::usrDelete(){delete usrList;}



UsrList::UsrList(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::UsrList)
{
    ui->setupUi(this);
    QSqlQuery sqlTemp( "CREATE local TEMPORARY TABLE \"usrlisttemp\" (\"usrcode\" INTEGER NOT NULL, CONSTRAINT \"usrlisttemp_pkey\" PRIMARY KEY(\"usrcode\") ) WITH OIDS ON COMMIT PRESERVE ROWS" );
    TSziIniFile settings( qTC->getWorkPath()+"users_settings.cfg" );
    settings.beginGroup( "users" );
    QStringList val= settings.value( "usrs", 0 ).toStringList();
    foreach(QString i, val)
    {
        sqlTemp.exec("insert into usrlisttemp (usrcode) values ("+i+")");
    }
    QSqlDatabase db=QSqlDatabase::addDatabase("QPSQL", "oss");
    db.setHostName(QSqlDatabase::database().hostName());
    db.setUserName(QSqlDatabase::database().userName());
    db.setPassword(QSqlDatabase::database().password());
    db.setDatabaseName("0300_oss");
    model=new UsrModel(ui->tvList);
    usrListUp();
}


void UsrList::usrListUp()
{

    QSqlQuery sqlTemp1=QSqlQuery("select * from getuserinfo('"+QSqlDatabase::database("oss").userName()+"',false)",QSqlDatabase::database("oss"));
    sqlTemp1.exec();
    sqlTemp1.first();

    QSqlRecord rec=sqlTemp1.record();
    sqlTemp1.clear();
    QSqlDatabase::database("oss").close();
    QSqlDatabase::removeDatabase("oss");
    QSqlQuery sqlTemp;
    sqlTemp.prepare("SELECT * FROM public.s_usr WHERE (s_usr.usr_fio=:fio) AND (s_usr.usr_zvan=:zvan)");
    sqlTemp.bindValue(":fio",rec.value("sotr_fio").toString());
    sqlTemp.bindValue(":zvan",rec.value("zvan_names").toString());
    sqlTemp.exec();
    if(!sqlTemp.first())
    {
        sqlTemp.prepare("INSERT INTO public.s_usr (usr_fio,usr_zvan) VALUES (:fio,:zvan)");
        sqlTemp.bindValue(":fio",rec.value("sotr_fio").toString());
        sqlTemp.bindValue(":zvan",rec.value("zvan_names").toString());
        sqlTemp.exec();
    }
    fillList();
}


UsrList::~UsrList()
{
    QSqlQuery sqlTemp;

    if(usrList->ui->rbAll->isChecked())
    {
        sqlTemp.exec("delete from usrlisttemp");
    }
    QStringList val;
    sqlTemp.exec("select * from usrlisttemp");
    while(sqlTemp.next())
    {
        val.append(sqlTemp.record().value("usrcode").toString());
    }
    TSziIniFile settings( qTC->getWorkPath()+"users_settings.cfg" );
    settings.beginGroup( "users" );
    settings.setValue("usrs",val);
    settings.endGroup();
    settings.sync();
    delete ui;
}

void UsrList::fillList()
{

    model->setQuery("select s_usr.usr_code,s_usr.usr_fio,s_usr.usr_zvan, "
                    "case "
                    "when s_usr.usr_code=usrlisttemp.usrcode then true::boolean "
                    "else false::boolean "
                    "end as checked "
                    "from public.s_usr left join usrlisttemp on usrlisttemp.usrcode=s_usr.usr_code");
    ui->tvList->setModel(model);

    model->setHeaderData(1, Qt::Horizontal, tr("ФИО"));
    model->setHeaderData(2, Qt::Horizontal, tr("Звание"));
    model->setHeaderData(3, Qt::Horizontal, tr(" "));
    ui->tvList->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
    ui->tvList->hideColumn(0);
    ui->tvList->hideColumn(3);

}

UsrModel::UsrModel(QWidget *parent): QSqlQueryModel(parent) {
    _checkableField = "usr_fio";
    _parent = static_cast<QTableView*>(parent);

}

Qt::ItemFlags UsrModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (record().fieldName(index.column()) == _checkableField)
        flags |= Qt::ItemIsUserCheckable ;
    return flags;
}

QVariant UsrModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::CheckStateRole) {
        if (record().fieldName(index.column()) == _checkableField) {
            int value = this->record(index.row()).value("checked").toInt();

            return (value == 0 ? Qt::Unchecked : Qt::Checked);
        }
    }


    return QSqlQueryModel::data(index, role);
}

bool UsrModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid())
        return false;

    if (role == Qt::CheckStateRole) {
        if (record().fieldName(index.column()) == _checkableField) {
            int scroll = _parent->verticalScrollBar()->value();
            int code=this->record(index.row()).value("usr_code").toInt();

            QSqlQuery query;
            if (value.toInt() == Qt::Checked)
                query.prepare("insert into usrlisttemp (usrcode) values (:code)");
            else
                query.prepare("delete from usrlisttemp where usrcode=:code");
            query.bindValue(":codenn", code);
            query.exec();
            this->setQuery(this->query().lastQuery());
            _parent->setCurrentIndex(index);
            _parent->verticalScrollBar()->setValue(scroll);
        }
    }
    emit layoutChanged();
    return true;
}

/**/

void UsrList::on_rbSel_clicked()
{
    ui->tvList->setEnabled(true);
}


void UsrList::on_rbAll_clicked()
{
    ui->tvList->setEnabled(false);
}
