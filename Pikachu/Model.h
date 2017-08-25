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

	// ========= OpenGL context build function =============
	bool loadMeshFromFile(QString fileName);
	void buildShaderProgram(QString vertexFile, QString fragmentFile);
	void buildMeshParameterization(ParameterizationBoundaryType boundaryType, ParameterizationInnerType innerType);
	void buildVAOAndVBO();
	void draw();

	// ========= set uniform value ===========
	void setViewMatValue(glm::mat4 viewMat);
	void setProjMatValue(glm::mat4 projMat);
	void setPolygonWay(MeshPolygonType polygonWay);

	// ======== flags value get =============
	const bool isModelLoaded() const;
	
	// ======== helper =====================
	void delModel();

	// ========= arcball ================
	ArcBall *m_arcball;

private:
	// ========= tools functions ============	
	void getUniformLoc();
	void setUniformValue();
	void setModelMatValue();

private:
	
	// ========== value ==============
	// model
	Mesh* m_mesh;
	bool m_isModelLoaded;
	QString m_modelFileName;
	
	// polygon
	MeshPolygonType m_polygonWay;

	// texture
	Parameterization *m_parameterization;
	bool m_isTextureLoad;

	// ========  shader ===============
	QOpenGLShaderProgram *m_shaderProgram;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;
	
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


