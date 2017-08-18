#include "Pikachu.h"

Pikachu::Pikachu(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	this->buildSlotsAndSignals();
	this->initUi();
}

Pikachu::~Pikachu()
{

}

// =========== helper function ============
void Pikachu::buildSlotsAndSignals()
{
	connect(m_ui.modelLoadBtn, SIGNAL(clicked()), this, SLOT(onModelLoadBtnClicked()));
	connect(this, SIGNAL(deliverString(QString)), m_ui.renderView, SLOT(acceptFilePath(QString)));
	connect(m_ui.modelDelBtn, SIGNAL(clicked()), m_ui.renderView, SLOT(onModelDelBtnClicked()));
	
	connect(m_ui.modelPolygonFaceBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonFaceBtnClicked(int)));
	connect(m_ui.modelPolygonLineBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonLineBtnClicked(int)));
	connect(m_ui.modelPolygonPointBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonPointBtnClicked(int)));
	
	connect(this, SIGNAL(deliverPolygonType(zcg::MeshPolygonWay)), m_ui.renderView, SLOT(acceptPolygonType(zcg::MeshPolygonWay)));
}


void Pikachu::initUi()
{
	m_ui.modelPolygonFaceBtn->setChecked(true);
}

// =================== slots ===============
void Pikachu::onModelLoadBtnClicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Load model file"), QString("..\\Data\\Model"), tr("Model (*.obj)"));
	if (!filePath.isEmpty())
	{
		emit deliverString(filePath);
	}
	// cancel load model
}


void Pikachu::onModelPolygonFaceBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelPolygonLineBtn->setCheckState(Qt::Unchecked);
		m_ui.modelPolygonPointBtn->setCheckState(Qt::Unchecked);
		m_polygonWay = zcg::FILL;
	}
	else
	{
		m_polygonWay = zcg::NONE;
	}
	emit deliverPolygonType(m_polygonWay);
}


void Pikachu::onModelPolygonLineBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelPolygonFaceBtn->setCheckState(Qt::Unchecked);
		m_ui.modelPolygonPointBtn->setCheckState(Qt::Unchecked);
		m_polygonWay = zcg::LINE;
	}
	else
	{
		m_polygonWay = zcg::NONE;
	}
	emit deliverPolygonType(m_polygonWay);
}


void Pikachu::onModelPolygonPointBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelPolygonFaceBtn->setCheckState(Qt::Unchecked);
		m_ui.modelPolygonLineBtn->setCheckState(Qt::Unchecked);
		m_polygonWay = zcg::POINT;
	}
	else
	{
		m_polygonWay = zcg::NONE;
	}
	emit deliverPolygonType(m_polygonWay);
}