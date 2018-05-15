#include "font2c.h"
//------------------------------------------------------------------------------
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
//------------------------------------------------------------------------------
#define QPRINT_VAL(__val) {qDebug()<<#__val<<":"<<__val;}
//------------------------------------------------------------------------------
Font2C::Font2C() : OptSize(true)
{

}
//------------------------------------------------------------------------------
static const char TestChars [] =	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
									"abcdefghijklmnopqrstuvwxyzτ"
									"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
									"абвгдеёжзийклмнопрстуфхцчшщъыьэюя"
									"0123456789"
									"(){}[]<>:;\"'.,!?\\|/@#$%^&*-+№="
									"\n\r\t "
									"λ▲▶▼◀";
//------------------------------------------------------------------------------
QString Font2C::TestSymbols()
{
	return QString(TestChars);
}
//------------------------------------------------------------------------------
void Font2C::SetSymbols(const QString &s)
{
	Symbols = s;
	//QPRINT_VAL(Symbols);
}
//------------------------------------------------------------------------------
void Font2C::Save2File(QString file,const QStringList &strlist)
{
	QDir dir(DirPath);
	if(!dir.exists()) {
		if(!dir.mkpath(dir.absolutePath())) {
			qDebug()<<"NOT mkpath";
		}
	}

	qDebug()<<dir.absoluteFilePath(file);

	QFile f(dir.absoluteFilePath(file));
	if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
		qDebug()<<"Not Save"<<dir.absoluteFilePath(file);
		return;
	}

	QTextStream stream(&f);
	foreach (QString line, strlist) {
		stream<<line;
	}

	f.close();
}
//------------------------------------------------------------------------------
void Font2C::SaveHenderDefFile()
{
	QStringList strlist;

	strlist <<	"#ifndef __FONT_DEF_H\n"
				"#define __FONT_DEF_H\n\n"
				"#include <stdio.h>\n"
				"#include <stdint.h>\n\n";

	strlist <<	"typedef struct {\n"
				"\tconst uint16_t GlyphIndex;\n"
				"\tconst uint16_t CharIndex;\n"
				"\tconst uint16_t CountIndex;\n"
				"} FONT_IndexTypeDef;\n\n";

	strlist <<	"typedef struct {\n"
				"\tconst uint8_t *buffer;\n"
				"\tconst int8_t xoff;\tсмещение по x\n"
				"\tconst int8_t yoff;\tсмещение по y\n"
				"\tconst int8_t xsz;\n"
				"\tconst int8_t ysz;\n"
				"\tconst int8_t width;\t//Ширина\n"
				"\tconst int8_t height;\t//Высота\n"
				"\tconst int8_t left;\t//Отступ слева\n"
				"\tconst int8_t top;\t//Отступ вверх от центральной линии\n"
				"\tconst int8_t advance_x;\n"
				"\tconst int8_t ascent;\n"
				"\tconst int8_t descent;\n"
				"} FONT_GlyphTypeDef;\n\n";

	strlist <<	"typedef struct {\n"
				"\tconst FONT_IndexTypeDef\t*Index;\n"
				"\tconst FONT_GlyphTypeDef\t*Glyph;\n"
				"\tconst uint16_t\t\t\tGlyphCount;\n"
				"\tconst uint16_t\t\t\tIndexCount;\n"
				"\tconst uint16_t\t\t\tGlyphSize;\n"
				"} FONT_TypeDef;\n\n";

	strlist <<	"int FONT_CharToGlyphIndex"
				"(const FONT_IndexTypeDef *Index,int Sz,int Unc);\n"
				"int FONT_CharUtf8ToUnc(char *text,int *size);\n\n";

	if(OptSize) {
		strlist << "#define FONT_OPTIMIZE_SIZE 1\n";
	} else {
		strlist << "#define FONT_OPTIMIZE_SIZE 0\n";
	}

	strlist <<	"\n"
				"#if !defined  (ARRAY_SIZE)\n"
				"#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))\n"
				"#endif\n\n"
				"#endif\n";

	Save2File("fontdef.h",strlist);
}
//------------------------------------------------------------------------------
void Font2C::SaveSourceDefFile()
{
	QFile f(":/src/fontdef.c");
	QStringList strlist;

	if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug()<<"Not Open /src/fontdef.c";
		return;
	}

	while(!f.atEnd()) {
		strlist.append(f.readLine());
	}
	f.close();

	Save2File("fontdef.c",strlist);
}
//------------------------------------------------------------------------------
void Font2C::RangeIndex(QList<quint16> list)
{
	Index.clear();

	quint16 last = 0;
	for(int i = 0;i<list.size();i++) {
		last = list.at(i);
		int z = 0;
		for(z=1;z<list.size() - i;z++) {
			if(last+1 != list.at(i+z)) {
				break;
			}
			last = list.at(i+z);
		}
		//qDebug()<<"G:"<<i<<"\tC:"<<list.at(i)<<"\tCount:"<<z;
		Index.append(Font2CIndex(i,list.at(i),z));

		i = z + i - 1;
	}
}
//------------------------------------------------------------------------------
void Font2C::GlyphMap(QStringList &strlist,const Font2CGlyph &glyph)
{
	QImage img = glyph.GetImageGlyph();
	QString strhex;
	for(int r=0;r<glyph.Height();r++) {
		strhex += "\t\t";
		for(int w=0;w<glyph.Width();w++) {

			quint8 i = ~qGray(img.pixel(w,r));
			if((r+1 == glyph.Height()) && (w+1==glyph.Width())) {
				strhex += QString(" 0x%1").arg(i, 2, 16, QLatin1Char( '0' ));
			} else {
				strhex += QString(" 0x%1,").arg(i, 2, 16, QLatin1Char( '0' ));
			}
		}
		strhex += "\n";
	}
	strlist << strhex;
}
//------------------------------------------------------------------------------
void Font2C::GlyphMapOptSize(QStringList &strlist,const Font2CGlyph &glyph)
{
	int xoff = glyph.XOffset();
	int yoff = glyph.YOffset();

	int xsz = glyph.XSize();
	int ysz = glyph.YSize();

	//glyph.TestSave();

	QImage img = glyph.GetImageGlyph();
	QString strhex;
	quint8 c = 0x80;


	for(int y=0;y<ysz;y++) {

		QString test_hex;
		int xbit8 = 0;
		quint8 hex=0;
		strhex += "\t\t";
		for(int x=0; x < xsz; x++) {

			bool add_str = false;

			quint8 g_hex = ~qGray(img.pixel(x+xoff,y+yoff));

			if(g_hex > c) {
				hex |= (1u<<xbit8);
				test_hex.append("1");
			} else {
				test_hex.append("0");
			}

			if(xbit8 < 8) {
				xbit8++;
			} else {
				xbit8 = 0;
				add_str = true;
			}

			if(x == xsz - 1) {
				add_str = true;
			}

			if(add_str) {
				test_hex.append("=");
				strhex += QString(" 0x%1,").arg(hex, 2, 16, QLatin1Char( '0' ));
				add_str = false;
			}
		}

		//qDebug()<<test_hex;
		strhex += "\n";
	}

	strlist<<strhex;
}
//------------------------------------------------------------------------------
void Font2C::SetFamily(QString family) {
	FontFamily = family;
}
//------------------------------------------------------------------------------
void Font2C::SetSize(qint32 size) {
	FontSize = size;
}
//------------------------------------------------------------------------------
void Font2C::SetDirPath(QString dirpath) {
	DirPath = dirpath;
}
//------------------------------------------------------------------------------
void Font2C::SaveSourceFile()
{
	QList<QChar>	listchar;
	QList<quint16>	listcharindex;

	for(int i = 0;i<Symbols.size();++i) {
		listchar.append(Symbols.at(i));
	}
	qSort(listchar);
	foreach (QChar qc, listchar) {
		listcharindex.append(qc.unicode());
	}

	RangeIndex(listcharindex);

	QStringList strlist;
	strlist<<QString("#include \"%1.h\"\n\n").arg(QString(FontName).toLower());
	strlist<<"static const  FONT_IndexTypeDef FIndex[] = {\n";
	for(int i=0;i<Index.size();i++) {
		if(i==0) {
			strlist<<"\t{\n";
		} else {
			strlist<<"\t},{\n";
		}
		strlist<<QString("\t\t.GlyphIndex\t= %1,\n").arg(Index.at(i).GetGlyph());
		strlist<<QString("\t\t.CharIndex\t= %1,\n").arg(Index.at(i).GetChar());
		strlist<<QString("\t\t.CountIndex\t= %1\n").arg(Index.at(i).GetCount());

		if(i + 1 == Index.size())
			strlist<<"\t}\n";
	}

	strlist<<"};\n\n";


	Glyph.clear();
	foreach (QChar c, listchar) {
		QFont Font(FontFamily,FontSize);
		Font.setKerning(false);

		//Если мало доступной ОЗУ для stm32 и хватает только для 2 фрейм буферов
		//шрифты как битовая маска
		Font.setStyleStrategy(QFont::NoAntialias);
		Glyph.append(Font2CGlyph(c,Font));
	}

	for(int i=0;i<Glyph.size();++i) {
		quint32 cc = Glyph.at(i).UnicodeSymbol();
		strlist<<QString("static const uint8_t FONT_Buffer_%1[] = {\n").arg(cc);
		if(OptSize) {
			GlyphMapOptSize(strlist,Glyph.at(i));
		} else {
			GlyphMap(strlist,Glyph.at(i));
		}
		strlist<<"};\n\n";
	}

	strlist<<"static const  FONT_GlyphTypeDef FGlyph[] = {\n";
	for(int i = 0; i<Glyph.size();++i) {
		if(i==0) {
			strlist<<"\t{";
		} else {
			strlist<<"\t},{";
		}
		strlist<<"\n";
		strlist<<QString("\t\t.xoff\t\t= %1,\n").arg(Glyph.at(i).XOffset());
		strlist<<QString("\t\t.yoff\t\t= %1,\n").arg(Glyph.at(i).YOffset());
		strlist<<QString("\t\t.xsz\t\t= %1,\n").arg(Glyph.at(i).XSize());
		strlist<<QString("\t\t.ysz\t\t= %1,\n").arg(Glyph.at(i).YSize());
		strlist<<QString("\t\t.width\t\t= %1,\n").arg(Glyph.at(i).Width());
		strlist<<QString("\t\t.height\t\t= %1,\n").arg(Glyph.at(i).Height());
		strlist<<QString("\t\t.ascent\t\t= %1,\n").arg(Glyph.at(i).Ascent());
		strlist<<QString("\t\t.descent\t= %1,\n").arg(Glyph.at(i).Descent());
		strlist<<QString("\t\t.buffer\t\t= FONT_Buffer_%1,\n").arg(Glyph.at(i).UnicodeSymbol());
		if(i + 1 == Glyph.size())
			strlist<<"\t}\n";
	}

	strlist<<"};\n\n";
	strlist<<	QString("FONT_TypeDef FONT_%1 = {\n").arg(FontName) +
				"\t.Index\t\t= FIndex,\n"
				"\t.Glyph\t\t= FGlyph,\n"
				"\t.IndexCount\t= ARRAY_SIZE(FIndex),\n"
				"\t.GlyphCount\t= ARRAY_SIZE(FGlyph),\n"
				"};\n\n";


	FontName = FontFamily.remove(" ") + QString("%1").arg(FontSize);
	Save2File(FontName.toLower() + ".c",strlist);

	//QPRINT_VAL(FontName);
}
//------------------------------------------------------------------------------
void Font2C::SaveHenderFile()
{
	QStringList strlist;
	strlist<<QString("#ifndef FONT_%1_H\n").arg(QString(FontName).toUpper());
	strlist<<QString("#define FONT_%1_H\n\n").arg(QString(FontName).toUpper());
	strlist<<"#include \"fontdef.h\"\n\n";
	strlist<<QString("FONT_TypeDef FONT_%1;\n\n").arg(FontName);
	strlist<<"#endif\n";
	Save2File(QString(FontName).toLower() + ".h",strlist);
}
//------------------------------------------------------------------------------
