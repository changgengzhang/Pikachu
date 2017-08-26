#ifndef PIKACHU_H
#define PIKACHU_H

#include <QtWidgets/QMainWindow>
#include <qfiledialog.h>

#include "ui_Pikachu.h"

class Pikachu : public QMainWindow
{
	Q_OBJECT

public:
	Pikachu(QWidget *parent = Q_NULLPTR);
	~Pikachu();

private:
	Ui::PikachuClass m_ui;

	// =========== helper function ==========
	void buildSlotsAndSignals();
	void initUi();


	// =========== signals ==========
signals:
	void deliverString(FileType fileType, QString fileName);
	void deliverPolygonType(MeshPolygonType polygonWay);
	void deliverParameterizationInnerType(ParameterizationInnerType innerType);


	// ============ slots ===========
private slots:
	void onModelLoadBtnClicked();

	void onModelPolygonFaceBtnClicked(int state);
	void onModelPolygonLineBtnClicked(int state);
	void onModelPolygonPointBtnClicked(int state);
	
	void onModelTextureSetBtnClicked();
	void onModelTextureAverageBtnClicked(int state);
	void onModelTextureShapPreserveBtnClicked(int state);

};

#endif // !PIKACHU_H