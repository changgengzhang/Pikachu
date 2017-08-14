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
// ASSIMP library for load object
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// glm 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Util.h"

class Model : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	
	Model(QWidget *parent=0);
	~Model();

	// ========= OpenGL context build function =============
	bool loadMeshFromFile(QString fileName);
	void buildShaderProgram(QString vertexFile, QString fragmentFile);
	void buildVAOAndVBO();
	void draw();

	// ========= set uniform value ===========
	void setModelMatValue(glm::mat4 modelMat);
	void setViewMatValue(glm::mat4 viewMat);
	void setProjMatValue(glm::mat4 projMat);

	// ======== flags value get set =============
	bool isModelLoaded();
	void delModel();

private:
	// ========= tools functions ============	
	void getUniformLoc();
	void setUniformValue();

private:
	// ========== menber value ==============
	bool m_isModelLoaded;
	Mesh* m_mesh;
	QString m_fileName;
	zcg::MeshDisplayWay m_displayWay;

	// ========  shader value ===============
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


