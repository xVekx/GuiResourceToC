#ifndef MW_H
#define MW_H
//------------------------------------------------------------------------------
#include <QMainWindow>
#include "fontselect.h"
//------------------------------------------------------------------------------
namespace Ui {
class MW;
}
//------------------------------------------------------------------------------
class MW : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MW(QWidget *parent = 0);
	~MW();

private slots:

	void on_QPBFont2C_clicked();
	void on_QPBFont2CPath_clicked();
	void on_QPBImage2C_clicked();
	void on_QPBAll2C_clicked();
	void on_QPBImage2CPath_clicked();
	void on_QPBImage2CSetting_clicked();

	void on_QPBFont2CSetting_clicked();

private:
	Ui::MW *ui;

};
//------------------------------------------------------------------------------
#endif // MW_H
