#include "genfont.h"
#include <QFile>
//------------------------------------------------------------------------------
//ABCDEFGHIJKLMNOPQRSTUVWXYZ
//abcdefghijklmnopqrstuvwxyz
//АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ
//абвгдеёжзийклмнопрстуфхцчшщъыьэюя
// (){}[]<>:;"'.!?\|/@#$%^&*-+_
//------------------------------------------------------------------------------
static QString TestENG = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static QString Testeng = "abcdefghijklmnopqrstuvwxyzτ";
//------------------------------------------------------------------------------
static QString TestRUS = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
static QString Testrus = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
//------------------------------------------------------------------------------
static QString TestNum = "0123456789";
//------------------------------------------------------------------------------
static QString TestSpec = "(){}[]<>:;\"'.,!?\\|/@#$%^&*-+№=";
static QString TestSpec_n = "\n\r\t ";
//------------------------------------------------------------------------------
static QString TestImg = "λ▲▶▼◀";
//------------------------------------------------------------------------------
/*
typedef struct {
	uint16_t GlyphIndex,
	uint16_t CharIndex,
	uint16_t CountIndex
} FONT_IndexTypeDef;

typedef struct {
	uint8_t *buffer;
	uint8_t width;	//Ширина
	uint8_t row;	//Высота
	uint8_t left;	//Отступ слева
	uint8_t top;	//Отступ вверх от центральной линии
} FONT_GlyphTypeDef;

typedef struct {
	FONT_IndexTypeDef *Index;
	FONT_GlyphTypeDef *Glyph;
	uint16_t GlyphCount;
	uint16_t IndexCount;
} FONT_FontTypeDef;
*/
//------------------------------------------------------------------------------
/*
G: 0 	C: 9 	Count: 2
G: 2 	C: 13 	Count: 1
G: 3 	C: 33 	Count: 28
G: 31 	C: 62 	Count: 33
G: 64 	C: 97 	Count: 29
G: 93 	C: 1025 	Count: 1
G: 94 	C: 1040 	Count: 64
G: 158 	C: 1105 	Count: 1

#define ARRAY_SIZE(x)			(sizeof(x) / sizeof((x)[0]))
int FONT_CharToGlyphIndex(FONT_RangeIndexDef *ri,int sz,int c)
{
	int ret = 0;
	for(int i = 0; i<sz ;i++) {
		if((ri.CharIndex <= c) && (c < ri.CharIndex + ri.CountIndex)) {
			ret = ri.CharIndex - c;
			break;
		}
	}
}
*/
//------------------------------------------------------------------------------
GenFont::GenFont()
{
	FontOptSize = 0;
}

void GenFont::SetOptSize(bool os)
{
	FontOptSize = os;
}

