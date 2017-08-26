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

	void buildMesh(QString vertexShaderFile, QString fragmentShaderFile);
	void attachTexture(QString vertexShaderFile, QString fragmentShaderFile);
	void destoryRender();
	void draw();

	// ========= set uniform value ===========
	void setViewMatValue(glm::mat4 viewMat);
	void setProjMatValue(glm::mat4 projMat);

	void setPolygonWay(MeshPolygonType polygonWay);
	void setParameterizationInnerType(ParameterizationInnerType innerType);
	void setModelFileName(QString modelFileName);
	void setTextureFileName(QString textureFileName);

	// ======== flags value get =============
	const bool getCanDraw() const;

	// ========= arcball ================
	ArcBall *m_arcball;

private:
	// ========= OpenGL context build function =============
	bool loadMeshFromFile(QString modelFile);
	void buildShaderProgram(QString vertexFile, QString fragmentFile);
	void buildMeshParameterization(ParameterizationBoundaryType boundaryType, ParameterizationInnerType innerType);
	void buildVAOAndVBO();
	void buildTexture(QString textureFile);

	// ========= tools functions ============	
	void getUniformLoc();
	void setUniformValue();
	void setModelMatValue();

private:
	
	// ========== value ==============
	bool m_canDraw;

	// model
	Mesh* m_mesh;
	QString m_modelFileName;
	MeshPolygonType m_polygonWay;

	// texture
	Parameterization *m_parameterization;
	QString m_textureFileName;
	ParameterizationInnerType m_parameterizationInnerType;
	bool m_hasTexture;
	cv::Mat m_texture;

	// ========  shader ===============
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

};

#endif // !MODEL_H


