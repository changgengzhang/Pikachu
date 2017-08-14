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
	Ui::PikachuClass ui;

	// =========== helper function ==========
	void buildSlotsAndSignals();

	// =========== signals ==========
signals:
	void getModelFilePath(QString filePath);

	// ============ slots ===========
private slots:
	void onModelLoadBtnClicked();
	

};

#endif // !PIKACHU_H