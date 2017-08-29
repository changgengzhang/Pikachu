#include "Pikachu.h"

Pikachu::Pikachu(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	this->initUi();
	this->buildSlotsAndSignals();	

	m_ui.modelPolygonFaceBtn->setChecked(true);
}

Pikachu::~Pikachu()
{
	
}

// =========== helper function ============
void Pikachu::buildSlotsAndSignals()
{
	// model
	connect(m_ui.modelLoadBtn, SIGNAL(clicked()), this, SLOT(onModelLoadBtnClicked()));
	connect(m_ui.modelDelBtn, SIGNAL(clicked()), m_renderView, SLOT(onModelDelBtnClicked()));
	
	// polygon type
	connect(m_ui.modelPolygonFaceBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonFaceBtnClicked(int)));
	connect(m_ui.modelPolygonLineBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonLineBtnClicked(int)));
	connect(m_ui.modelPolygonPointBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelPolygonPointBtnClicked(int)));

	// texture
	connect(m_ui.modelTextureSetBtn, SIGNAL(clicked()), this, SLOT(onModelTextureSetBtnClicked()));
	connect(m_ui.modelTextureDelBtn, SIGNAL(clicked()), m_renderView, SLOT(onModelTextureDelBtnClicked()));

	// parameterization inner type
	connect(m_ui.modelTextureAverageBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelTextureAverageBtnClicked(int)));
	connect(m_ui.modelTextureShapPreserveBtn, SIGNAL(stateChanged(int)), this, SLOT(onModelTextureShapPreserveBtnClicked(int)));

	// deliver data 
	connect(this, SIGNAL(deliverPolygonType(ZVALUE)), m_renderView, SLOT(acceptPolygonType(ZVALUE)));
	connect(this, SIGNAL(deliverString(FileType, QString)), m_renderView, SLOT(acceptString(FileType, QString)));
	connect(this, SIGNAL(deliverParameterizationInnerType(ZVALUE)), m_renderView, SLOT(acceptParameterizationInnerType(ZVALUE)));
}


void Pikachu::initUi()
{
	m_renderView = new RenderView(m_ui.centralWidget);
	m_renderView->setObjectName(QStringLiteral("renderView"));
	m_renderView->setGeometry(QRect(10, 10, 1000, 900));
	m_renderView->setMinimumSize(QSize(1000, 900));
	m_renderView->setMaximumSize(QSize(1000, 900));
	m_renderView->show();
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

void Pikachu::onModelTextureAverageBtnClicked(int state)
{
	if (state == Qt::Checked)
	{
		m_ui.modelTextureShapPreserveBtn->setCheckState(Qt::Unchecked);
		emit deliverParameterizationInnerType(Z_AVERAGE);
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
		m_ui.modelTextureAverageBtn->setCheckState(Qt::Unchecked);
		emit deliverParameterizationInnerType(Z_SHAP_PRESERVING);
	}
	else
	{
		emit deliverParameterizationInnerType(Z_NONE);
	}
}

