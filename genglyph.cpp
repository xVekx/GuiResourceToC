#include "genglyph.h"
//------------------------------------------------------------------------------
#include <QPainter>
#include <QDebug>
//------------------------------------------------------------------------------
#include FT_OUTLINE_H
#include FT_RENDER_H
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include "math.h"
//------------------------------------------------------------------------------
#define ABS(X)	((X) > 0 ? (X) : -(X))
//------------------------------------------------------------------------------
void GenGlyph::paintEvent(QPaintEvent *e)
{
	QWidget::paintEvent(e);
	QPainter painter(this);
	if(!Image.isNull())
		painter.drawImage(0,0, Image.scaled(this->size()));
}
//------------------------------------------------------------------------------
const char *GenGlyph::FreeTypeErrorMessage(FT_Error err)
{
#undef __FTERRORS_H__
#define FT_ERRORDEF(e,v,s)		case e: return s;
#define FT_ERROR_START_LIST		switch (err) {
#define FT_ERROR_END_LIST		}
#include FT_ERRORS_H
	return "(Unknown error)";
}
//------------------------------------------------------------------------------
bool GenGlyph::InitLib(FT_Library &lib, FT_Face &face, QString name)
{
	FT_Error error = FT_Err_Ok;
	error = FT_Init_FreeType( &lib );
	if(error) {
		qDebug()<<"FT_Load_Glyph:"<<error<<FreeTypeErrorMessage(error);
		return false;
	}
	error = FT_New_Face(lib,name.toUtf8(),0,&face);
	if(error) {
		qDebug()<<"FT_Load_Glyph:"<<error<<FreeTypeErrorMessage(error);
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------
void GenGlyph::DeInitLib(FT_Library &lib, FT_Face &face)
{
	FT_Done_Face(face);
	FT_Done_FreeType(lib);
}
//------------------------------------------------------------------------------
void GenGlyph::SetFace(FT_Face f)
{
	Face = f;
}
//------------------------------------------------------------------------------
void GenGlyph::SetCharCode(quint32 cc)
{
	CharCode = cc;
}
//------------------------------------------------------------------------------
void GenGlyph::SetSize(quint32 sz) {
	Size = sz;
}
//------------------------------------------------------------------------------
void GenGlyph::SetId(quint16 id)
{
	Id = id;
}
//------------------------------------------------------------------------------
quint32 GenGlyph::GetCharCode() const
{
	return CharCode;
}
//------------------------------------------------------------------------------
double int26p6_to_double(int p)
{
	const int div = 1 << 6;
	return double(p) / div;
}

double int16p16_to_double(int p)
{
	const int div = 1 << 16;
	return double(p) / div;
}

bool GenGlyph::GlyphToBuffer()
{
	Buffer.clear();
	FT_Error error = FT_Err_Ok;
	error = FT_Set_Pixel_Sizes(Face,0,Size);

//	/*error = FT_Set_Char_Size(
//			  Face,    /* handle to face object           */
//			  0,       /* char_width in 1/64th of points  */
//			  Size*64,   /* char_height in 1/64th of points */
//			  300,     /* horizontal device resolution    */
//			  300 );   /* vertical device resolution      */*/


	if(error) {
		qDebug()<<"FT_Set_Pixel_Sizes:"<<error<<FreeTypeErrorMessage(error);
		return false;
	}

	FT_UInt index = FT_Get_Char_Index(Face,CharCode);
	error = FT_Load_Glyph(Face,index,FT_LOAD_DEFAULT | FT_LOAD_RENDER);
	if(error) {
		qDebug()<<"FT_Load_Glyph:"<<error<<FreeTypeErrorMessage(error);
		return false;
	}

	if(CharCode == 0x20) {
		qDebug()<<"!!!BS index:"<<index;
	}

	error = FT_Render_Glyph(Face->glyph,FT_RENDER_MODE_LIGHT);
	if(error) {
		qDebug()<<"FT_Render_Glyph:"<<error<<FreeTypeErrorMessage(error);
		return false;
	}

	quint8 alfa = 0x00;

	Width	= Face->glyph->bitmap.width;

	//qDebug()<<Face->glyph->bitmap.pitch;
	Row		= Face->glyph->bitmap.rows;
	Left	= Face->glyph->bitmap_left;
	Top		= Face->glyph->bitmap_top;
	AdvanceX = Face->glyph->advance.x>>6;
	//qDebug()<<"Face->glyph->advance.x<<6:"<<();

		//qDebug()<<((Face->ascender>>6))<<((Face->descender>>6))<<(Face->ascender>>6)-(Face->descender>>6);
		//qDebug()<<(Face->ascender)<<((Face->descender))<<(Face->ascender)-(Face->descender);

		FT_Fixed scale = Face->size->metrics.y_scale;

//		qDebug()<<int26p6_to_double(scale);
//#define 	FT_FLOOR(X)   ((X & -64) / 64)
//#define 	FT_CEIL(X)   (((X + 63) & -64) / 64)
//		qDebug()<<FT_CEIL(FT_MulFix(Face->ascender, scale));
//		qDebug()<<FT_CEIL(FT_MulFix(Face->descender,scale));

		double a,d;

		a = int26p6_to_double(FT_MulFix(Face->ascender, scale));
		d = int26p6_to_double(FT_MulFix(Face->descender,scale));


		Ascender = ::round(ABS(a));
		Descender = ::round(ABS(d));




	   // font->ascent  = FT_CEIL(FT_MulFix(face->ascender, scale));
	   // font->descent = FT_CEIL(FT_MulFix(face->descender, scale));

		//qDebug()<<(Face->max_advance_height>>6);
		//qDebug()<<(Face->size->metrics.height>>6);
		//qDebug()<<(Face->glyph->face->max_advance_height);

	/*qDebug()<<Face->height;

	qDebug()<<Face->max_advance_width;
	qDebug()<<Face->max_advance_height;

   qDebug()<<Face->underline_position;
	qDebug()<<Face->underline_thickness;*/

	if(CharCode == 0x20) {
		Width = Size>>2;
		Row = 1;
		for(int w=0;w<Width;w++) {
			Buffer<<alfa;
		}
	} else {


	for(int r=0;r<Row;r++)
		for(int w=0;w<Width;w++) {
			alfa=Face->glyph->bitmap.buffer[r * Width + w];
			Buffer<<alfa;
		}

		}

	if(CharCode == 0x20) {
		qDebug()<<Buffer;
	}
	return true;
}
//------------------------------------------------------------------------------
void GenGlyph::UpdateImage()
{
	Image = QImage(Width,Row,QImage::Format_ARGB32);
	for(int r=0;r<Row;r++)
		for(int w=0;w<Width;w++) {
			quint8 z=Buffer.at(r * Width + w);
			Image.setPixel(w,r,z<<24|0xF0<<16|0x00<<8|0xF0);
		}
}
//------------------------------------------------------------------------------
void GenGlyph::GenStructDefGlyph(QStringList &strlist)
{
	strlist<<"typedef struct {\n";
	strlist<<"\tconst uint8_t *buffer;\n";
	strlist<<"\tconst int8_t width;\t//Ширина\n";
	strlist<<"\tconst int8_t row;\t//Высота\n";
	strlist<<"\tconst int8_t left;\t//Отступ слева\n";
	strlist<<"\tconst int8_t top;\t//Отступ вверх от центральной линии\n";
	strlist<<"\tconst int8_t advance_x;\n";
	strlist<<"} FONT_GlyphTypeDef;\n";
}
//------------------------------------------------------------------------------
void GenGlyph::GenStructGlyph(QStringList &strlist, QString tab)
{
	strlist<<tab<<QString(".width\t= %1,\n").arg(Width);
	strlist<<tab<<QString(".row\t= %1,\n")	.arg(Row);
	strlist<<tab<<QString(".left\t= %1,\n")	.arg(Left);
	strlist<<tab<<QString(".top\t= %1,\n")	.arg(Top);
	strlist<<tab<<QString(".advance_x\t= %1,\n")	.arg(AdvanceX);
	strlist<<tab<<QString(".buffer\t= FONT_Buffer_%1\n").arg(CharCode);
}
//------------------------------------------------------------------------------
void GenGlyph::GenStructGlyphBuffer(QStringList &strlist,QString tab)
{
	QString strhex;
	for(int r=0;r<Row;r++) {
		strhex += tab + "\t";
		for(int w=0;w<Width;w++) {
			quint8 i=Buffer.at(r*Width + w);
			if((r+1 == Row) && (w+1==Width)) {
				strhex += QString(" 0x%1").arg(i, 2, 16, QLatin1Char( '0' ));
			} else {
				strhex += QString(" 0x%1,").arg(i, 2, 16, QLatin1Char( '0' ));
			}
		}
		strhex += "\n";
	}
	strlist<<strhex;
}
//------------------------------------------------------------------------------
void GenGlyph::GenStructGlyphBufferOptSize(QStringList &strlist, quint8 c, QString tab)
{
	QString strhex;
	for(int r=0;r<Row;r++) {
		int WidthH = Width/8;

		strhex += tab + "\t";

		int wt=0;

		do {
			//qDebug()<<WidthH<<Width;
			quint8 hex=0;



			for(int bit=0;bit<8;bit++) {
				if(!((wt*8+bit) < Width))break;

				quint8 g_hex=Buffer.at(r*Width + (wt*8+bit)/*w*/ );
				//qDebug()<<(wt*8+bit);



				if(g_hex > c) {
					hex |= (1u<<bit);
				}


			}

			strhex += QString(" 0x%1,").arg(hex, 2, 16, QLatin1Char( '0' ));

			wt++;


		} while (WidthH-- > 0);

		//qDebug()<<"rrr"<<r;

		/*for(int i=0;i<1;i++)
		{
			qDebug()<<i;
		}*/

		strhex += "\n";

		//CMASK(WidthH,0x08);
		//qDebug()<<WidthH<<Width;


		/*for(int w=0;w<Width;w++) {
			quint8 i=Buffer.at(r*Width + w);
		}*/
	}

	strlist<<strhex;
}
//------------------------------------------------------------------------------
QString GenGlyph::Info()
{
	QString msg = QString("(sz:%1,w:%2,r:%3,l:%4,t:%5,id:%6,cc:%7,asc:%8,desc:%9)")
			.arg(Size).arg(Width).arg(Row).arg(Left).arg(Top).arg(Id).arg(CharCode).arg(Ascender).arg(Descender);
	return msg;
}
//------------------------------------------------------------------------------
