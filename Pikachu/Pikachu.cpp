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
	connect(m_ui.modelTextureuUniformBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelTextureUniformBtnClicked(int)));
	connect(m_ui.modelTextureShapPreserveBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelTextureShapPreserveBtnClicked(int)));

	// deliver data 
	connect(this, SIGNAL(deliverPolygonType(ZVALUE)), m_ui.renderView, SLOT(acceptPolygonType(ZVALUE)));
	connect(this, SIGNAL(deliverString(FileType, QString)), m_ui.renderView, SLOT(acceptString(FileType, QString)));
	connect(this, SIGNAL(deliverParameterizationInnerType(ZVALUE)), m_ui.renderView, SLOT(acceptParameterizationInnerType(ZVALUE)));


	// 
	//connect(m_ui.renderTypeTab, SIGNAL(currentChanged(int)), m_ui.renderView, SLOT());
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
		emit deliverPolygonType(Z_FILL);
	}
	else
	{
		emit deliverPolygonType(Z_NONE);
	}
}


void Pikachu::onModelPolygonLineBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelPolygonFaceBtn->setCheckState(Qt::Unchecked);
		m_ui.modelPolygonPointBtn->setCheckState(Qt::Unchecked);
		emit deliverPolygonType(Z_LINE);
	}
	else
	{
		emit deliverPolygonType(Z_NONE);
	}
}


void Pikachu::onModelPolygonPointBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelPolygonFaceBtn->setCheckState(Qt::Unchecked);
		m_ui.modelPolygonLineBtn->setCheckState(Qt::Unchecked);
		emit deliverPolygonType(Z_POINT);
	}
	else
	{
		emit deliverPolygonType(Z_NONE);
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

void Pikachu::onModelTextureUniformBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelTextureShapPreserveBtn->setCheckState(Qt::Unchecked);
		emit deliverParameterizationInnerType(Z_UNIFORM);
	}
	else
	{
		emit deliverParameterizationInnerType(Z_NONE);
	}
}


void Pikachu::onModelTextureShapPreserveBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelTextureuUniformBtn->setCheckState(Qt::Unchecked);
		emit deliverParameterizationInnerType(Z_SHAP_PRESERVING);
	}
	else
	{
		emit deliverParameterizationInnerType(Z_NONE);
	}
}

