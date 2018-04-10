#include "genindex.h"
//------------------------------------------------------------------------------
GenIndex::GenIndex(quint16 gi, quint16 chi, quint16 coi):
	GlyphIndex(gi),CharIndex(chi),CountIndex(coi)
{

}
//------------------------------------------------------------------------------
void GenIndex::SearchRangIndex(QList<quint16> list, QList<GenIndex *> &rang)
{
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
		qDebug()<<"G:"<<i<<"\tC:"<<list.at(i)<<"\tCount:"<<z;
		rang<<new GenIndex(i,list.at(i),z);
		i = z + i - 1;
	}
}
//------------------------------------------------------------------------------
void GenIndex::GenStructDefIndex(QStringList &strlist){
	strlist<<"typedef struct {\n";
	strlist<<"\tconst uint16_t GlyphIndex;\n";
	strlist<<"\tconst uint16_t CharIndex;\n";
	strlist<<"\tconst uint16_t CountIndex;\n";
	strlist<<"} FONT_IndexTypeDef;\n";
}
//------------------------------------------------------------------------------
void GenIndex::GenStructIndex(QStringList &list, QString tab){
	list<<tab<<QString(".GlyphIndex\t= %1,\n").arg(GlyphIndex);
	list<<tab<<QString(".CharIndex\t= %1,\n").arg(CharIndex);
	list<<tab<<QString(".CountIndex\t= %1\n").arg(CountIndex);
}
//------------------------------------------------------------------------------
void GenIndex::SetGlyphIndex(quint16 i)
{
	GlyphIndex = i;
}
//------------------------------------------------------------------------------
void GenIndex::SetCharIndex(quint16 i)
{
	CharIndex = i;
}
//------------------------------------------------------------------------------
void GenIndex::SetCountIndex(quint16 i)
{
	CountIndex = i;
}
//------------------------------------------------------------------------------
quint16 GenIndex::GetGlyphIndex() const
{
	return GlyphIndex;
}
//------------------------------------------------------------------------------
quint16 GenIndex::GetCharIndex() const
{
	return CharIndex;
}
//------------------------------------------------------------------------------
quint16 GenIndex::GetCountIndex() const
{
	return CountIndex;
}
//------------------------------------------------------------------------------
QString GenIndex::Info()
{
	return QString("(gi:%1,ci:%2,ci:%3)")
			.arg(GlyphIndex).arg(CharIndex).arg(CountIndex);
}
//------------------------------------------------------------------------------
