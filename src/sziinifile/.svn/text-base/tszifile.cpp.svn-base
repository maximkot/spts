
// $Id:$

#include "tszifile.h"
#include "nomacl.h"
#include "fcntl.h"

int nomacl_conn_num=0;

TSziFile::TSziFile() :QIODevice()
{
    nomacl_error=0;
    connect_nomacl();
}

TSziFile::TSziFile( QString fileName ) :QIODevice()
{
    nomacl_error=0;
    this->fileName=fileName;
    connect_nomacl();
}

TSziFile::TSziFile(QObject * parent ) :QIODevice( parent)
{
    nomacl_error=0;
    connect_nomacl();
}

void TSziFile::connect_nomacl()
{
    if (nomacl_conn_num==0) {
        int ret=open_connection();
        if (ret==0) {    
           isConnected=true;
           nomacl_conn_num++;
        } 
	else {
	 isConnected = false;
	 nomacl_error=ret;
	} 
    } else
    {
      isConnected=true;
      nomacl_conn_num++;
    }
}

TSziFile::~TSziFile()
{
    if (isConnected)
     {      
      nomacl_conn_num--;
      if (nomacl_conn_num==0) close_connection();
     }
}

bool TSziFile::open(OpenMode mode)
{
    if (isOpen()) return false;
    if (fileName.isEmpty()) return false;
    if (!isConnected) return false;

    int ret = nomacl_open(fileName.toLocal8Bit().data(), mode-1);

    if (ret<0) 
    {
      nomacl_error=ret;
      return false;
    }

    QIODevice::open(mode);

    fileDesc = ret;
    nomacl_error = 0;
    return true;
}

bool TSziFile::open_creat(OpenMode mode, mode_t cr_mode)
{
    if (isOpen()) return false;
    if (fileName.isEmpty()) return false;
    if (!isConnected) return false;

    int ret = nomacl_open_creat(fileName.toLocal8Bit().data(), mode-1 | O_CREAT, cr_mode);

    if (ret<0) 
    {
      nomacl_error = ret;
      return false;
    }

    QIODevice::open(mode);
    fileDesc = ret;
    nomacl_error = 0;

    return true;
}

bool TSziFile::creat(mode_t cr_mode)
{
    if (isOpen()) return false;
    if (fileName.isEmpty()) return false;
    if (!isConnected) return false;

    int ret = nomacl_creat(fileName.toLocal8Bit().data(), cr_mode);

    if (ret<0) 
    {
      nomacl_error = ret;
      return false;
    }
    
    QIODevice::open(QFile::ReadWrite);
    nomacl_error = ret;
    fileDesc = ret;

    return true;
}

void TSziFile::close()
{
    QIODevice::close();
    nomacl_close(fileDesc);
}

qint64 TSziFile::readData(char *data, qint64 maxlen)
{
    if (!isConnected) return -1;
    nomacl_error = 0;
    return nomacl_read(fileDesc,data,maxlen);
}

qint64 TSziFile::writeData(const char *data, qint64 len)
{
    if (!isConnected) return -1;
    nomacl_error = 0;    
    return nomacl_write(fileDesc,data,len);
}

bool TSziFile::seek(qint64 pos){
    if (!isConnected) return false;
    int ret=nomacl_lseek(fileDesc, pos, SEEK_SET);
    nomacl_error = ret;
    if (ret>-1) {nomacl_error = 0; return true;} 
    else {nomacl_error = ret; return false;}
}

bool TSziFile::isSequential ()
{
    //we are random-acces IODevice :)
    return false;
}

bool TSziFile::exists (QString name)
{
    return test_exist(name.toLocal8Bit().data());
}

QFile::FileError TSziFile::error()
{
    return (QFile::FileError)get_errno();
}

int TSziFile::resize(qint64 sz)
{
    int ret = nomacl_ftruncate(fileDesc, sz);
    if (ret<0) nomacl_error=ret; else nomacl_error = 0;
    return ret;
}

int TSziFile::euidaccess(QString name, int type)
{
    int ret = nomacl_euidaccess(name.toLocal8Bit().data(), type);
    if (ret<0) nomacl_error=ret; else nomacl_error = 0;
    return ret;
}

int TSziFile::mkdir(QString name)
{
    int ret = nomacl_mkdir(name.toLocal8Bit().data());
    if (ret<0) nomacl_error=ret; else nomacl_error = 0;
    return ret;    
}

int TSziFile::copy(QString oldname, QString newname)
{
    int ret =  nomacl_copy(oldname.toLocal8Bit().data(), newname.toLocal8Bit().data());
    if (ret<0) nomacl_error=ret; else nomacl_error = 0;
    return ret;
}

bool TSziFile::isNomaclConnected(){
    return isConnected;
}

int TSziFile::last_nomacl_error(){
 return nomacl_error;
}
