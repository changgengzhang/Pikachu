#include "RenderView.h"

RenderView::RenderView(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m_model = new Model();
	m_scrWidth = 1000;
	m_scrHeight = 800;

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

	glViewport(0, 0, m_scrWidth, m_scrWidth);
	glClearColor(1.0f, 1.0f, 1.0f, 1);

	m_modelMat = glm::mat4(1.0f);
	m_viewMat = glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_projMat = glm::perspective(glm::radians(45.0f), m_scrWidth / m_scrWidth, 0.01f, 100.0f);

}

void RenderView::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set MVP model
	if (m_model->isModleLoaded())
	{
		m_model->setModelMatValue(m_modelMat);
		m_model->setViewMatValue(m_viewMat);
		m_model->setProjMatValue(m_projMat);

		m_model->draw();
	}

}

/*
Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been resized
*/
void RenderView::resizeGL(int width, int height)
{
	m_projMat = glm::mat4(1.0f);
	m_projMat = glm::perspective(45.0f, m_scrWidth / m_scrHeight, 0.01f, 100.0f);
}

void RenderView::mousePressEvent(QMouseEvent *mouseEvent)
{

}

void RenderView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{

}

void RenderView::mouseMoveEvent(QMouseEvent *mouseEvent)
{

}


// ================ helper function =============
void RenderView::buildModel()
{
	makeCurrent();
	m_model->loadMeshFromFile(m_modelFilePath);
	m_model->buildShaderProgram(m_vertexShaderFilePath, m_fragmentShaderFilePath);
	m_model->buildVAOAndVBO();
}

// ================= slots =================
void RenderView::cleanup()
{
	delete m_model;
}


void RenderView::getModelFilePath(QString filePath)
{
	m_modelFilePath = filePath;
	buildModel();
	update();
}