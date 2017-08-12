#include "Pikachu.h"

Pikachu::Pikachu(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
}

Pikachu::~Pikachu()
{

}

// =========== helper function ============
void Pikachu::buildSlotsAndSignals()
{

}


// =================== slots ===============
void Pikachu::onModelLoadBtnClicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("get model file"), "F:\coding\Pikachu\Data\Model", tr("Model (*.ply *.obj)"));
	if (!filePath.isEmpty())
	{
		emit modleFile(filePath);
	}
}