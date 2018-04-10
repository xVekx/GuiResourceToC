#ifndef GENINDEX_H
#define GENINDEX_H
//------------------------------------------------------------------------------
#include <QObject>
#include <QDataStream>
#include <QDebug>
//------------------------------------------------------------------------------
class GenIndex
{
public:
	GenIndex(quint16 gi,quint16 chi,quint16 coi);
	static void SearchRangIndex(QList<quint16> list, QList<GenIndex *> &rang);
	static void GenStructDefIndex(QStringList &strlist);
	void GenStructIndex(QStringList &list, QString tab = "\t\t");
	void SetGlyphIndex(quint16 i);
	void SetCharIndex(quint16 i);
	void SetCountIndex(quint16 i);
	quint16 GetGlyphIndex() const;
	quint16 GetCharIndex() const;
	quint16 GetCountIndex() const;
	QString Info();
private:
	quint16 GlyphIndex;
	quint16 CharIndex;
	quint16 CountIndex;
};
//------------------------------------------------------------------------------
#endif // GENINDEX_H
