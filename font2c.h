#ifndef FONT2C_H
#define FONT2C_H
//------------------------------------------------------------------------------
#include "font2cindex.h"
#include "font2cglyph.h"
//------------------------------------------------------------------------------
#include <QString>
#include <QList>
//------------------------------------------------------------------------------
class Font2C
{
public:
	Font2C();
	static QString TestSymbols();
	void SetSymbols(const QString &s = TestSymbols());
	void Save2File(QString file, const QStringList &strlist);
	void SaveHenderDefFile();
	void SaveSourceDefFile();
	void SaveSourceFile();
	void SaveHenderFile();
	void RangeIndex(QList<quint16> list);
	void GlyphMap(QStringList &strlist, const Font2CGlyph &glyph);
	void GlyphMapOptSize(QStringList &strlist, const Font2CGlyph &glyph);
	void SetFamily(QString family);
	void SetSize(qint32 size);
	void SetDirPath(QString dirpath);

private:
	QString Symbols;
	QString DirPath;
	QString FontName;

	qint32 FontSize;
	QString FontFamily;

	bool OptSize;

	QList<Font2CIndex> Index;
	QList<Font2CGlyph> Glyph;
};
//------------------------------------------------------------------------------
#endif // FONT2C_H
