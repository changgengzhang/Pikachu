#include "RenderView.h"

RenderView::RenderView(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m_model = new Model();

	m_vertexShaderFilePath = "./shader/modelShader.vert";
	m_fragmentShaderFilePath = "./shader/modelShader.frag";
}

RenderView::~RenderView()
{
}

void RenderView::initializeGL()
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &RenderView::cleanup);
	initializeOpenGLFunctions();

	glViewport(0, 0, RenderViewWidth, RenderViewHeight);
	glClearColor(1.0f, 1.0f, 1.0f, 1);

	m_modelMat = glm::mat4(1.0f);
	m_viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_projMat = glm::perspective(glm::radians(45.0f), RenderViewWidth / RenderViewHeight, 0.01f, 100.0f);
}


void RenderView::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set MVP model
	m_model->setViewMatValue(m_viewMat);
	m_model->setProjMatValue(m_projMat);
	//m_model->setModelMatValue(m_modelMat);

	m_model->draw();
}


/*
Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been resized
*/
void RenderView::resizeGL(int width, int height)
{
	m_projMat = glm::mat4(1.0f);
	m_projMat = glm::perspective(45.0f, RenderViewWidth / RenderViewHeight, 0.01f, 100.0f);
}


void RenderView::mousePressEvent(QMouseEvent *mouseEvent)
{
	makeCurrent();
	m_model->mousePressEvent(mouseEvent);
	update();
}


void RenderView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	makeCurrent();
	m_model->mouseReleaseEvent(mouseEvent);
	update();
}


void RenderView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
	makeCurrent();
	m_model->mouseMoveEvent(mouseEvent);
	update();
}


// ================= slots =================
void RenderView::cleanup()
{
	delete m_model;
}


void RenderView::onModelDelBtnClicked()
{
	makeCurrent();
	m_model->destoryRender();
	update();
}


void RenderView::onModelTextureDelBtnClicked()
{
	makeCurrent();
	m_model->setTextureFileName("");
	m_model->buildMesh(m_vertexShaderFilePath, m_fragmentShaderFilePath);
	update();
}


void RenderView::acceptString(FileType fileType, QString fileName)
{
	makeCurrent();
	switch (fileType)
	{
	case FileType::MODEL:
		m_model->setModelFileName(fileName);
		m_model->buildMesh(m_vertexShaderFilePath, m_fragmentShaderFilePath);
		break;
	case FileType::TEXTURE:
		m_model->setTextureFileName(fileName);
		m_model->attachTexture(m_vertexShaderFilePath, m_fragmentShaderFilePath);
		break;
	default:
		break;
	}
	update();
}


void RenderView::acceptPolygonType(MeshPolygonType polygonType)
{
	makeCurrent();
	m_model->setPolygonWay(polygonType);
	update();
}


void RenderView::acceptParameterizationInnerType(ParameterizationInnerType innerType)
{
	makeCurrent();
	m_model->setParameterizationInnerType(innerType);
	m_model->attachTexture(m_vertexShaderFilePath, m_fragmentShaderFilePath);
	update();
}