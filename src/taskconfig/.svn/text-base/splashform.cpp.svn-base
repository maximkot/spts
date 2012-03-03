#include <QtGui/QMessageBox>
#include "splashform.h"

#include <QPushButton>

#include "taskconfig.h"

SplashForm::SplashForm(QWidget *parent): QDialog(parent)
{
    ui.setupUi(this);

#ifdef TASKCONFIG_RESCUE
    ui.Image_flag->setPixmap( QPixmap( QString::fromUtf8( ":/splash/icons/flagofrescue.png" ) ) );
    ui.L_title->setText( tr( "КСА МСО" ) );
#endif
#ifdef TASKCONFIG_ASUTOK
    ui.L_title->setText( "" );
#endif
    //setWindowTitle( TaskConfig::getFormTitleName( objectName() ).toLatin1() );
    ui.buttonBox->button( QDialogButtonBox::Ok )->setText( tr( "Войти" ) );
}
