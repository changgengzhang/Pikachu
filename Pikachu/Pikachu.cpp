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
	//connect(this, SIGNAL(getModelFilePath(QString filePath)), ui.renderView, SLOT(getModelFilePath(QString filePath)));
	connect(this, &Pikachu::getModelFilePath, ui.renderView, &RenderView::getModelFilePath);
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

void Pikachu::onModelDelBtnClicked()
{

}