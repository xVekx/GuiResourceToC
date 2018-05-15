#include "fontselect.h"
#include "ui_fontselect.h"
//------------------------------------------------------------------------------
#include <QDebug>
#include <QFile>
#include <QFontDatabase>
//------------------------------------------------------------------------------
FontSelect::FontSelect(QWidget *parent) :
QDialog(parent),
ui(new Ui::FontSelect)
{
	ui->setupUi(this);

	qDebug()<<Q_FUNC_INFO;

	QFile loadFile("conf.json");

	QByteArray JsonData;

	if (loadFile.open(QIODevice::ReadOnly)) {
		JsonData = loadFile.readAll();
		loadFile.close();
	} else {
		JsonData.clear();
		qWarning("Couldn't open save file.");
	}

	QJsonDocument JsonDoc(QJsonDocument::fromJson(JsonData));

	JsonArrayFonts = JsonDoc.object()["Font"].toArray();

	for(int i=0;i<JsonArrayFonts.size();i++) {
		QString name_item = JsonArrayFonts.at(i).toObject()["Name"].toString();
		ui->QLWFontName->addItem(name_item);
	}

	QFontDatabase qfd;
	QStringList list_fonts = qfd.families();
	ui->QLWFamily->addItems(list_fonts);

	int qlw_font_name_count = ui->QLWFontName->count();
	if(qlw_font_name_count > 0) {
		ui->QLWFontName->setCurrentRow(qlw_font_name_count - 1);
	}

	//ui->QTWMain->setCurrentIndex(0);

	//ui->QLEFamily->setEnabled(false);
}
//------------------------------------------------------------------------------
FontSelect::~FontSelect()
{

	QFile file("conf.json");

	if (!file.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open save file.");
	}

	QJsonDocument JsonDoc;
	QJsonObject JsonObj;

	JsonObj["Font"] = JsonArrayFonts;
	JsonDoc.setObject(JsonObj);
	file.write(JsonDoc.toJson());

	file.close();

	qDebug()<<Q_FUNC_INFO;
	delete ui;
}


//------------------------------------------------------------------------------
void FontSelect::on_QPBFontAdd_clicked()
{
	static int test = 0;
	QString name_item = QString("NewFont%1").arg(test++);

	int curr_row = ui->QLWFontName->currentRow();

	if(curr_row < 0) {
		curr_row = 0;
	} else {
		curr_row++;
	}

	ui->QLWFontName->insertItem(curr_row,name_item);

	QJsonObject json_obj;
	json_obj["Name"] = name_item;
	json_obj["Family"] = "DejaVu Sans Mono";
	json_obj["Size"] = 10;
	JsonArrayFonts.insert(curr_row,json_obj);

	ui->QLWFontName->setCurrentRow(curr_row);

	//TODO добавить поиск дубликата
}
//------------------------------------------------------------------------------
void FontSelect::on_QPBFontDelete_clicked()
{
	int curr = ui->QLWFontName->currentRow();
	if(curr < 0) {
		return;
	}

	QListWidgetItem *it = ui->QLWFontName->takeItem(curr);
	delete it;

	JsonArrayFonts.removeAt(curr);
}
//------------------------------------------------------------------------------
void FontSelect::on_QPBFontRename_clicked()
{
	int curr = ui->QLWFontName->currentRow();
	if(curr < 0) {
		return;
	}

	QString str = ui->QLEFontName->text();
	ui->QLWFontName->item(curr)->setText(str);
	//TODO добавить поиск дубликата
}



//------------------------------------------------------------------------------
void FontSelect::on_QLWFontName_currentRowChanged(int currentRow)
{
	qDebug()<<Q_FUNC_INFO<<currentRow;
	if(currentRow > -1) {
		QJsonObject json_obj = JsonArrayFonts.at(currentRow).toObject();

		int size = json_obj["Size"].toInt();
		ui->QSBSize->setValue(size);

		QString family = json_obj["Family"].toString();
		ui->QLEFamily->setText(family);

		QList<QListWidgetItem *> list_family = ui->QLWFamily->findItems(family,Qt::MatchExactly);
		if(list_family.size() != 0 ) {
			ui->QLWFamily->setCurrentItem(list_family.first());
		}

		QString style = json_obj["Style"].toString();
		QList<QListWidgetItem *> list_style = ui->QLWStyles->findItems(style,Qt::MatchExactly);
		if(list_style.size() != 0 ) {
			ui->QLWStyles->setCurrentItem(list_style.first());
		} else {
			ui->QLWStyles->setCurrentRow(0);
		}

	}
}
//------------------------------------------------------------------------------
void FontSelect::on_QLWFontName_currentTextChanged(const QString &currentText)
{
	qDebug()<<Q_FUNC_INFO<<currentText;
	ui->QLEFontName->setText(currentText);
}
//------------------------------------------------------------------------------
void FontSelect::on_QLWFamily_currentTextChanged(const QString &currentText)
{
	qDebug()<<Q_FUNC_INFO<<currentText;
	ui->QLEFamily->setText(currentText);

	int row = ui->QLWFontName->currentRow();
	if(row > -1) {

		QJsonObject json_obj = JsonArrayFonts.at(row).toObject();

		json_obj["Family"] = currentText;

		JsonArrayFonts.removeAt(row);
		JsonArrayFonts.insert(row,json_obj);

		QFontDatabase qfd;

		QStringList list_styles = qfd.styles(currentText);

		ui->QLWStyles->clear();
		ui->QLWStyles->addItems(list_styles);

				QString style = json_obj["Style"].toString();
		QList<QListWidgetItem *> list_style = ui->QLWStyles->findItems(style,Qt::MatchExactly);
		if(list_style.size() != 0 ) {
			ui->QLWStyles->setCurrentItem(list_style.first());
		} else {
			ui->QLWStyles->setCurrentRow(0);
		}



		//

		//qDebug()<<JsonArrayFonts;
	}
}
//------------------------------------------------------------------------------
void FontSelect::on_QLWStyles_currentTextChanged(const QString &currentText)
{
	qDebug()<<Q_FUNC_INFO<<currentText;

	int row = ui->QLWFontName->currentRow();
	if(row > -1) {

		QJsonObject json_obj = JsonArrayFonts.at(row).toObject();

		json_obj["Style"] = currentText;

		JsonArrayFonts.removeAt(row);
		JsonArrayFonts.insert(row,json_obj);
	}
}
//------------------------------------------------------------------------------
void FontSelect::on_QSBSize_valueChanged(int arg1)
{
	qDebug()<<Q_FUNC_INFO<<arg1;
		int row = ui->QLWFontName->currentRow();
	if(row > -1) {

		QJsonObject json_obj = JsonArrayFonts.at(row).toObject();

		json_obj["Size"] = arg1;

		JsonArrayFonts.removeAt(row);
		JsonArrayFonts.insert(row,json_obj);
	}
}
//------------------------------------------------------------------------------
