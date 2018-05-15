#include "mw.h"
#include "ui_mw.h"
//------------------------------------------------------------------------------
#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QPainter>
#include "font2c.h"
#include <QFileDialog>
#include "testjson.h"

//------------------------------------------------------------------------------
#define QPRINT_VAL(__val) {qDebug()<<#__val<<":"<<__val;}
//------------------------------------------------------------------------------
#include <QDir>

MW::MW(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MW)
{
	ui->setupUi(this);

	ui->QLEFont2CPath->setText("../Resource/Font");
	ui->QLEImage2CPath->setText("../Resource/Font");

	on_QPBFont2CSetting_clicked();

	//TestJson.

	//QFile loadFile(

}

//------------------------------------------------------------------------------
MW::~MW()
{
	delete ui;
}
//------------------------------------------------------------------------------
void MW::on_QPBFont2C_clicked()
{
	Font2C f2c;

	f2c.SetSymbols(Font2C::TestSymbols());
	f2c.SetFamily("DejaVu Sans Mono");
	f2c.SetSize(12);
	f2c.SetDirPath(ui->QLEFont2CPath->text());
	f2c.SaveHenderDefFile();
	f2c.SaveSourceDefFile();
	f2c.SaveSourceFile();
	f2c.SaveHenderFile();
}
//------------------------------------------------------------------------------
void MW::on_QPBImage2C_clicked()
{

}
//------------------------------------------------------------------------------
void MW::on_QPBFont2CPath_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this,
											tr("Open Directory"),
											ui->QLEFont2CPath->text(),
											QFileDialog::ShowDirsOnly
											| QFileDialog::DontResolveSymlinks);

	ui->QLEFont2CPath->setText(dir);
}
//------------------------------------------------------------------------------
void MW::on_QPBImage2CPath_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this,
											tr("Open Directory"),
											ui->QLEImage2CPath->text(),
											QFileDialog::ShowDirsOnly
											| QFileDialog::DontResolveSymlinks);

	ui->QLEImage2CPath->setText(dir);
}

//------------------------------------------------------------------------------
void MW::on_QPBAll2C_clicked()
{
	on_QPBFont2C_clicked();
	on_QPBImage2C_clicked();
}
//------------------------------------------------------------------------------
void MW::on_QPBImage2CSetting_clicked()
{

}
//------------------------------------------------------------------------------
void MW::on_QPBFont2CSetting_clicked()
{
	qDebug()<<"123";
	FontSelect fontselect(this);
	//fontselect.show();
	fontselect.exec();
}
//------------------------------------------------------------------------------
