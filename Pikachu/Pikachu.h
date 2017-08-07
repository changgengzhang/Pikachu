#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Pikachu.h"

class Pikachu : public QMainWindow
{
	Q_OBJECT

public:
	Pikachu(QWidget *parent = Q_NULLPTR);

private:
	Ui::PikachuClass ui;

};