//------------------------------------------------------------------------------
void GenFont::SetFontName(QString fn)
{
	FontName = fn;
}
//------------------------------------------------------------------------------
void GenFont::SetFontFileName(QString ffn)
{
	FontFileName = ffn;
}
//------------------------------------------------------------------------------
void GenFont::SetFontSize(quint32 fsz) {
	FontSize = fsz;
}
//------------------------------------------------------------------------------
void GenFont::Load(QString c)
{
	foreach (GenIndex *gi, ListIndex) {
		delete gi;
	}

	ListIndex.clear();

	foreach (GenGlyph *gg, ListGlyph) {
		delete gg;
	}

	ListGlyph.clear();

	QList<QChar>	listchar;
	QList<quint16>	listcharindex;

	for(int i = 0;i<c.size();++i) {
		listchar.append(c.at(i));
	}
	qSort(listchar);
	foreach (QChar qc, listchar) {
		listcharindex.append(qc.unicode());
	}
	GenIndex::SearchRangIndex(listcharindex,ListIndex);
	foreach (GenIndex *var, ListIndex) {
		qDebug()<<var->Info();
	}

	if(FontFileName.isNull()){
		qDebug()<<"FontName.isNull()";
		return;
	}

	FT_Library	lib;
	FT_Face		face;
	GenGlyph::InitLib(lib,face,FontFileName);

	foreach (quint16 ci, listcharindex) {
		GenGlyph *gg = new GenGlyph;
		gg->SetFace(face);
		gg->SetSize(FontSize);
		gg->SetCharCode(ci);
		gg->GlyphToBuffer();
		FontAsc = gg->GetAsc();
		FontDesc = gg->GetDesc();
		qDebug()<<gg->Info();
		ListGlyph<<gg;
	}

	GenGlyph::DeInitLib(lib,face);
}
//------------------------------------------------------------------------------
void GenFont::LoadTestString()
{
	qDebug()<<"CharGlyphIndex::LoadTestString";
	Load(TestENG +
			   Testeng +
			   TestRUS +
			   Testrus +
			   TestSpec +
			   TestSpec_n +
				TestImg +
		 TestNum);

	//Load("H");
}
//------------------------------------------------------------------------------
void GenFont::GenFontSourceFile(QString path)
{
	QStringList strlist;
	strlist<<QString("#include \"%1.h\"\n\n").arg(QString(FontName).toLower());
	strlist<<"static const  FONT_IndexTypeDef FIndex[] = {\n";
	for(int i=0;i<ListIndex.size();++i) {
		if(i==0) strlist<<"\t{\n"; else strlist<<"\t},{\n";
		ListIndex.at(i)->GenStructIndex(strlist);
		if(i + 1 == ListIndex.size())
			strlist<<"\t}\n";
	}
	strlist<<"};\n\n";

	for(int i=0;i<ListGlyph.size();++i) {
		quint32 cc = ListGlyph.at(i)->GetCharCode();
		strlist<<QString("static const uint8_t FONT_Buffer_%1[] = {\n").arg(cc);
		if(FontOptSize) {
			ListGlyph.at(i)->GenStructGlyphBufferOptSize(strlist,0x80);
		} else {
			ListGlyph.at(i)->GenStructGlyphBuffer(strlist);
		}
		strlist<<"};\n\n";
	}

	strlist<<"static const  FONT_GlyphTypeDef FGlyph[] = {\n";
	for(int i=0;i<ListGlyph.size();++i) {
		if(i==0) strlist<<"\t{"; else strlist<<"\t},{";
		strlist<<"\n";
		ListGlyph.at(i)->GenStructGlyph(strlist);
		if(i + 1 == ListGlyph.size())
			strlist<<"\t}\n";
	}
	strlist<<"};\n\n";

	strlist<<QString("FONT_TypeDef FONT_%1 = {\n").arg(FontName);
	strlist<<"\t.Index\t\t= FIndex,\n";
	strlist<<"\t.Glyph\t\t= FGlyph,\n";
	strlist<<"\t.IndexCount\t= ARRAY_SIZE(FIndex),\n";
	strlist<<"\t.GlyphCount\t= ARRAY_SIZE(FGlyph),\n";
	strlist<<QString("\t.GlyphSize\t= %1,\n").arg(FontSize);
	strlist<<QString("\t.GlyphAsc\t= %1,\n").arg(FontAsc);
	strlist<<QString("\t.GlyphDesc\t= %1,\n").arg(FontDesc);
	strlist<<"};\n\n";

	GenFont::SaveFile(path + QString(FontName).toLower() + ".c",strlist);
}
//------------------------------------------------------------------------------
void GenFont::GenFontHeaderFile(QString path)
{
	QStringList strlist;
	strlist<<QString("#ifndef FONT_%1_H\n").arg(QString(FontName).toUpper());
	strlist<<QString("#define FONT_%1_H\n\n").arg(QString(FontName).toUpper());
	strlist<<"#include \"fontdef.h\"\n\n";
	strlist<<QString("FONT_TypeDef FONT_%1;\n\n").arg(FontName);
	strlist<<"#endif\n";
	GenFont::SaveFile(path + QString(FontName).toLower() + ".h",strlist);
}
//------------------------------------------------------------------------------
void GenFont::GenStructDefFont(QStringList &strlist)
{
	strlist<<"typedef struct {\n";
	strlist<<"\tconst FONT_IndexTypeDef	*Index;\n";
	strlist<<"\tconst FONT_GlyphTypeDef	*Glyph;\n";
	strlist<<"\tconst uint16_t			GlyphCount;\n";
	strlist<<"\tconst uint16_t			IndexCount;\n";
	strlist<<"\tconst uint16_t			GlyphSize;\n";
	strlist<<"\tconst int8_t			GlyphAsc;\n";
	strlist<<"\tconst int8_t			GlyphDesc;\n";
	strlist<<"} FONT_TypeDef;\n";
}
//------------------------------------------------------------------------------
void GenFont::GenStructDefFile(QStringList &strlist,bool FontOptSize)
{
	strlist.clear();
	strlist << "#ifndef __FONT_DEF_H\n";
	strlist << "#define __FONT_DEF_H\n\n";
	strlist << "#include \"stdio.h\"\n";
	strlist << "#include \"stdint.h\"\n\n";
	GenIndex::GenStructDefIndex(strlist);
	strlist << "\n";
	GenGlyph::GenStructDefGlyph(strlist);
	strlist << "\n";
	GenFont::GenStructDefFont(strlist);
	strlist << "\n";
	strlist << "int FONT_CharToGlyphIndex(const FONT_IndexTypeDef *Index,int Sz,int Unc);\n";
	strlist << "int FONT_CharUtf8ToUnc(char *text,int *size);\n";
	strlist << "\n";
	if(FontOptSize) {
		strlist << "#define FONT_OPTIMIZE_SIZE 1\n";
	} else {
		strlist << "#define FONT_OPTIMIZE_SIZE 0\n";
	}
	strlist << "\n";
	strlist << "#if !defined  (ARRAY_SIZE)\n";
	strlist << "#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))\n";
	strlist << "#endif\n\n";
	strlist << "#endif\n";
}
//------------------------------------------------------------------------------
void GenFont::SaveFile(QString file,const QStringList &strlist)
{
	QFile f(file);
	if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
		qDebug()<<"Not Open"<<file;
		return;
	}
	QTextStream stream(&f);
	foreach (QString line, strlist) {
		stream<<line;
	}
	f.close();
}
//------------------------------------------------------------------------------
void GenFont::GenDefFile()
{
	QStringList strlist;
	GenFont::GenStructDefFile(strlist);
	GenFont::SaveFile("fontdef.h",strlist);
}
//------------------------------------------------------------------------------
