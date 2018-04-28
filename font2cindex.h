#ifndef FONT2CINDEX_H
#define FONT2CINDEX_H
//------------------------------------------------------------------------------
#include <QtMath>
//------------------------------------------------------------------------------
class Font2CIndex
{
public:
	explicit Font2CIndex(quint32 fglyph,quint32 fchar, quint32 fcount);

	quint32 GetGlyph() const;
	quint32 GetChar() const;
	quint32 GetCount() const;

private:
	quint32 FGlyph;
	quint32 FChar;
	quint32 FCount;
};
//------------------------------------------------------------------------------
#endif // FONT2CINDEX_H
