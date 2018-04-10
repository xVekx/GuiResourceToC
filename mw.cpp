#include "mw.h"
#include "ui_mw.h"
//------------------------------------------------------------------------------
#include <QDebug>
#include <QFile>
//------------------------------------------------------------------------------
MW::MW(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MW)
{
	ui->setupUi(this);

	FT_Library  lib;
	FT_Face     f;
	GenGlyph::InitLib(lib,f,"/usr/share/fonts/truetype/ttf-dejavu/DejaVuSerif.ttf");
	ui->QWGlyph->SetFace(f);
	ui->QWGlyph->SetSize(24);
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

/*	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(24);
		gf.SetFontName("DejaVuSerif24");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(18);
		gf.SetFontName("DejaVuSerif18");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(14);
		gf.SetFontName("DejaVuSerif14");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(12);
		gf.SetFontName("DejaVuSerif12");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}*/


	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(24);
		gf.SetFontName("DejaVuSansMono24");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(18);
		gf.SetFontName("DejaVuSansMono18");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(20);
		gf.SetFontName("DejaVuSansMono20");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(22);
		gf.SetFontName("DejaVuSansMono22");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(16);
		gf.SetFontName("DejaVuSansMono16");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(14);
		gf.SetFontName("DejaVuSansMono14");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(12);
		gf.SetFontName("DejaVuSansMono12");
		gf.SetFontFileName("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	/*{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(12);
		gf.SetFontName("TakaoPGothic12");
		gf.SetFontFileName("/usr/share/fonts/truetype/takao-gothic/TakaoPGothic.ttf");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}

	{
		GenFont gf;
		gf.SetOptSize(optsize);
		gf.SetFontSize(24);
		gf.SetFontName("z003034l12");
		gf.SetFontFileName("/usr/share/fonts/X11/Type1/z003034l.pfb");
		gf.LoadTestString();
		gf.GenDefFile();
		gf.GenFontSourceFile(Path);
		gf.GenFontHeaderFile(Path);
	}*/

	QStringList strl;
	GenFont::GenStructDefFile(strl,optsize);
	GenFont::SaveFile(Path + "fontdef.h",strl);

}
//------------------------------------------------------------------------------
void MW::on_QPBTest2_clicked()
{
	ui->QWGlyph->SetCharCode('1');
	ui->QWGlyph->GlyphToBuffer();
	ui->QWGlyph->UpdateImage();

	QStringList strlist;
	ui->QWGlyph->GenStructGlyph(strlist);

	qDebug()<<ui->QWGlyph->Info();

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
