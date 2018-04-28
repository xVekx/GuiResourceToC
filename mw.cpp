#include "mw.h"
#include "ui_mw.h"
//------------------------------------------------------------------------------
#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QPainter>
#include "font2c.h"
//------------------------------------------------------------------------------
#define QPRINT_VAL(__val) {qDebug()<<#__val<<":"<<__val;}
//------------------------------------------------------------------------------
MW::MW(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MW)
{
	ui->setupUi(this);
	//TestSketch();

	Font2C f2c;

	f2c.SetSymbols(Font2C::TestSymbols());
	f2c.SetFamily("DejaVu Sans Mono");
	f2c.SetSize(12);
	f2c.SaveHenderDefFile();
	f2c.SaveSourceDefFile();
	f2c.SaveSourceFile();
	f2c.SaveHenderFile();

	//qDebug()<<Font2C::TestSymbols();
}
//------------------------------------------------------------------------------
void MW::TestSketch()
{
	//Просмотр доступных qt шрифтов
	QFontDatabase qfd;
	qDebug()<<qfd.families();

	static QString TestRUS = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

	foreach (QChar ch, TestRUS) {
		TestAlg(QChar(ch),50,QString("%1").arg(ch));
	}

	TestAlg(QChar(L'Ц'),50);
}
//------------------------------------------------------------------------------
void MW::TestAlg(QChar ch,int pointSize,QString file)
{

	//Выбранный шрифт
	//моноширинный шрифт приоритет при выборе
	QFont qf("DejaVu Sans Mono",pointSize);
	qf.setKerning(false);

	//Если мало доступной ОЗУ для stm32 и хватает только для 2 фрейм буферов
	//шрифты как битовая маска
	qf.setStyleStrategy(QFont::NoAntialias);

	qDebug()<<qf;
	QPRINT_VAL(qf.weight());
	QPRINT_VAL(qf.rawName());
	QPRINT_VAL(qf.family());

	QFontMetrics qfm(qf);

	QPRINT_VAL(qfm.ascent());
	QPRINT_VAL(qfm.descent() );
	QPRINT_VAL(qfm.height() );
	QPRINT_VAL(qfm.leading() );
	QPRINT_VAL(qfm.lineSpacing() );
	QPRINT_VAL(qfm.minLeftBearing() );
	QPRINT_VAL(qfm.minRightBearing() );
	QPRINT_VAL(qfm.maxWidth() );
	QPRINT_VAL(qfm.xHeight() );
	QPRINT_VAL(qfm.averageCharWidth() );


	QPRINT_VAL(qfm.leftBearing(ch) );
	QPRINT_VAL(qfm.rightBearing(ch) );
	QPRINT_VAL(qfm.width(ch));
	QPRINT_VAL(qfm.underlinePos());
	QPRINT_VAL(qfm.overlinePos());
	QPRINT_VAL(qfm.strikeOutPos());
	QPRINT_VAL(qfm.lineWidth());

	QPixmap map(qfm.width(ch),qfm.height());
	map.fill();

	QPainter painter;
	painter.begin(&map);
	painter.setFont(qf);
	painter.drawText(0,qfm.height()-qfm.descent(),ch);
	painter.end();


	QImage img = map.toImage();
	img.save(file + QString(".jpg"));

}

//------------------------------------------------------------------------------
MW::~MW()
{
	delete ui;
}
//------------------------------------------------------------------------------
void MW::paintEvent(QPaintEvent *e)
{
	QMainWindow::paintEvent(e);
}
//------------------------------------------------------------------------------
void MW::on_QPBUp_clicked()
{
	qDebug()<<"Up";
}
//------------------------------------------------------------------------------
void MW::on_QPBDown_clicked()
{
	qDebug()<<"Down";
}
//------------------------------------------------------------------------------
void MW::on_QPBTest_clicked()
{
	bool optsize = true;

	QString Path = "../FT2-Font/";
}
//------------------------------------------------------------------------------
void MW::on_QPBTest2_clicked()
{
	//ui->QWGlyph->SetCharCode('1');
	//ui->QWGlyph->GlyphToBuffer();
	//ui->QWGlyph->UpdateImage();

	QStringList strlist;
	//ui->QWGlyph->GenStructGlyph(strlist);

	//qDebug()<<ui->QWGlyph->Info();

	QFile f("file.h");
	if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
		qDebug()<<"Not Open";
		return;
	}
	QTextStream stream(&f);

	foreach (QString line, strlist) {
		stream<<line;
	}
	f.close();

	update();
}
//------------------------------------------------------------------------------
