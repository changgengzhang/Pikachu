#include "RenderView.h"

RenderView::RenderView(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m_model = new Model();
	// m_scrWidth = 1000;
	// m_scrHeight = 800;

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
	if (m_model->isModelLoaded())
	{
		m_model->setViewMatValue(m_viewMat);
		m_model->setProjMatValue(m_projMat);
		//m_model->setModelMatValue(m_modelMat);
		
		m_model->draw();
	}

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
	float x = mouseEvent->x();
	float y = mouseEvent->y();

	if (mouseEvent->button() == Qt::LeftButton)
	{
		if (m_model->isModelLoaded())
		{
			m_model->m_arcball->mousePressCallback(Qt::LeftButton, x, y);
			update();
		}
	}
	else if (mouseEvent->button() == Qt::RightButton)
	{
		if (m_model->isModelLoaded())
		{
			m_model->m_arcball->mousePressCallback(Qt::RightButton, x, y);
			update();
		}
	}
	
}


void RenderView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	if (m_model->isModelLoaded())
	{
		m_model->m_arcball->mouseReleaseCallback();
		update();
	}
}


void RenderView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
	float x = mouseEvent->x();
	float y = mouseEvent->y();
	if (m_model->isModelLoaded())
	{
		m_model->m_arcball->mouseMoveCallback(x, y);
		update();
	}
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


void RenderView::acceptFilePath(QString filePath)
{
	m_modelFilePath = filePath;
	buildModel();
	update();
}


void RenderView::onModelDelBtnClicked()
{
	makeCurrent();
	m_model->delModel();
	update();
}


void RenderView::acceptPolygonType(zcg::MeshPolygonWay polygonWay)
{
	makeCurrent();
	m_model->setPolygonWay(polygonWay);
	update();
}