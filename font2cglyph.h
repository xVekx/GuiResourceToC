#ifndef FONT2CGLYPH_H
#define FONT2CGLYPH_H
//------------------------------------------------------------------------------
#include <QFont>
#include <QFontMetrics>
#include <QImage>
//------------------------------------------------------------------------------
class Font2CGlyph
{
public:
	explicit Font2CGlyph(QChar symbol,QFont font);
	ushort UnicodeSymbol() const;
	int Width () const;
	int Height() const;
	int Ascent() const;
	int Descent() const;
	QImage GetImageGlyph() const;
	int XOffset() const;
	int XSize() const;
	int YOffset() const;
	int YSize() const;
	void TestSave() const;

private:
	QChar Symbol;
	QFont Font;
	QFontMetrics FontMetrics;
};
//------------------------------------------------------------------------------
#endif // FONT2CGLYPH_H
