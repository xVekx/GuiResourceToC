#include <QPixmap>
#include <QPainter>
#include <QRgb>
#include <QDebug>
//------------------------------------------------------------------------------
#include "font2cglyph.h"
//------------------------------------------------------------------------------
#define QPRINT_VAL(__val) {qDebug()<<#__val<<":"<<__val;}
//------------------------------------------------------------------------------
Font2CGlyph::Font2CGlyph(QChar symbol, QFont font) :
Symbol(symbol),Font(font),FontMetrics(font)
{
	qDebug()<<"====================";
	/*QPRINT_VAL(FontMetrics.ascent());
	QPRINT_VAL(FontMetrics.descent() );*/
	QPRINT_VAL(FontMetrics.height() );
	//QPRINT_VAL(FontMetrics.leading() );
	//QPRINT_VAL(FontMetrics.lineSpacing() );
	QPRINT_VAL(FontMetrics.minLeftBearing() );
	QPRINT_VAL(FontMetrics.minRightBearing() );
	//QPRINT_VAL(FontMetrics.maxWidth() );
	QPRINT_VAL(FontMetrics.xHeight() );
	//QPRINT_VAL(FontMetrics.averageCharWidth() );


	//QPRINT_VAL(FontMetrics.leftBearing(symbol) );
	//QPRINT_VAL(FontMetrics.rightBearing(symbol) );
	//QPRINT_VAL(FontMetrics.width(symbol));
	/*QPRINT_VAL(FontMetrics.underlinePos());
	QPRINT_VAL(FontMetrics.overlinePos());
	QPRINT_VAL(FontMetrics.strikeOutPos());
	QPRINT_VAL(FontMetrics.lineWidth());*/
}

ushort Font2CGlyph::UnicodeSymbol() const {
	return Symbol.unicode();
}

int Font2CGlyph::Width() const {		//X
	return FontMetrics.width(Symbol);
}

int Font2CGlyph::Height() const {		//Y
	return FontMetrics.height();
}

int Font2CGlyph::Ascent() const {
	return FontMetrics.ascent();
}

int Font2CGlyph::Descent() const {
	return FontMetrics.descent();
}

QImage Font2CGlyph::GetImageGlyph() const {

	QPixmap map(FontMetrics.width(Symbol),FontMetrics.height());
	map.fill();

	QPainter painter;
	painter.begin(&map);
	painter.setFont(Font);
	painter.drawText(0,FontMetrics.height() - FontMetrics.descent(),Symbol);
	painter.end();
	return map.toImage();
}

int Font2CGlyph::XOffset() const {
	int ret = 0;
	const QImage img = GetImageGlyph();
	bool xoff_bool = false;
	for(int x=0;x<Width();x++) {
		for(int y=0;y<Height();y++)
			if(qGray(img.pixel(x,y)) != 0xFF) {
				xoff_bool = true;
				break;
			}
		if(xoff_bool) {
			ret = x;
			break;
		}
	}
	return ret;
}

int Font2CGlyph::XSize() const {
	int ret = Width() - XOffset();
	const QImage img = GetImageGlyph();
	bool xsize_bool = false;
	for(int x=0;x < Width(); x++ ) {
		for(int y=0;y<Height();y++)
			if(qGray(img.pixel(Width() - x - 1,y)) != 0xFF) {
				xsize_bool = true;
				break;
			}
		if(xsize_bool) {
			ret = (Width() - XOffset()) - x;
			break;
		}
	}
	return ret;
}

int Font2CGlyph::YOffset() const {
	int ret = 0;
	const QImage img = GetImageGlyph();
	bool yoff_bool = false;
	for(int y=0;y<Height();y++) {
		for(int x=0;x<Width();x++)
			if(qGray(img.pixel(x,y)) != 0xFF) {
				yoff_bool = true;
				break;
			}
		if(yoff_bool) {
			ret = y;
			break;
		}
	}
	return ret;
}

int Font2CGlyph::YSize() const {
	int ret = Height() - YOffset();
	const QImage img = GetImageGlyph();
	bool ysize_bool = false;
	for(int y=0;y < Height(); y++ ) {
		for(int x=0;x<Width();x++)
			if(qGray(img.pixel(x,Height() - y - 1)) != 0xFF) {
				ysize_bool = true;
				break;
			}
		if(ysize_bool) {
			ret = (Height() - YOffset()) - y;
			break;
		}
	}
	return ret;
}

void Font2CGlyph::TestSave() const
{
	const int xoff = XOffset();
	const int yoff = YOffset();
	const int xsz = XSize();
	const int ysz = YSize();

	QPRINT_VAL(xoff);
	QPRINT_VAL(yoff);
	QPRINT_VAL(xsz);
	QPRINT_VAL(ysz);


	const QImage img = GetImageGlyph();
	QPixmap map(xsz,ysz);
	QPainter painter;
	painter.begin(&map);
	painter.drawImage(-xoff,-yoff,img);
	painter.end();

	QImage img_save = map.toImage();
	img_save.save(QString("%1").arg(Symbol.unicode()) + QString(".jpg"));
}
