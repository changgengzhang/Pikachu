#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <qopenglwidget.h>
#include <qopenglfunctions.h>
#include <qopenglvertexarrayobject.h>
#include <qopenglshaderprogram.h>
#include <qopenglbuffer.h>
#include <qopengl.h>
#include <qevent.h>

namespace zcg {
	class RenderView;
}

class RenderView : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	RenderView(QWidget *parent);
	~RenderView();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	
	void mousePressEvent(QMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QMouseEvent *mouseEvent) override;

private slots:
	void cleanup();


private:
	float m_scrWidth;
	float m_scrHeight;



};




#endif


