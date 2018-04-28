#include "fontdef.h"
//------------------------------------------------------------------------------
#define BIT(B)					(1u<<B)// преобр. номера бита в позицию в байте
#define TBIT0(ADDRESS,B)		(!(ADDRESS & (BIT(B))))// проверка бита на ноль
#define TBIT1(ADDRESS,B)		(ADDRESS & (BIT(B)))// проверка бита на единицу
#define SBIT(ADDRESS,B)			(ADDRESS |= BIT(B))// установка бита
#define CBIT(ADDRESS,B)			(ADDRESS &=~BIT(B))// сброс бита
#define SMASK(ADDRESS,BYTE)		(ADDRESS |= BYTE)// установка битов по байтовой маске
#define CMASK(ADDRESS,BYTE)		(ADDRESS &= ~(BYTE))//очистка битов по байтовой маске
//------------------------------------------------------------------------------
/*
0xxxxxxx	0x80
10xxxxxx
110xxxxx	0xE0	0xC0
1110xxxx	0xF0	0xE0
11110xxx	0xF8	0xF0
111110xx			0xF8
1111110x			0xFC
*/
//------------------------------------------------------------------------------
int FONT_CharToGlyphIndex(const FONT_IndexTypeDef *Index,int Sz,int Unc)
{
	int ret = -1;
	for(int i = 0; i< Sz ;i++) {
		if(	(Index[i].CharIndex <= Unc) &&
			(Unc < Index[i].CharIndex + Index[i].CountIndex) ) {
			ret =  (Unc - Index[i].CharIndex) + Index[i].GlyphIndex ;
			break;
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
int FONT_CharUtf8ToUnc(char *text,int *size)
{
	int ret = -1;

	if(TBIT0(text[0],7)) {
		ret = text[0];
		if(size != NULL) {
			*size += 1;
		}
	} else if(	TBIT1(text[0],7) &&
				TBIT1(text[0],6) &&
				TBIT0(text[0],5)) {
		uint8_t u0 = text[0];
		uint8_t u1 = text[1];

		CMASK(u0,(BIT(7)|BIT(6)));
		CMASK(u1,(BIT(7)));
		ret = 0;
		SMASK(ret,(u0 << 6u));
		SMASK(ret,u1);
		if(size != NULL) {
			*size += 2;
		}
	} else if(	TBIT1(text[0],7) &&
				TBIT1(text[0],6) &&
				TBIT1(text[0],5) &&
				TBIT0(text[0],4)) {

		uint8_t u0 = text[0];
		uint8_t u1 = text[1];
		uint8_t u2 = text[2];

		CMASK(u0,(BIT(7)|BIT(6)|BIT(5)));
		CMASK(u1,(BIT(7)));
		CMASK(u2,(BIT(7)));

		ret = 0;

		SMASK(ret,(u0 << 12u));
		SMASK(ret,(u1 << 6u));
		SMASK(ret,u2);

		if(size != NULL) {
			*size += 3;
		}
	} else if(	TBIT1(text[0],7) &&
				TBIT1(text[0],6) &&
				TBIT1(text[0],5) &&
				TBIT1(text[0],4) &&
				TBIT0(text[0],3)) {
		ret = -1;

		if(size != NULL) {
			*size += 4;
		}
	}

#if 0
	PrintHexString(text,*size);
	for(int i=0;i<*size;i++) {
		PrintBitStr(text[i]);
	}
	printf("unc id:%i\n",ret);
#endif

	if(ret < 0) {
		printf("!!!FONT_CharUtf8ToUnc NOT CONV\n");
	}

	return ret;
}
//------------------------------------------------------------------------------
