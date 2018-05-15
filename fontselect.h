#ifndef FONTSELECT_H
#define FONTSELECT_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class FontSelect;
}

class FontSelect : public QDialog
{
	Q_OBJECT

public:
	explicit FontSelect(QWidget *parent = 0);
	~FontSelect();

private slots:

	void on_QPBFontAdd_clicked();

	void on_QPBFontDelete_clicked();

	void on_QPBFontRename_clicked();

	void on_QLWFontName_currentRowChanged(int currentRow);

	void on_QLWFontName_currentTextChanged(const QString &currentText);

	void on_QLWFamily_currentTextChanged(const QString &currentText);


	void on_QLWStyles_currentTextChanged(const QString &currentText);

	void on_QSBSize_valueChanged(int arg1);

private:
	Ui::FontSelect *ui;
	//QByteArray JsonData;
	//QJsonDocument JsonDoc;
	//QJsonObject JsonFont;
	QJsonArray JsonArrayFonts;
};

#endif // FONTSELECT_H
