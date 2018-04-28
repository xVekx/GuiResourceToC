#ifndef MW_H
#define MW_H
//------------------------------------------------------------------------------
#include <QMainWindow>
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
	void paintEvent(QPaintEvent *e);
	void DrawGlyph(const QFont &f, unsigned int gi, QImage &img);

	void TestAlg(QChar ch, int pointSize = -1, QString file= "test");
	void TestSketch();
private slots:
	void on_QPBUp_clicked();
	void on_QPBDown_clicked();
	void on_QPBTest_clicked();
	void on_QPBTest2_clicked();

private:
	Ui::MW *ui;
};
//------------------------------------------------------------------------------
#endif // MW_H
