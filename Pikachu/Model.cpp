#include "Model.h"

Model::Model(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m_canDraw = false;
	m_mesh = nullptr;
	m_polygonWay = MeshPolygonType::NONE_POLYGON_TYPE;
	
	m_parameterization = nullptr;
	m_hasTexture = false;
	m_parameterizationInnerType = ParameterizationInnerType::NONE_INNER_TYPE;

	m_shaderProgram = nullptr;
	m_arcball = nullptr;
}


Model::~Model()
{
	this->destoryRender();
}


void Model::buildMesh(QString vertexShaderFile, QString fragmentShaderFile)
{
	m_hasTexture = m_parameterizationInnerType != ParameterizationInnerType::NONE_INNER_TYPE && !m_textureFileName.isEmpty();

	this->loadMeshFromFile(m_modelFileName);

	if (m_hasTexture)
	{
		this->buildMeshParameterization(ParameterizationBoundaryType::SQUARE, m_parameterizationInnerType);
	}

	this->buildShaderProgram(vertexShaderFile, fragmentShaderFile);

	this->getUniformLoc();

	this->buildVAOAndVBO();

	if (m_hasTexture)
	{
		this->buildTexture(m_textureFileName);
	}

	// can draw
	m_canDraw = true;

	// =========== initalize arcball =======
	m_arcball = new ArcBall(RenderViewWidth, RenderViewWidth, 0.1f);
}


void Model::attachTexture(QString vertexShaderFile, QString fragmentShaderFile)
{
	m_hasTexture = m_parameterizationInnerType != ParameterizationInnerType::NONE_INNER_TYPE && !m_textureFileName.isEmpty();
	if (m_canDraw && m_hasTexture)
	{
		this->buildMeshParameterization(ParameterizationBoundaryType::SQUARE, m_parameterizationInnerType);
		this->buildShaderProgram(vertexShaderFile, fragmentShaderFile);
		this->getUniformLoc();
		this->buildVAOAndVBO();
		this->buildTexture(m_textureFileName);
	}
}


// ========= OpenGL context build function =============
bool Model::loadMeshFromFile(QString modelFile)
{
	// is created
	if (m_mesh != nullptr) delete m_mesh;
	
	m_mesh = new Mesh();
	bool ret = m_mesh->buildMesh(modelFile);

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
	if (m_shaderProgram != nullptr) delete m_shaderProgram;

	makeCurrent();
	initializeOpenGLFunctions();

	m_shaderProgram = new QOpenGLShaderProgram;
	
	m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile);
	m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile);

	// link for compile shader
	m_shaderProgram->link();
}


void Model::buildMeshParameterization(ParameterizationBoundaryType boundaryType, ParameterizationInnerType innerType)
{
	if (m_parameterization != nullptr) delete m_parameterization;

	m_parameterization = new Parameterization(
		m_mesh->getVertexPos(),
		m_mesh->getFaceIndex(),
		m_mesh->getIsBoundary(),
		m_mesh->getAdjacentVV(),
		m_mesh->getBoundaryVertexCount()
	);
	m_parameterization->calculate(boundaryType, innerType);
	
	// set texture coordiante
	m_mesh->setTextureCoordinate(m_parameterization->getParameterizedResult(SpatialDimension::D2));
}


// build vao and vbo, and initialize arcballl
void Model::buildVAOAndVBO()
{
	if (m_mesh == nullptr) return;

	if (m_shaderProgram == nullptr) return;

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

	// allocate memory for vertex position and normal
	int unitSize = m_mesh->getVertexCount() * sizeof(float);
	int offset = 0;
	if (m_hasTexture)
	{
		m_vbo.allocate(unitSize * (3 + 3 + 2));
	}
	else
	{
		m_vbo.allocate(unitSize * (3 + 3 ));
	}

	// position
	glEnableVertexAttribArray(0);
	m_vbo.write(offset, m_mesh->getVertexPos().constData(), unitSize * 3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	offset += unitSize * 3;

	// normal
	glEnableVertexAttribArray(1);
	m_vbo.write(offset, m_mesh->getVertexNormal().constData(), unitSize * 3);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(offset));
	offset += unitSize * 3;

	// texture 
	if (m_hasTexture)
	{
		glEnableVertexAttribArray(2);
		m_vbo.write(offset, m_mesh->getTextureCoordinate().constData(), unitSize * 2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(offset));
	}

	m_vbo.release();
	m_vao.release();
	m_shaderProgram->release();	
}


void Model::buildTexture(QString textureFile)
{
	if (m_shaderProgram == nullptr) return;

	makeCurrent();
	m_shaderProgram->bind();
	m_vao.bind();

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_texture = cv::imread(textureFile.toStdString());

	if (m_texture.empty())
	{
		qDebug() << "ERROR::IMAGE::setup texture failed";
	}
	else
	{
		// image may align
		glPixelStorei(GL_UNPACK_ALIGNMENT, (m_texture.step & 3) ? 1 : 4);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, m_texture.step / m_texture.elemSize());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texture.cols, m_texture.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, m_texture.ptr());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	m_vao.release();
	m_shaderProgram->release();
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
	case MeshPolygonType::NONE_POLYGON_TYPE:
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

	if (m_polygonWay != MeshPolygonType::NONE_POLYGON_TYPE)
	{
		if (m_hasTexture)
			glBindTexture(GL_TEXTURE_2D, m_textureID);
		glDrawElements(GL_TRIANGLES, m_mesh->getFaceCount() * 3, GL_UNSIGNED_INT, m_mesh->getFaceIndex().constData());
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


void Model::setParameterizationInnerType(ParameterizationInnerType innerType)
{
	m_parameterizationInnerType = innerType;
}


void Model::setModelFileName(QString modelFileName)
{
	m_modelFileName = modelFileName;
}


void Model::setTextureFileName(QString textureFileName)
{
	m_textureFileName = textureFileName;
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
const bool Model::getCanDraw() const
{
	return m_canDraw;
}


// =========== helper function =============
void Model::destoryRender()
{
	m_canDraw = false;

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