#include "Model.h"

Model::Model(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m_isModelLoaded = false;
	m_polygonWay = zcg::NONE;

	m_mesh = nullptr;
	m_shaderProgram = nullptr;
	m_arcball = nullptr;
}


Model::~Model()
{
	this->delModel();
}


// ========= OpenGL context build function =============
bool Model::loadMeshFromFile(QString fileName)
{
	// is created
	if (m_mesh != nullptr)
	{
		delete m_mesh;
	}

	m_mesh = new Mesh();
	bool ret = m_mesh->buildMesh(fileName);

	if (ret)
	{
		return true;
	}
	else
	{
		qDebug() << "ERROR::MODEL::loadMeshFromFile: buildMesh Failed";
		return false;
	}
}


void Model::buildShaderProgram(QString vertexFile, QString fragmentFile)
{
	// is build
	if (m_shaderProgram != nullptr)
	{
		delete m_shaderProgram;
	}

	makeCurrent();
	initializeOpenGLFunctions();

	m_shaderProgram = new QOpenGLShaderProgram;
	
	m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile);
	m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile);

	// link for compile shader
	m_shaderProgram->link();

	// ==========  get uniform value location ============
	this->getUniformLoc();
}

// build vao and vbo, and initialize arcballl
void Model::buildVAOAndVBO()
{
	if (m_mesh == nullptr)
	{
		// todo: throw exception
		return;
	}

	if (m_shaderProgram == nullptr)
	{
		// todo: throw exception
		return;
	}

	makeCurrent();
	// initial
	if (m_vao.isCreated())
	{
		m_vao.destroy();
	}
	
	if (m_vbo.isCreated())
	{
		m_vbo.destroy();
	}
	// build
	m_shaderProgram->bind();
	m_vao.create();
	m_vao.bind();

	m_vbo.create();
	m_vbo.bind();
	
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// memory for vertex position and normal
	m_vbo.allocate(m_mesh->getVertexCount() * 2 * 3 * sizeof(float));
	// position
	glEnableVertexAttribArray(0);
	m_vbo.write(0, m_mesh->getVertexPos(), m_mesh->getVertexCount() * sizeof(float) * 3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	// normal
	glEnableVertexAttribArray(1);
	m_vbo.write(m_mesh->getVertexCount() * sizeof(float) * 3, m_mesh->getVertexNormal(), m_mesh->getVertexCount() * sizeof(float) * 3);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(m_mesh->getVertexCount() * sizeof(float) * 3));

	m_vbo.release();
	m_vao.release();
	m_shaderProgram->release();

	// can draw
	m_isModelLoaded = true;
	

	// =========== initalize arcball =======
	m_arcball = new ArcBall(RenderViewWidth, RenderViewWidth, 0.1f);
}

void Model::draw()
{
	makeCurrent();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEBUG_OUTPUT);

	// model matrix manage by arcball. One model has one arcball
	this->setModelMatValue();
	// set uniform value
	this->setUniformValue();

	m_shaderProgram->bind();
	m_vao.bind();
	
	// draw mode
	switch (m_polygonWay)
	{
	case MeshPolygonType::NONE:
		break;
	case MeshPolygonType::FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case MeshPolygonType::LINE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case MeshPolygonType::POINT:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	default:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	if (m_polygonWay != MeshPolygonType::NONE)
	{
		glDrawElements(GL_TRIANGLES, m_mesh->getFaceCount() * 3, GL_UNSIGNED_INT, m_mesh->getFaceIndex());
	}
	
	m_vao.release();
	m_shaderProgram->release();
}


// ========= set uniform value ===========
void Model::setModelMatValue()
{
	m_modelMat = m_arcball->getModelMatrix(m_viewMat);
}


void Model::setViewMatValue(glm::mat4 viewMat)
{
	m_viewMat = viewMat;
}


void Model::setProjMatValue(glm::mat4 projMat)
{
	m_projMat = projMat;
}

void Model::setPolygonWay(MeshPolygonType polygonWay)
{
	m_polygonWay = polygonWay;
}


// ========= tools functions ============	
void Model::getUniformLoc()
{
	m_shaderProgram->bind();
	
	m_modelMatLoc = m_shaderProgram->uniformLocation("modelMat");
	m_viewMatLoc = m_shaderProgram->uniformLocation("viewMat");
	m_projMatLoc = m_shaderProgram->uniformLocation("projMat");

	m_shaderProgram->release();
}

void Model::setUniformValue()
{
	m_shaderProgram->bind();

	glUniformMatrix4fv(m_modelMatLoc, 1, GL_FALSE, glm::value_ptr(m_modelMat));
	glUniformMatrix4fv(m_viewMatLoc, 1, GL_FALSE, glm::value_ptr(m_viewMat));
	glUniformMatrix4fv(m_projMatLoc, 1, GL_FALSE, glm::value_ptr(m_projMat));

	m_shaderProgram->release();
}

// ======== flags value get =============
const bool Model::isModelLoaded() const
{
	return m_isModelLoaded;
}


// =========== helper function =============
void Model::delModel()
{
	// pretend to delete
	m_isModelLoaded = false;

	if (m_mesh != nullptr)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}

	if (m_shaderProgram != nullptr)
	{
		delete m_shaderProgram;
		m_shaderProgram = nullptr;
	}

	if (m_vao.isCreated())
	{
		m_vao.destroy();
	}

	if (m_vbo.isCreated())
	{
		m_vbo.destroy();
	}

	if (m_arcball != nullptr)
	{
		delete m_arcball;
		m_arcball = nullptr;
	}
}