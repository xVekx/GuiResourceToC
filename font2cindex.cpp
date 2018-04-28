#include "font2cindex.h"
//------------------------------------------------------------------------------
Font2CIndex::Font2CIndex(quint32 fglyph, quint32 fchar, quint32 fcount) :
FGlyph(fglyph),FChar(fchar),FCount(fcount)
{
}
//------------------------------------------------------------------------------
quint32 Font2CIndex::GetGlyph() const
{
	return FGlyph;
}
//------------------------------------------------------------------------------
quint32 Font2CIndex::GetChar() const
{
	return FChar;
}
//------------------------------------------------------------------------------
quint32 Font2CIndex::GetCount() const
{
	return FCount;
}
//------------------------------------------------------------------------------
