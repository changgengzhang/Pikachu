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
	// model
	connect(m_ui.modelLoadBtn, SIGNAL(clicked()), this, SLOT(onModelLoadBtnClicked()));
	connect(m_ui.modelDelBtn, SIGNAL(clicked()), m_ui.renderView, SLOT(onModelDelBtnClicked()));
	
	// polygon type
	connect(m_ui.modelPolygonFaceBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonFaceBtnClicked(int)));
	connect(m_ui.modelPolygonLineBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonLineBtnClicked(int)));
	connect(m_ui.modelPolygonPointBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonPointBtnClicked(int)));

	// texture
	connect(m_ui.modelTextureSetBtn, SIGNAL(clicked()), this, SLOT(onModelTextureSetBtnClicked()));
	connect(m_ui.modelTextureDelBtn, SIGNAL(clicked()), m_ui.renderView, SLOT(onModelTextureDelBtnClicked()));

	// parameterization inner type
	connect(m_ui.modelTextureAverageBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelTextureAverageBtnClicked(int)));
	connect(m_ui.modelTextureShapPreserveBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelTextureShapPreserveBtnClicked(int)));

	// deliver data 
	connect(this, SIGNAL(deliverPolygonType(MeshPolygonType)), m_ui.renderView, SLOT(acceptPolygonType(MeshPolygonType)));
	connect(this, SIGNAL(deliverString(FileType, QString)), m_ui.renderView, SLOT(acceptString(FileType, QString)));
	connect(this, SIGNAL(deliverParameterizationInnerType(ParameterizationInnerType)), m_ui.renderView, SLOT(acceptParameterizationInnerType(ParameterizationInnerType)));
}


void Pikachu::initUi()
{
	m_ui.modelPolygonFaceBtn->setChecked(true);
}

// =================== slots ===============
void Pikachu::onModelLoadBtnClicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load model file"), QString("..\\Data\\Model"), tr("Model (*.obj)"));
	if (!fileName.isEmpty())
	{
		emit deliverString(FileType::MODEL, fileName);
	}
	// cancel load model
}


void Pikachu::onModelPolygonFaceBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelPolygonLineBtn->setCheckState(Qt::Unchecked);
		m_ui.modelPolygonPointBtn->setCheckState(Qt::Unchecked);
		emit deliverPolygonType(MeshPolygonType::FILL);
	}
	else
	{
		emit deliverPolygonType(MeshPolygonType::NONE_POLYGON_TYPE);
	}
}


void Pikachu::onModelPolygonLineBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelPolygonFaceBtn->setCheckState(Qt::Unchecked);
		m_ui.modelPolygonPointBtn->setCheckState(Qt::Unchecked);
		emit deliverPolygonType(MeshPolygonType::LINE);
	}
	else
	{
		emit deliverPolygonType(MeshPolygonType::NONE_POLYGON_TYPE);
	}
}


void Pikachu::onModelPolygonPointBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelPolygonFaceBtn->setCheckState(Qt::Unchecked);
		m_ui.modelPolygonLineBtn->setCheckState(Qt::Unchecked);
		emit deliverPolygonType(MeshPolygonType::POINT);
	}
	else
	{
		emit deliverPolygonType(MeshPolygonType::NONE_POLYGON_TYPE);
	}
}


void Pikachu::onModelTextureSetBtnClicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load texture file"), QString("..\\Data\\Image"), tr("Model (*.png *.jpg *.bmp)"));
	if (!fileName.isEmpty())
	{
		emit deliverString(FileType::TEXTURE, fileName);
	}
}

void Pikachu::onModelTextureAverageBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelTextureShapPreserveBtn->setCheckState(Qt::Unchecked);
		emit deliverParameterizationInnerType(ParameterizationInnerType::AVERAGE);
	}
	else
	{
		emit deliverParameterizationInnerType(ParameterizationInnerType::NONE_INNER_TYPE);
	}
}


void Pikachu::onModelTextureShapPreserveBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelTextureAverageBtn->setCheckState(Qt::Unchecked);
		emit deliverParameterizationInnerType(ParameterizationInnerType::SHAP_PRESERVING);
	}
	else
	{
		emit deliverParameterizationInnerType(ParameterizationInnerType::NONE_INNER_TYPE);
	}
}