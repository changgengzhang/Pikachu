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
	
	connect(ui.modelPolygonFaceBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonFaceBtnClicked(int)));
	connect(ui.modelPolygonLineBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonLineBtnClicked(int)));
	connect(ui.modelPolygonPointBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonPointBtnClicked(int)));
	
	connect(this, SIGNAL(setModelPolygonWay(zcg::MeshPolygonWay)), ui.renderView, SLOT(setModelPolygonWay(zcg::MeshPolygonWay)));
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


void Pikachu::onModelPolygonFaceBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		ui.modelPolygonLineBtn->setCheckState(Qt::Unchecked);
		ui.modelPolygonPointBtn->setCheckState(Qt::Unchecked);
		m_polygonWay = zcg::FILL;
	}
	else
	{
		m_polygonWay = zcg::NONE;
	}
	emit setModelPolygonWay(m_polygonWay);
}


void Pikachu::onModelPolygonLineBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		ui.modelPolygonFaceBtn->setCheckState(Qt::Unchecked);
		ui.modelPolygonPointBtn->setCheckState(Qt::Unchecked);
		m_polygonWay = zcg::LINE;
	}
	else
	{
		m_polygonWay = zcg::NONE;
	}
	emit setModelPolygonWay(m_polygonWay);
}


void Pikachu::onModelPolygonPointBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		ui.modelPolygonFaceBtn->setCheckState(Qt::Unchecked);
		ui.modelPolygonLineBtn->setCheckState(Qt::Unchecked);
		m_polygonWay = zcg::POINT;
	}
	else
	{
		m_polygonWay = zcg::NONE;
	}
	emit setModelPolygonWay(m_polygonWay);
}