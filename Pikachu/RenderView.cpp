#include "RenderView.h"

RenderView::RenderView(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m_model = new Model();
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

	m_model->loadMeshFromFile("./sphere.ply");
	m_model->buildShaderProgram("./shader/modelShader.vert", "./shader/modelShader.frag");
	m_model->buildVAOAndVBO();
}

/*
Renders the OpenGL scene. Gets called whenever the widget needs to be updated.
If you need to trigger a repaint from places other than paintGL()(a typical example is when using timers to animate scenes),
you should call the widget's update() function to schedule an update.
*/
void RenderView::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_model->draw();
}

/*
Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been resized
*/
void RenderView::resizeGL(int width, int height)
{
	
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

void RenderView::cleanup()
{

}