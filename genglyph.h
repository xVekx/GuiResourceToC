#ifndef GENGLYPH_H
#define GENGLYPH_H
//------------------------------------------------------------------------------
#include <QWidget>
#include <ft2build.h>
#include FT_FREETYPE_H
//------------------------------------------------------------------------------
class GenGlyph : public QWidget
{
	Q_OBJECT
public:
	explicit GenGlyph(QWidget *parent = 0) : QWidget(parent) {}
	void paintEvent(QPaintEvent *e);

	static const char* FreeTypeErrorMessage(FT_Error err);
	static bool InitLib(FT_Library &lib,FT_Face &face,QString name);
	static void DeInitLib(FT_Library &lib,FT_Face &face);

	void SetFace(FT_Face f);
	void SetCharCode(quint32 cc);
	void SetSize(quint32 sz);
	void SetId(quint16 id);

	qint8 GetAsc()
	{
		return Ascender;
	}

	quint8 GetDesc()
	{
		return Descender;
	}

	quint32 GetCharCode() const;

	bool GlyphToBuffer();
	void UpdateImage();

	static void GenStructDefGlyph(QStringList &strlist);
	void GenStructGlyph(QStringList &strlist,QString tab = "\t\t");

	QString Info();

	void GenStructGlyphBuffer(QStringList &strlist, QString tab = "\t");
	void GenStructGlyphBufferOptSize(QStringList &strlist, quint8 c, QString tab = "\t");
private:
	QVector<quint8> Buffer;
	quint32 Size;
	qint8 Width;			//Ширина
	qint8 Row;				//Высота
	qint8 Left;				//Отступ слева
	qint8 Top;				//Отступ вверх от центральной линии
	qint8 AdvanceX;

	qint8 Ascender;
	qint8 Descender;
	quint16 Id;				//При генерации
	quint32	CharCode;
	FT_Face Face;
	QImage Image;
};
//------------------------------------------------------------------------------
#endif // GENGLYPH_H
