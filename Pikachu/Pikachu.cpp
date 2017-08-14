#include "Pikachu.h"

Pikachu::Pikachu(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	this->buildSlotsAndSignals();
}

Pikachu::~Pikachu()
{

}

// =========== helper function ============
void Pikachu::buildSlotsAndSignals()
{
	connect(ui.modelLoadBtn, SIGNAL(clicked()), this, SLOT(onModelLoadBtnClicked()));
	connect(this, SIGNAL(getModelFilePath(QString)), ui.renderView, SLOT(getModelFilePath(QString)));
	connect(ui.modelDelBtn, SIGNAL(clicked()), ui.renderView, SLOT(onModelDelBtnClicked()));
	
}


// =================== slots ===============
void Pikachu::onModelLoadBtnClicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Load model file"), "F:\\coding\\Pikachu\\Data\\Model", tr("Model (*.obj)"));
	if (!filePath.isEmpty())
	{
		emit getModelFilePath(filePath);
	}
	// cancel load model
}