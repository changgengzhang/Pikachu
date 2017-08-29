#ifndef MODEL_H
#define MODEL_H

#include <qopenglwidget.h>
#include <qopenglfunctions.h>
#include <qopenglvertexarrayobject.h>
#include <qopenglshaderprogram.h>
#include <qopenglbuffer.h>
#include <qopengl.h>
#include <qstring.h>
#include <qdebug.h>
#include <qevent.h>
// glm 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// OpenCV
#include <opencv2/opencv.hpp>

#include "Mesh.h"
#include "Util.h"
#include "ArcBall.h"
#include "Parameterization.h"
using namespace zcg;

class Model : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	
	Model(QWidget *parent=0);
	~Model();

	void buildMesh(QString vertexShaderFile, QString fragmentShaderFile, QString modelFile);
	void cleanup();
	void draw();
	void deleteTexture();

	// ========= set uniform value ===========
	void setCanRender(bool canRender = true);

	void setViewMatValue(glm::mat4 viewMat);
	void setProjMatValue(glm::mat4 projMat);

	void setPolygonWay(ZVALUE polygonWay);

	void setTextureFileName(QString textureFileName);
	void setParameterizationInnerType(ZVALUE innerType);

	// ========= arcball ================
	void mousePressEvent(QMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QMouseEvent *mouseEvent) override;

private:
	// ========= OpenGL context build function =============
	bool loadMeshFromFile(QString modelFile);
	void buildShaderProgram(QString vertexFile, QString fragmentFile);
	void buildVAOAndVBO();
	void generateTexture(QString textureFile);

	// ========= tools functions ============	
	void setUniformValue();

	void attachTexture();

private:
	bool m_canRender;
	// mesh
	Mesh* m_mesh;
	bool m_meshReady;
	QString m_modelFileName;
	ZVALUE m_polygonWay;

	// texture
	bool m_textureReady;
	QString m_textureFileName;
	ZVALUE m_paramInnerType;
	cv::Mat m_texture;
	
	// ========  shader ===============
	QString m_vertexShaderFile;
	QString m_fragmentShaderFile;
	QOpenGLShaderProgram *m_shaderProgram;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;
	GLuint m_textureID;

	// ========== uniform value ==========
	glm::mat4 m_modelMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;

	// ======== shader uniform value location =========
	GLuint m_modelMatLoc;
	GLuint m_viewMatLoc;
	GLuint m_projMatLoc;
	GLuint m_hasTextureLoc;

	// Arc ball
	ArcBall *m_arcball;
};

#endif // !MODEL_H


