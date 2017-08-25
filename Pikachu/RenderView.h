#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <qopenglwidget.h>
#include <qopenglfunctions.h>
#include <qopenglvertexarrayobject.h>
#include <qopenglshaderprogram.h>
#include <qopenglbuffer.h>
#include <qopengl.h>
#include <qevent.h>

#include "Model.h"
#include "ArcBall.h"
#include "Util.h"
using namespace zcg;


class RenderView : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	RenderView(QWidget *parent);
	~RenderView();

protected:
	// ============== virtual function inherited form QOpenGLWidget ================
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	
	void mousePressEvent(QMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QMouseEvent *mouseEvent) override;

private:
	void buildModel();


private slots:
	void cleanup();

public slots: 
	void acceptFilePath(QString filePath);
	void onModelDelBtnClicked();
	void acceptPolygonType(MeshPolygonType polygonWay);


private:
	// ========= viewport ============

	//float m_scrWidth;
	//float m_scrHeight;

	// ========= MVP matrix ==========
	glm::mat4 m_modelMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;

	// ========= Model ===============
	Model *m_model;
	QString m_modelFilePath;

	// ========= shader file path =========
	QString m_vertexShaderFilePath;
	QString m_fragmentShaderFilePath;
};




#endif


