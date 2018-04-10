#ifndef GENFONT_H
#define GENFONT_H
//------------------------------------------------------------------------------
#include "genglyph.h"
#include "genindex.h"
//------------------------------------------------------------------------------
class GenFont
{
public:
	GenFont();
	void SetFontName(QString fn);
	void SetFontFileName(QString ffn);
	void SetFontSize(quint32 fsz);

	void Load(QString c);
	void LoadTestString();

	void GenFontSourceFile(QString path = "");
	void GenFontHeaderFile(QString path = "");

	static void GenStructDefFont(QStringList &strlist);
	static void GenStructDefFile(QStringList &strlist, bool FontOptSize = 0);
	static void SaveFile(QString file, const QStringList &strlist);
	static void GenDefFile();

	void SetOptSize(bool os);
private:
	QList<GenIndex*> ListIndex;
	QList<GenGlyph*> ListGlyph;
	QString FontName;
	QString FontFileName;
	quint32 FontSize;
	qint8	FontAsc;
	qint8	FontDesc;
	bool FontOptSize;
};
//------------------------------------------------------------------------------
#endif // GENFONT_H
